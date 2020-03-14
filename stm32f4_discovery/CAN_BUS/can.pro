QT += serialbus widgets
CONFIG += console
requires(qtConfig(combobox))

TARGET = can
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mcuthread.cpp \
    sendframebox.cpp

HEADERS += \
    mainwindow.h \
    mcuthread.h \
    sendframebox.h

FORMS   += mainwindow.ui \
    sendframebox.ui

RESOURCES += can.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/serialbus/can
INSTALLS += target

unix|win32: LIBS += -L$$PWD/ -lremcu

INCLUDEPATH += $$PWD/remcu_include

unix:QMAKE_RPATHDIR += $$PWD
