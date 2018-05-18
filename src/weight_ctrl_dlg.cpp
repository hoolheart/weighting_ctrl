#include <QList>
#include <QSerialPortInfo>
#include <QMessageBox>
#include "weight_ctrl_dlg.h"
#include "ui_weight_ctrl_dlg.h"
#include "serial_interface.h"

WeightCtrlDlg::WeightCtrlDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WeightCtrlDlg)
{
    ui->setupUi(this);//setup UI

    srl = new SerialInterface(this);//create serial port interface

    QList<QSerialPortInfo> port_list = QSerialPortInfo::availablePorts();//get list of available ports
    if(port_list.isEmpty()) {
        QMessageBox::warning(this,windowTitle(),tr("No available serial port"));
    }
    QStringList port_name_list;//prepare port name list
    foreach (const QSerialPortInfo &info, port_list) {
        port_name_list.append(info.portName());//setup port name list
    }
    ui->comboPort->addItems(port_name_list);//set context of port combo
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

    if(port_name_list.size()>0) {
        SerialConf_ptr conf(new SerialConf(port_name_list[0]));//prepare default serial port config
        conf->baudrate = QSerialPort::Baud9600;//baudrate
        conf->dataBit  = QSerialPort::Data8;//data bit length
        conf->checkBit = QSerialPort::NoParity;//no parity
        conf->stopBit  = QSerialPort::OneStop;//stop bit length
        srl->configure(conf);//configure
    }
    showPortInfo();//show initial info
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
            showPortInfo();//refresh info
        }
    }
}

void WeightCtrlDlg::configurePort()
{
    //TODO use dialog
    showPortInfo();//refresh info
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
    //TODO
}

void WeightCtrlDlg::readData()
{
    //TODO
}

void WeightCtrlDlg::startContinousRead()
{
    //TODO
}

void WeightCtrlDlg::stopContinuousRead()
{
    //TODO
}

void WeightCtrlDlg::reset()
{
    //TODO
}

void WeightCtrlDlg::calibrate()
{
    //TODO
}

void WeightCtrlDlg::help()
{
    //TODO
}

void WeightCtrlDlg::showPortInfo()
{
    bool is_open = srl->isOpen();//open status
    const SerialConf_ptr conf = srl->getConfiguration();//get configuration
    ui->ledPort->setValue(is_open?LED_OK:LED_CLOSE);//LED
    ui->labelStatus->setText(tr("%1, %2, baudrate: %3, data bit: %4, parity: %5, stop bit: %6")
                             .arg(conf->name).arg(is_open?tr("on"):tr("off"))
                             .arg(conf->baudrate).arg(conf->dataBit)
                             .arg(SerialInterface::interpreteParity(conf->checkBit))
                             .arg(SerialInterface::interpreteStopBit(conf->stopBit)));//show status label
}
