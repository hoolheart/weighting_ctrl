#include "weight_ctrl_dlg.h"
#include "ui_weight_ctrl_dlg.h"

WeightCtrlDlg::WeightCtrlDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WeightCtrlDlg)
{
    ui->setupUi(this);
}

WeightCtrlDlg::~WeightCtrlDlg()
{
    delete ui;
}
