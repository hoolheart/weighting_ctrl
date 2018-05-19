#ifndef CONFIG_DLG_H
#define CONFIG_DLG_H

#include <QDialog>

class SerialInterface;

namespace Ui {
class ConfigDlg;
}

/** dialog to configure serial interface */
class ConfigDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDlg(SerialInterface *srl_int, QWidget *parent = 0);
    ~ConfigDlg();

private:
    Ui::ConfigDlg *ui;
    SerialInterface *srl;/**< configured interface */

private slots:
    void configure();/**< configure serial interface according to user setting */
};

#endif // CONFIG_DLG_H
