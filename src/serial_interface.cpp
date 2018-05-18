#include "serial_interface.h"

SerialInterface::SerialInterface(QObject *parent) : QObject(parent)
{
    //prepare configuration
    conf = SerialConf_ptr(new SerialConf());
    //prepare buffers
    txBuf = new unsigned char(CONTROL_COMMAND_LEN);
    rxBuf = new unsigned char(RECEIVE_MESSAGE_LEN);
    //prepare check timer
    connect(&chkTimer,SIGNAL(timeout()),this,SLOT(checkStatus()));
    chkTimer.start(1000);
}

SerialInterface::~SerialInterface()
{
    if(port && port->isOpen()) {
        port->close();//close serial port if it has been open
    }
    delete txBuf;//release transmit buffer
    delete rxBuf;//release receive buffer
}

QString SerialInterface::lastError() const
{
    return err;
}

bool SerialInterface::configure(const SerialConf_ptr conf)
{
    if(isOpen()) {//check port status
        err = tr("The port can't be configured while it is open");//fill error
        return false;
    }
    else {
        if(checkConfiguration(conf,err)) {//check setting
            this->conf->copyFrom(*conf);//copy configuration info
            return true;
        }
        else {
            //err = tr("Invalid serial port configuration");//fill error
            return false;
        }
    }
}

const SerialConf_ptr SerialInterface::getConfiguration() const
{
    return conf;
}

bool SerialInterface::open()
{
    if(isOpen()) {
        return true;//the port has been open
    }
    else {
        if(port.isNull()) {
            port.reset(new QSerialPort());//create serial port instance
            connect(port.data(),SIGNAL(readyRead()),this,SLOT(onDataAvailable()));//connect read slot
            connect(port.data(),SIGNAL(errorOccurred(QSerialPort::SerialPortError)),this,SLOT(onPortError(QSerialPort::SerialPortError)));//error
            connect(port.data(),SIGNAL(bytesWritten(qint64)),this,SLOT(onDataWritten(qint64)));//written event
        }
        port->setPortName(conf->name);//name
        port->setBaudRate(conf->baudrate);//baudrate
        port->setDataBits((QSerialPort::DataBits)conf->dataBit);//data bits
        port->setParity((QSerialPort::Parity)conf->checkBit);//parity
        port->setStopBits((QSerialPort::StopBits)conf->stopBit);//stop bits
        if(port->open(QIODevice::ReadWrite)) {//try open port
            //succeess
            bytesToWrite = 0;//reset count of bytes to write
            rxIndex = 0;//reset receive buffer index
            return true;
        }
        else {
            //failed
            QSerialPort::SerialPortError port_err = port->error();//get error
            if(port_err==QSerialPort::NoError) {
                err = tr("Serial port failed to open with no error");//fill empty error
            }
            else {
                err = interpretePortError(port_err);//interprete error
                port->clearError();//clear error
            }
            return false;
        }
    }
}

bool SerialInterface::close()
{
    if(isOpen()) {
        port->close();//close serial port
        if(port->isOpen()) {
            err = tr("Failed to close serial port");//fill error
        }
        return !port->isOpen();
    }
    return true;//the port has been closed
}

bool SerialInterface::isOpen() const
{
    return (port && port->isOpen());
}

bool SerialInterface::sendCommand(const ControlCommand_ptr cmd)
{
    if(cmd) {//check command
        if(isOpen()) {//check port status
            cmd->serializeToByteArray(txBuf);//serial command
            if(port->write((char*)txBuf,CONTROL_COMMAND_LEN)!=CONTROL_COMMAND_LEN) {
                err = tr("Failed to write command via serial port");//fill error
                return false;
            }
            else {
                bytesToWrite += CONTROL_COMMAND_LEN;//increase count of bytes to write
                return true;
            }
        }
        else {
            err = tr("The serial port is now closed");//fill error
            return false;
        }
    }
    else {
        err = tr("Input command is invalid");//fill error
        return false;
    }
}

bool SerialInterface::hasPendingMessage() const
{
    return messages.size()>0;
}

ReceivedMessage_ptr SerialInterface::takeMessage()
{
    if(messages.size()>0) {
        return messages.takeFirst();//return the first pending message
    }
    return ReceivedMessage_ptr();//return null pointer
}

void SerialInterface::clearPendingMessage()
{
    messages.clear();//clear list
}

QString SerialInterface::interpreteParity(int parity)
{
    switch (parity) {
    case QSerialPort::NoParity:
        return tr("no parity");
    case QSerialPort::EvenParity:
        return tr("even parity");
    case QSerialPort::OddParity:
        return tr("odd parity");
    case QSerialPort::SpaceParity:
        return tr("space parity");
    case QSerialPort::MarkParity:
        return tr("mark parity");
    default:
        return tr("Invalid parity setting");
    }
}

QString SerialInterface::interpreteStopBit(int stop_bit)
{
    switch (stop_bit) {
    case QSerialPort::OneStop:
        return "1";
    case QSerialPort::OneAndHalfStop:
        return "1.5";
    case QSerialPort::TwoStop:
        return "2";
    default:
        return tr("Invalid stop bit setting");
    }
}

