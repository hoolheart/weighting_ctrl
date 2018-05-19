#include <QApplication>
#include <QTranslator>
#include "weight_ctrl_dlg.h"

int main(int argc, char *argv[])
{
    //setup application
    QApplication a(argc, argv);//create application
    a.setOrganizationName("tianxiang");//organization name
    a.setOrganizationDomain("tianxiang.com.cn");//organization domain
    a.setApplicationName("weighting-ctrl");//application name
    QTranslator t;//prepare translator
    if(t.load("weight_ctrl")) {//load translation file
        a.installTranslator(&t);//translate
    }

    //setup GUI
    WeightCtrlDlg w;//create main dialog
    w.show();//display dialog

    return a.exec();
}
