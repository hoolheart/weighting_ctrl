#include <QSerialPortInfo>
#include <QMessageBox>
#include "config_dlg.h"
#include "ui_config_dlg.h"
#include "serial_interface.h"

ConfigDlg::ConfigDlg(SerialInterface *srl_int, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDlg),
    srl(srl_int)
{
    ui->setupUi(this);

    //setup combo of baudrate
    QList<qint32> br_list = QSerialPortInfo::standardBaudRates();//get standard baudrate list
    foreach (qint32 br, br_list) {
        ui->comboBaudrate->addItem(QString::number(br),br);//add every baudrate into combo
    }
    //setup combo of parity
    QList<int> parity_list = QList<int>()<<QSerialPort::NoParity
                                         <<QSerialPort::EvenParity<<QSerialPort::OddParity
                                         <<QSerialPort::SpaceParity<<QSerialPort::MarkParity;//list of available parity setting
    foreach (int parity, parity_list) {
        ui->comboParity->addItem(SerialInterface::interpreteParity(parity),parity);//add every parity into combo
    }
    //setup combo of data bit
    QList<int> db_list = QList<int>()<<QSerialPort::Data5<<QSerialPort::Data6<<QSerialPort::Data7<<QSerialPort::Data8;//list of available data bits
    foreach (int data_bit, db_list) {
        ui->comboDataBit->addItem(QString::number(data_bit),data_bit);//add every data bit number into combo
    }
    //setup combo of stop bit
    QList<int> sb_list = QList<int>()<<QSerialPort::OneStop<<QSerialPort::OneAndHalfStop<<QSerialPort::TwoStop;//list of available stop bits
    foreach (int stop_bit, sb_list) {
        ui->comboStopBit->addItem(SerialInterface::interpreteStopBit(stop_bit),stop_bit);//add every stop bit number into combo
    }

    //show current setting
    const SerialConf_ptr cur_conf = srl->getConfiguration();//get current configuration
    ui->comboBaudrate->setCurrentIndex(ui->comboBaudrate->findData(cur_conf->baudrate));//baudrate
    ui->comboParity->setCurrentIndex(ui->comboParity->findData(cur_conf->checkBit));//parity
    ui->comboDataBit->setCurrentIndex(ui->comboDataBit->findData(cur_conf->dataBit));//data bit
    ui->comboStopBit->setCurrentIndex(ui->comboStopBit->findData(cur_conf->stopBit));//stop bit

    //define button behavior
    connect(ui->btnConfirm,SIGNAL(clicked(bool)),this,SLOT(configure()));//confirm
    connect(ui->btnCancel,SIGNAL(clicked(bool)),this,SLOT(reject()));//cancel
}

ConfigDlg::~ConfigDlg()
{
    delete ui;
}

void ConfigDlg::configure()
{
    //prepare configure
    SerialConf_ptr conf(new SerialConf(srl->getConfiguration()->name));//create with current name
    conf->baudrate = ui->comboBaudrate->currentData().toInt();//baudrate
    conf->checkBit = ui->comboParity->currentData().toInt();//parity
    conf->dataBit = ui->comboDataBit->currentData().toInt();//data bit
    conf->stopBit = ui->comboStopBit->currentData().toInt();//stop bit

    //try configure
    if(srl->configure(conf)) {
        accept();//success
    }
    else {
        QMessageBox::warning(this,windowTitle(),tr("Failed to configure:\n%1").arg(srl->lastError()));//show error
    }
}
