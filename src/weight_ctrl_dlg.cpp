#include <QList>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QSettings>
#include <qmath.h>
#include <QDebug>
#include "weight_ctrl_dlg.h"
#include "ui_weight_ctrl_dlg.h"
#include "serial_interface.h"
#include "config_dlg.h"
#include "calibration_dlg.h"

WeightCtrlDlg::WeightCtrlDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WeightCtrlDlg),
    hasCommand(false)
{
    ui->setupUi(this);//setup UI

    srl = new SerialInterface(this);//create serial port interface
    cmd.reset(new ControlCommand());//create control command

    connect(srl,SIGNAL(messageReceived()),this,SLOT(onMessageReceived()));//trigger message process
    connect(srl,SIGNAL(serialPortError(QString)),this,SLOT(onPortError(QString)));//trigger error process

    QSettings setting;//prepare setting
    ctrlNode = (unsigned char)setting.value("device/node-index",1).toInt();//get control node from setting
    ui->spinNode->setValue(ctrlNode);//initialize node spinbox with control node index

    QList<QSerialPortInfo> port_list = QSerialPortInfo::availablePorts();//get list of available ports
    if(port_list.isEmpty()) {
        QMessageBox::warning(this,windowTitle(),tr("No available serial port"));
    }
    QStringList port_name_list;//prepare port name list
    foreach (const QSerialPortInfo &info, port_list) {
        port_name_list.append(info.portName());//setup port name list
    }
    ui->comboPort->addItems(port_name_list);//set context of port combo
    QString port_name;//prepare port name
    if(port_name_list.size()>0) {
        port_name = setting.value("port/name",QString()).toString();//get initial port name
        if(!port_name_list.contains(port_name)) {//invalid setting
            port_name = port_name_list[0];//use first available port
            setting.setValue("port/name",port_name);//record to setting
        }
        ui->comboPort->setCurrentText(port_name);//initialize combo
    }
    connect(ui->comboPort,SIGNAL(currentIndexChanged(int)),this,SLOT(onPortChanged(int)));//index change event

    connect(ui->btnPortConfig,SIGNAL(clicked(bool)),this,SLOT(configurePort()));//configure button
    connect(ui->btnPortCtrl,SIGNAL(clicked(bool)),this,SLOT(controlPort()));//control button
    connect(ui->btnNodeSet,SIGNAL(clicked(bool)),this,SLOT(setNode()));//set node button
    connect(ui->btnRead,SIGNAL(clicked(bool)),this,SLOT(readData()));//read button
    connect(ui->btnContRead,SIGNAL(clicked(bool)),this,SLOT(startContinousRead()));//continous read button
    connect(ui->btnStopRead,SIGNAL(clicked(bool)),this,SLOT(stopContinuousRead()));//stop read button
    connect(ui->btnReset,SIGNAL(clicked(bool)),this,SLOT(reset()));//reset button
    connect(ui->btnCalibrate,SIGNAL(clicked(bool)),this,SLOT(calibrate()));//calibrate button
    connect(ui->btnHelp,SIGNAL(clicked(bool)),this,SLOT(help()));//help button

    SerialConf_ptr conf(new SerialConf(port_name));//prepare default serial port config
    conf->baudrate = setting.value("port/baudrate",QSerialPort::Baud9600).toInt();//baudrate
    conf->dataBit  = setting.value("port/data-bit",QSerialPort::Data8).toInt();//data bit length
    conf->checkBit = setting.value("port/parity",QSerialPort::NoParity).toInt();//parity
    conf->stopBit  = setting.value("port/stop-bit",QSerialPort::OneStop).toInt();//stop bit length
    srl->configure(conf);//configure
    showPortInfo();//show initial info

    readTimer.setInterval(300);//continous read period
    connect(&readTimer,SIGNAL(timeout()),this,SLOT(continousRead()));//timer trigger read

    ui->lcdWeight->display("0.00");//initial weight
    ui->lcdAD->display("0");//initial AD value
    ui->lcdNode->display(QString::number(ctrlNode));//initial node

    onPortStatusChanged(false);//initial: port closed
}

WeightCtrlDlg::~WeightCtrlDlg()
{
    delete ui;
}

