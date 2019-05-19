TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt
SUBDIRS += src/
SOURCES += \
        main.cpp \
    src/blockchain.cpp \
    src/sha256.cpp

HEADERS += \
    src/blockchain.hpp \
    src/sha256.hpp

DISTFILES += \
    LICENSE

unix:!macx: LIBS += -luuid
