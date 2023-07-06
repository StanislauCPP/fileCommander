QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AuxiableParentClasses.cpp \
    CopyInformationDialog.cpp \
    CreateNewDirectoryDialog.cpp \
    ThreadClasses.cpp \
    main.cpp \
    Mainwindow.cpp

HEADERS += \
    AuxiableParentClasses.h \
    CopyInformationDialog.h \
    CreateNewDirectoryDialog.h \
    Mainwindow.h \
    Side.h \
    ThreadClasses.h

FORMS += \
    CopyInformationDialog.ui \
    CreateNewDirectoryDialog.ui \
    Mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
