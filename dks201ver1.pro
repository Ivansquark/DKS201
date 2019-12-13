#-------------------------------------------------
#
# Project created by QtCreator 2019-08-21T15:09:55
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += serialport
CONFIG   += serialport
QT       += printsupport


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dks201ver1
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        add_chamber.cpp \
        add_source.cpp \
        averadc_com.cpp \
        averageadc.cpp \
        chambers.cpp \
        com.cpp \
        compensation.cpp \
        connect_com.cpp \
        date.cpp \
        emulator.cpp \
        emulhex.cpp \
        keyboard.cpp \
        main.cpp \
        mainwindow.cpp \
        measurement_config.cpp \
        measuring.cpp \
        measuring_mode.cpp \
        measuring_parameters.cpp \
        measuring_popup.cpp \
        setup.cpp \
        setup_commonsetup.cpp \
        setup_commonsetup_userlist.cpp \
        setup_commonsetup_userlist_setmanufactorer.cpp \
        setup_commonsetup_userlist_setoperator.cpp \
        setup_equipment.cpp \
        setup_equipment_advanced_settings.cpp \
        setup_test.cpp \
        sources.cpp \
        testcom.cpp \
        voltage.cpp \
        volthex.cpp \
    bd.cpp \
    bd_table.cpp \
    qcustomplot.cpp

HEADERS += \
        add_chamber.h \
        add_source.h \
        averadc_com.h \
        averageadc.h \
        chambers.h \
        com.h \
        compensation.h \
        connect_com.h \
        date.h \
        emulator.h \
        emulhex.h \
        keyboard.h \
        mainwindow.h \
        measurement_config.h \
        measuring.h \
        measuring_mode.h \
        measuring_parameters.h \
        measuring_popup.h \
        setup.h \
        setup_commonsetup.h \
        setup_commonsetup_userlist.h \
        setup_commonsetup_userlist_setmanufactorer.h \
        setup_commonsetup_userlist_setoperator.h \
        setup_equipment.h \
        setup_equipment_advanced_settings.h \
        setup_test.h \
        sources.h \
        testcom.h \
        voltage.h \
        volthex.h \
    bd.h \
    bd_table.h \
    qcustomplot.h

FORMS += \
        add_chamber.ui \
        add_source.ui \
        chambers.ui \
        compensation.ui \
        connect_com.ui \
        date.ui \
        emulator.ui \
        keyboard.ui \
        mainwindow.ui \
        measurement_config.ui \
        measuring.ui \
        measuring_mode.ui \
        measuring_parameters.ui \
        measuring_popup.ui \
        setup.ui \
        setup_commonsetup.ui \
        setup_commonsetup_userlist.ui \
        setup_commonsetup_userlist_setmanufactorer.ui \
        setup_commonsetup_userlist_setoperator.ui \
        setup_equipment.ui \
        setup_equipment_advanced_settings.ui \
        setup_test.ui \
        sources.ui \
        testcom.ui \
        voltage.ui \
    bd.ui \
    bd_table.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    myres.qrc
