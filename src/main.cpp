#include "weight_ctrl_dlg.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WeightCtrlDlg w;
    w.show();

    return a.exec();
}
