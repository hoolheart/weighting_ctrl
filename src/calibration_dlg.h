#ifndef CALIBRATION_DLG_H
#define CALIBRATION_DLG_H

#include <QDialog>

namespace Ui {
class CalibrationDlg;
}

/** dialog to calibrate device */
class CalibrationDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CalibrationDlg(QWidget *parent = 0);
    ~CalibrationDlg();

public slots:
    /**
     * @brief slot to show AD value
     * @param val --- AD value
     */
    void showADValue(qint32 val);

signals:
    /**
     * @brief signal emitted to trigger calibration
     * @param point --- calibration point
     * @param weight --- target weight
     */
    void calibration(int point, double weight);

private:
    Ui::CalibrationDlg *ui;

private slots:
    void calibrate();//trigger calibration
};

#endif // CALIBRATION_DLG_H
