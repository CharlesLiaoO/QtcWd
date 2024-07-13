QT = core

CONFIG += c++17 cmdline
CONFIG+=force_debug_info

DESTDIR = $$PWD/bin

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    Common/GetProcessInfo.h \
    Common/KeyShortcut.h \
    QtcDeal.h

SOURCES += \
        Common/GetProcessInfo.cpp \
        Common/KeyShortcut.cpp \
        QtcDeal.cpp \
        main.cpp

win32 {
LIBS += -luser32
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
