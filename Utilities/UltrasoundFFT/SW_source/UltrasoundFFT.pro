#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = UltrasoundFFT
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES +=  main.cpp\
            GraphicInterface.cpp \
            libplot\qcustomplot.cpp

HEADERS +=  GraphicInterface.h \
            libplot\qcustomplot.h\
            HS_DataLog\HS_DataLog.h

FORMS    += GraphicInterface.ui


INCLUDEPATH += $$PWD/ libplot
DEPENDPATH += $$PWD/ libplot

INCLUDEPATH += $$PWD/ HS_DataLog
DEPENDPATH += $$PWD/ HS_DataLog

win32:LIBS += "$$PWD/HS_DataLog/libhs_datalog.dll"
unix: LIBS += -L"$$PWD/HS_DataLog" -lhs_datalog

# copy DLL to target directory
win32:QMAKE_POST_LINK += copy /y $$shell_path($$PWD/HS_DataLog/libhs_datalog.dll) $$shell_path($$OUT_PWD)
unix: QMAKE_POST_LINK += cp $$shell_path($$PWD/HS_DataLog/libhs_datalog.dll) $$shell_path($$OUT_PWD)

RC_FILE = resources.rc

RESOURCES += resources.qrc

