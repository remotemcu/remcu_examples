QT += serialbus widgets
CONFIG += console
requires(qtConfig(combobox))

TARGET = lcd_stm8l_discovery
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mcuaction.cpp \
    stm8l_discovery_lcd.c

HEADERS += \
    mainwindow.h \
    mcuaction.h \
    stm8l_discovery_lcd.h

FORMS   += mainwindow.ui

unix|win32: LIBS += -L$$PWD/ -lremcu

INCLUDEPATH += $$PWD/remcu_include

unix:QMAKE_RPATHDIR += $$PWD
