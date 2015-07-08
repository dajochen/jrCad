TEMPLATE = app
TARGET = jrCad

CONFIG += debug_and_release

DEPENDPATH += . \

INCLUDEPATH += . \
    ../jrlib ../jrlib/uic \


#HEADERS +=

#FORMS +=

SOURCES += main.cpp

QT += widgets

LIBS += -ldxflib \
        -L../jrlib -ljr
   
#RESOURCES += Icons.qrc
