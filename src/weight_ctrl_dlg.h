#ifndef WEIGHT_CTRL_DLG_H
#define WEIGHT_CTRL_DLG_H

#include <QDialog>
#include <QTimer>
#include "data_structure.h"

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

signals:
    /**
     * @brief signal emitted to show AD value
     * @param val --- AD value
     */
    void adValueReceived(qint32 val);

private:
    Ui::WeightCtrlDlg *ui;
    SerialInterface *srl;/**< serial port interface */
    ControlCommand_ptr cmd;/**< control command to the device */
    unsigned char ctrlNode;/**< current controlling node index */
    QTimer readTimer;/**< timer to control continous reading */

private slots:
    void onPortStatusChanged(bool on);//control UI status according to port status
    void onPortChanged(int index);//change using port
    void configurePort();//configure serial port
    void controlPort();//control serial port
    void setNode();//set node index
    void readData();//read data once
    void startContinousRead();//start continuous reading of data
    void stopContinuousRead();//stop continuous reading
    void continousRead();//continuous read
    void reset();//reset data
    void calibrate();//calibrate device
    void help();//show help info
    void showPortInfo();//show current info of serial port

    void onPortError(QString err);//process error from serial port interface
    void onMessageReceived();//process received message from device

    void onCalibration(int point,double weight);//process calibration
};

#endif // WEIGHT_CTRL_DLG_H