void SerialInterface::onDataAvailable()
{
    static unsigned char ch = 0;//prepare char
    if(!isOpen()) {//check port status first
        return;
    }
    while(port->bytesAvailable()>0) {//check available byte count
        if(port->read((char*)&ch,1)!=1) {
            break;//read failed
        }
        if((rxIndex<0) || (rxIndex>=RECEIVE_MESSAGE_LEN)) {//check current index
            rxIndex = 0;
        }
        if(rxIndex==0) {
            //head
            if(ch==0x5a) {//check value
                rxBuf[rxIndex++] = ch;//record
            }
        }
        else if(rxIndex==1) {
            //node
            if((ch>=0x1) && (ch<0xfe)) {//check value
                rxBuf[rxIndex++] = ch;//record
            }
            else {//restart
                rxIndex = 0;
            }
            if(ch==0x5a) {//check head
                rxBuf[rxIndex++] = ch;//record
            }
        }
        else if(rxIndex==2) {
            //length
            if(ch==0x09) {//check value
                rxBuf[rxIndex++] = ch;//record
            }
            else {//restart
                rxIndex = 0;
            }
            if(ch==0x5a) {//check head
                rxBuf[rxIndex++] = ch;//record
            }
        }
        else if(rxIndex<12) {
            //data
            rxBuf[rxIndex++] = ch;//record
        }
        else {
            //tail
            if(ch==0xff) {//check value
                rxBuf[rxIndex++] = ch;//record
            }
            else {//restart
                rxIndex = 0;
            }
            if(ch==0x5a) {//check head
                rxBuf[rxIndex++] = ch;//record
            }
        }
        if(rxIndex==RECEIVE_MESSAGE_LEN) {
            //received a complete message
            ReceivedMessage_ptr msg(new ReceivedMessage());//create a new message
            msg->parseFromByteArray(rxBuf);//parse data
            messages.append(msg);//append to list
            emit messageReceived();//emit signal
        }
    }
}

void SerialInterface::onDataWritten(qint64 len)
{
    if(isOpen()) {
        bytesToWrite -= len;//reduce count of bytes to write
    }
}

void SerialInterface::onPortError(QSerialPort::SerialPortError port_err)
{
    if(port_err!=QSerialPort::NoError) {//check error
        err = interpretePortError(port_err);//interprete error
        if(port) {
            port->clearError();//clear error
        }
        emit serialPortError(err);//emit signal
    }
}

void SerialInterface::checkStatus()
{
    if(isOpen()) {
        if(bytesToWrite<0) {
            //write more data
            err = tr("Bytes written to serial port are more than sent commands");//fill error
            emit serialPortError(err);//emit signal
        }
        else if(bytesToWrite>1000) {
            //too many pending bytes
            err = tr("Too many pending bytes (%1) to write, the serial port may has been down").arg(bytesToWrite);//fill error
            emit serialPortError(err);//emit signal
        }
    }
}

bool SerialInterface::checkConfiguration(const SerialConf_ptr &conf, QString &err)
{
    //check name
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();//get all available ports
    bool port_available = false;
    foreach(const QSerialPortInfo &info, ports) {
        if(info.portName()==conf->name) {//found matched port
            port_available = true;
            break;
        }
    }
    if(!port_available) {
        err = tr("The port %1 is not available").arg(conf->name);//fill error
        return false;
    }
    //check baudrate
    QList<qint32> baudrates = QSerialPortInfo::standardBaudRates();//get all standard baud rates
    bool br_available = false;
    foreach (qint32 br, baudrates) {
        if(br==conf->baudrate) {//found match
            br_available = true;
            break;
        }
    }
    if(!br_available) {
        err = tr("The baud rate %1 is not a standard baud rate").arg(conf->baudrate);//fill error
        return false;
    }
    //check parity
    if((conf->checkBit<QSerialPort::NoParity) || (conf->checkBit>QSerialPort::MarkParity) || (conf->checkBit==1)) {
        err = tr("Invalid setting to parity");//fill error
        return false;
    }
    //check data bits
    if((conf->dataBit<QSerialPort::Data5) || (conf->dataBit>QSerialPort::Data8)) {
        err = tr("Invalid setting to data bits");//fill error
        return false;
    }
    //check stop bits
    if((conf->stopBit<QSerialPort::OneStop) || (conf->stopBit>QSerialPort::OneAndHalfStop)) {
        err = tr("Invalid setting to stop bits");//fill error
        return false;
    }

    return true;
}

QString SerialInterface::interpretePortError(QSerialPort::SerialPortError port_err)
{
    switch (port_err) {
    case QSerialPort::NoError:
        return tr("No error");
    case QSerialPort::DeviceNotFoundError:
        return tr("Serial port is not found");
    case QSerialPort::PermissionError:
        return tr("Serial port has been used or user don't has enough permission");
    case QSerialPort::OpenError:
        return tr("Failed to open serial port");
    case QSerialPort::NotOpenError:
        return tr("Serial port has not been open");
    case QSerialPort::WriteError:
        return tr("An I/O error occurred while writing the data");
    case QSerialPort::ReadError:
        return tr("An I/O error occurred while reading the data");
    case QSerialPort::ResourceError:
        return tr("Serial port becomes unavailable");
    case QSerialPort::UnsupportedOperationError:
        return tr("The operation is not supported");
    case QSerialPort::TimeoutError:
        return tr("The operation times out");
    default:
        break;
    }
    return tr("Unknown or obsolete error code %1").arg(port_err);
}
