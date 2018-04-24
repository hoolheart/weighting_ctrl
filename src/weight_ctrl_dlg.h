#ifndef WEIGHT_CTRL_DLG_H
#define WEIGHT_CTRL_DLG_H

#include <QDialog>

namespace Ui {
class WeightCtrlDlg;
}

class WeightCtrlDlg : public QDialog
{
    Q_OBJECT

public:
    explicit WeightCtrlDlg(QWidget *parent = 0);
    ~WeightCtrlDlg();

private:
    Ui::WeightCtrlDlg *ui;
};

#endif // WEIGHT_CTRL_DLG_H
