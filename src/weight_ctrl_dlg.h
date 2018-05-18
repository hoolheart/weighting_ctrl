#ifndef WEIGHT_CTRL_DLG_H
#define WEIGHT_CTRL_DLG_H

#include <QDialog>

namespace Ui {
class WeightCtrlDlg;
}

class SerialInterface;

/** dialog to control weight calibration device */
class WeightCtrlDlg : public QDialog
{
    Q_OBJECT

public:
    explicit WeightCtrlDlg(QWidget *parent = 0);
    ~WeightCtrlDlg();

private:
    Ui::WeightCtrlDlg *ui;
    SerialInterface *srl;/**< serial port interface */

private slots:
    void onPortStatusChanged(bool on);//control UI status according to port status
    void onPortChanged(int index);//change using port
    void configurePort();//configure serial port
    void controlPort();//control serial port
    void setNode();//set node index
    void readData();//read data once
    void startContinousRead();//start continuous reading of data
    void stopContinuousRead();//stop continuous reading
    void reset();//reset data
    void calibrate();//calibrate device
    void help();//show help info
    void showPortInfo();//show current info of serial port
};

#endif // WEIGHT_CTRL_DLG_H
