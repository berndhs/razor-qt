cmake_minimum_required(VERSION 2.6)

set(PLUGIN "mainmenu")


set(HEADERS
    menu-view.h
    menu-model.h
    geuzen-main-menu.h
    geuzen-menu-config.h
)

set(SOURCES
    menu-view.cpp
    menu-model.cpp
    geuzen-main-menu.cpp
    geuzen-menu-config.cpp
)

set(MOCS
    menu-view.h
    menu-model.h
    geuzen-main-menu.h
    geuzen-menu-config.h
)

set(UIS
    geuzen-menu-config.ui
)

set(RESOURCES
    geuzen-menu.qrc
)

set(LIBRARIES
  razorqt
)

set(QT_USE_QTXML 1)
set(QT_USE_QTDBUS 1)
set(QT_USE_DECLARATIVE 1)

#*******************************************
include ("../BuildPlugin.cmake")
BUILD_RAZOR_PLUGIN(${PLUGIN})
