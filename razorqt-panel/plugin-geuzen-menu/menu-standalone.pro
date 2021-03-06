TEMPLATE = app
TARGET = gappmenu

QT += gui xml declarative

OBJECTS_DIR = temp/obj
UI_DIR = temp/ui
RCC_DIR = temp/rcc
MOC_DIR = temp/moc

LIBS += -lrazorqt -lrazorqxt -lqtxdg

QMAKE_CXXFLAGS += -std=c++0x

FORMS = \
    geuzen-menu-config.ui

RESOURCES = \
    geuzen-menu.qrc

SOURCES = \
    main-standalone.cpp \
    menu-standalone.cpp \
    geuzen-imagepro.cpp \
    menu-model.cpp \
    menu-view.cpp \
    menustyle.cpp

HEADERS = \
    menu-standalone.h \
    geuzen-imagepro.h \
    menu-model.h \
    menu-view.h \
    menustyle.h

target.path = /usr/local/bin

INSTALLS += target
