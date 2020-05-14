QT += widgets

CONFIG += console

HEADERS       = analogcompas.h \
                stm32f3_discovery_l3gd20.h \
                stm32f3_discovery_lsm303dlhc.h \
                utils.h \
                mcuthread.h

SOURCES       = analogcompas.cpp \
                mcuthread.cpp\
                stm32f3_discovery_l3gd20.c \
                stm32f3_discovery_lsm303dlhc.c \
                utils.c \
                main.cpp

QMAKE_PROJECT_NAME = widgets_analogclock

# install
TARGET = stm32f3_Discovery_compas

INSTALLS += target

unix|win32: LIBS += -L$$PWD/ -lremcu

INCLUDEPATH += $$PWD/remcu_include

unix:QMAKE_RPATHDIR += $$PWD