void WeightCtrlDlg::onPortStatusChanged(bool on)
{
    static QList<QWidget*> cfgUIs = QList<QWidget*>()<<ui->comboPort<<ui->btnPortConfig;//configure UI list
    static QList<QWidget*> wrkUIs = QList<QWidget*>()<<ui->spinNode<<ui->btnNodeSet
                                                     <<ui->btnRead<<ui->btnContRead<<ui->btnStopRead
                                                     <<ui->btnReset<<ui->btnCalibrate;//working UI list
    if(on) {//serial port on
        foreach (QWidget *wnd, cfgUIs) {
            wnd->setEnabled(false);//disable configure
        }
        foreach (QWidget *wnd, wrkUIs) {
            wnd->setEnabled(true);//enable working
        }
        ui->btnPortCtrl->setText(tr("Close"));//change control button text
    }
    else {//serial port off
        foreach (QWidget *wnd, cfgUIs) {
            wnd->setEnabled(true);//enable configure
        }
        foreach (QWidget *wnd, wrkUIs) {
            wnd->setEnabled(false);//disable working
        }
        ui->btnPortCtrl->setText(tr("Open"));//change control button text
    }
}

void WeightCtrlDlg::onPortChanged(int index)
{
    if((index>=0) && (index<ui->comboPort->count())) {//check index
        QString port_name = ui->comboPort->itemText(index);//get port name
        const SerialConf_ptr cur_conf = srl->getConfiguration();//get current configuration
        if(cur_conf->name!=port_name) {//check name
            SerialConf_ptr conf(new SerialConf());//prepare new configure
            conf->copyFrom(*cur_conf);//copy current configuration
            conf->name = port_name;//change name
            if(!srl->configure(conf)) {//configure
                QMessageBox::warning(this,windowTitle(),tr("Failed to configure:\n%1").arg(srl->lastError()));//pump error
            }
            port_name = srl->getConfiguration()->name;//get current port name
            ui->comboPort->setCurrentText(port_name);//set combo status
            QSettings().setValue("port/name",port_name);//record into setting
            showPortInfo();//refresh info
        }
    }
}

void WeightCtrlDlg::configurePort()
{
    ConfigDlg dlg(srl,this);//use dialog
    if(dlg.exec()==QDialog::Accepted) {//execute dialog
        const SerialConf_ptr conf = srl->getConfiguration();//get current configuration
        QSettings setting;//prepare to record configuration into setting
        setting.setValue("port/baudrate",conf->baudrate);//baudrate
        setting.setValue("port/data-bit",conf->dataBit);//data bit
        setting.setValue("port/parity",conf->checkBit);//parity
        setting.setValue("port/stop-bit",conf->stopBit);//stop bit
        showPortInfo();//refresh info
    }
}

void WeightCtrlDlg::controlPort()
{
    bool flag = false;//prepare flag
    if(srl->isOpen()) {//check current status
        flag = srl->close();//try close
    }
    else {
        flag = srl->open();//try open
    }
    if(!flag) {
        QMessageBox::warning(this,windowTitle(),tr("Failed to control serial port:\n%1").arg(srl->lastError()));//pump error
    }
    onPortStatusChanged(srl->isOpen());//control UI
    showPortInfo();//refresh info
}

void WeightCtrlDlg::setNode()
{
    //prepare command
    cmd->setNodeIndex((unsigned char)ui->spinNode->value());//node index
    cmd->setFuncCode(0x01);//func code
    //try control
    if(sendSrlCommand(tr("Failed to set device node"))) {
        ctrlNode = (unsigned char)ui->spinNode->value();//record
        QSettings().setValue("device/node-index",ctrlNode);//save into setting
    }
}

void WeightCtrlDlg::readData()
{
    //prepare command
    cmd->setNodeIndex(ctrlNode);//node index
    cmd->setFuncCode(0x00);//func code
    //try control
    sendSrlCommand(tr("Failed to read data"));
}

void WeightCtrlDlg::startContinousRead()
{
    readTimer.start();
}

void WeightCtrlDlg::stopContinuousRead()
{
    readTimer.stop();
}

void WeightCtrlDlg::continousRead()
{
    if(srl->isOpen()) {//check serial port status first
        //prepare command
        if(hasCommand) {
            hasCommand = false;//clear flag
        }
        else {
            cmd->setNodeIndex(ctrlNode);//node index
            cmd->setFuncCode(0x00);//func code
            cmdWarning = tr("Failed to read data");//warning
        }
        //try control
        if(!srl->sendCommand(cmd)) {
            QMessageBox::warning(this,windowTitle(),QString("%1:\n%2").arg(cmdWarning).arg(srl->lastError()));//pump error
        }
        cmd->setWeight(0);//clear weight
    }
    else {
        hasCommand = false;//clear flag
        readTimer.stop();//stop timer
    }
}

void WeightCtrlDlg::reset()
{
    //prepare command
    cmd->setNodeIndex(ctrlNode);//node index
    cmd->setFuncCode(0x02);//func code
    //try control
    sendSrlCommand(tr("Failed to reset data"));
}

