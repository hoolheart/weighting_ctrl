#-------------------------------------------------
#
# Project created by QtCreator 2018-04-24T22:16:11
#
#-------------------------------------------------

QT       += core gui widgets serialport

TARGET = weighting_ctrl
TEMPLATE = app
CONFIG(debug, debug|release): TARGET = $${TARGET}d
macx {
CONFIG -= app_bundle
}
VERSION = 1.0.0

DESTDIR = ../bin
MOC_DIR = ../gen/$$TARGET
UI_DIR = ../gen/$$TARGET
OBJECTS_DIR = ../gen/$$TARGET
RCC_DIR = ../gen/$$TARGET
INCLUDEPATH += ./

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    main.cpp \
    weight_ctrl_dlg.cpp \
    serial_interface.cpp \
    color_led.cpp \
    config_dlg.cpp \
    calibration_dlg.cpp

HEADERS += \
    weight_ctrl_dlg.h \
    serial_interface.h \
    data_structure.h \
    color_led.h \
    config_dlg.h \
    calibration_dlg.h

FORMS += \
    weight_ctrl_dlg.ui \
    config_dlg.ui \
    calibration_dlg.ui

RESOURCES += \
    res.qrc

TRANSLATIONS = weight_ctrl.ts
