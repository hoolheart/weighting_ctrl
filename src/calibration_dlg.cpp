#include "calibration_dlg.h"
#include "ui_calibration_dlg.h"

CalibrationDlg::CalibrationDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibrationDlg)
{
    ui->setupUi(this);

    connect(ui->btnCalibration,SIGNAL(clicked(bool)),this,SLOT(calibrate()));//define button behavior
}

CalibrationDlg::~CalibrationDlg()
{
    delete ui;
}

void CalibrationDlg::showADValue(qint32 val)
{
    ui->editAD->setText(QString::number(val));//show value
}

void CalibrationDlg::calibrate()
{
    emit calibration(ui->spinPoint->value(),ui->spinWeight->value());//emit signal
}