void WeightCtrlDlg::calibrate()
{
    bool stop_read = !readTimer.isActive();//check continuous reading
    readTimer.start();//start continous reading
    CalibrationDlg dlg(this);//use dialog
    connect(&dlg,SIGNAL(calibration(int,double)),this,SLOT(onCalibration(int,double)));//process calibration
    connect(this,SIGNAL(adValueReceived(qint32)),&dlg,SLOT(showADValue(qint32)));//show AD value
    dlg.exec();//execute dialog
    if(stop_read) {
        readTimer.stop();//stop continuous reading
    }
}

void WeightCtrlDlg::help()
{
    //TODO
}

void WeightCtrlDlg::showPortInfo()
{
    bool is_open = srl->isOpen();//open status
    const SerialConf_ptr conf = srl->getConfiguration();//get configuration
    if(conf->name.isEmpty()) {//check name
        ui->ledPort->disable();//disable LED
        ui->labelStatus->setText(tr("Port unspecified"));//status
    }
    else {
        ui->ledPort->setValue(is_open?LED_OK:LED_CLOSE);//LED
        ui->labelStatus->setText(tr("%1, %2, baudrate: %3, data bit: %4, parity: %5, stop bit: %6")
                                 .arg(conf->name).arg(is_open?tr("on"):tr("off"))
                                 .arg(conf->baudrate).arg(conf->dataBit)
                                 .arg(SerialInterface::interpreteParity(conf->checkBit))
                                 .arg(SerialInterface::interpreteStopBit(conf->stopBit)));//show status label
    }
}

void WeightCtrlDlg::onPortError(QString err)
{
    QMessageBox::warning(this,windowTitle(),err);//show error
}

void WeightCtrlDlg::onMessageReceived()
{
    if(srl->hasPendingMessage()) {//check pending message
        ReceivedMessage_ptr msg = srl->takeMessage();//take next message
        if(msg) {//check message
            unsigned char node_indx = msg->getNodeIndex();//get node index
            ui->lcdNode->display(QString::number(node_indx));//show node index
            unsigned char func_code = msg->getFuncCode();//get functionality code
            if(func_code==0) {//read data
                qint32 ad_val = msg->getAdValue();//AD value
                double weight = msg->getWeight_g();//weight
                ui->lcdAD->display(QString::number(ad_val));//show AD value
                ui->lcdWeight->display(QString::number(weight,'f',2));//show weight
                emit adValueReceived(ad_val);//emit signal
            }
            else if(func_code==1) {//node setting
                qDebug()<<"[ECHO] set node"<<node_indx;
            }
            else if(func_code==2) {//reset
                qDebug()<<"[ECHO] reset data";
            }
            else if((func_code>4) && (func_code<0xf)) {//calibration
                qDebug()<<"[ECHO] calibration "<<func_code-0x4;
            }
            else if(func_code==0x81) {//failed to set node
                QMessageBox::warning(this,windowTitle(),tr("Failed to set node index"));
            }
            else if(func_code==0x82) {//failed to reset
                QMessageBox::warning(this,windowTitle(),tr("Failed to reset data"));
            }
            else if((func_code>0x84) && (func_code<0x8f)) {//failed to calibrate
                QMessageBox::warning(this,windowTitle(),tr("Failed to calibrate (point: %1)").arg(func_code-0x84));
            }
            else {//invalid code
                QMessageBox::warning(this,windowTitle(),tr("Invalid code 0x%1 from node %2").arg(func_code,2,16,QChar('0')).arg(node_indx));
            }
        }
    }
}

void WeightCtrlDlg::onCalibration(int point, double weight)
{
    //prepare command
    cmd->setNodeIndex(ctrlNode);//node index
    cmd->setFuncCode((unsigned char)(4+point));//func code
    cmd->setWeight((qint32)qRound(weight));//weight
    //try control
    sendSrlCommand(tr("Failed to calibrate"));
}

bool WeightCtrlDlg::sendSrlCommand(const QString &warn)
{
    bool ok = false;
    if(srl->isOpen()) {//check serial port
        if(readTimer.isActive()) {//check read timer
            hasCommand = true;//set flag
            cmdWarning = warn;//record warning info
            return true;
        }
        else {
            ok = srl->sendCommand(cmd);//send command
            if(!ok) {//warn on failure
                QMessageBox::warning(this,windowTitle(),QString("%1:\n%2").arg(warn).arg(srl->lastError()));
            }
        }
    }
    else {
        QMessageBox::warning(this,windowTitle(),tr("Port has not been open"));
    }
    cmd->setWeight(0);//clear weight
    return ok;
}
