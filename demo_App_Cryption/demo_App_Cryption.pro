QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++23

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Đường dẫn tới thư mục include=
# INCLUDEPATH += "C:\msys64\ucrt64\include"
INCLUDEPATH += "C:\msys64\mingw64\include"

# Đường dẫn tới thư mục lib
LIBS += -L"C:\msys64\mingw64\lib" -lgcrypt
# LIBS += -L"C:/msys64/mingw64/lib" -lssl -lcrypto

SOURCES += \
    Cipher.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Cipher.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
