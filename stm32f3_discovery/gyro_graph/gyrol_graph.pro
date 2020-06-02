QT += charts core
CONFIG += console

HEADERS += \
    chart.h \
    mcuthread.h \
                stm32f3_discovery_l3gd20.h \
                stm32f3_discovery_lsm303dlhc.h \
                utils.h \
    viewcontroller.h

SOURCES += \
    chart.cpp \
    main.cpp \
    mcuthread.cpp \
                stm32f3_discovery_l3gd20.c \
                stm32f3_discovery_lsm303dlhc.c \
                utils.c \
    viewcontroller.cpp

TARGET = stm32f3_Discovery_gyro_graph
TEMPLATE = app

unix|win32: LIBS += -L$$PWD/ -lremcu

INCLUDEPATH += $$PWD/remcu_include

unix:QMAKE_RPATHDIR += $$PWD
