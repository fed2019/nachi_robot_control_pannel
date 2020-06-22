QT       += core gui\
           datavisualization
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
RC_ICONS = mz07.ico


CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    controlrobot.cpp \
    main.cpp \
    controlpannel.cpp \
    readcamera.cpp \
    subwindow.cpp

HEADERS += \
    controlpannel.h \
    controlrobot.h \
    readcamera.h \
    subwindow.h

FORMS += \
    controlpannel.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix|win32: LIBS += -L'C:/Program Files (x86)/nachi/OpenNR-IF/' -lOpenNR-IF

INCLUDEPATH += 'C:/Program Files (x86)/nachi/OpenNR-IF'
DEPENDPATH += 'C:/Program Files (x86)/nachi/OpenNR-IF'
#INCLUDEPATH += 'G:/QTCodes/AutoTeach_Ver01/OpenNR-IF'
#DEPENDPATH += 'G:/QTCodes/AutoTeach_Ver01/OpenNR-IF'
