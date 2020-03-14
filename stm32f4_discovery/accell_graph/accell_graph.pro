QT += charts core
CONFIG += console

HEADERS += \
    chart.h \
    mcuthread.h \
    stm32f4_discovery_lis302dl.h \
    viewcontroller.h

SOURCES += \
    chart.cpp \
    main.cpp \
    mcuthread.cpp \
    stm32f4_discovery_lis302dl.c \
    viewcontroller.cpp

TARGET = stm32f4_Discovery_accell_graph
TEMPLATE = app

unix|win32: LIBS += -L$$PWD/ -lremcu

INCLUDEPATH += $$PWD/remcu_include

unix:QMAKE_RPATHDIR += $$PWD
