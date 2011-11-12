cmake_minimum_required(VERSION 2.6)

set(PLUGIN "mainmenu")


set(HEADERS
    geuzen-main-menu.h
    menustyle.h
    geuzen-menu-config.h
)

set(SOURCES
    geuzen-main-menu.cpp
    menustyle.cpp
    geuzen-menu-config.cpp
)

set(MOCS
    geuzen-main-menu.h
    menustyle.h
    geuzen-menu-config.h
)

set(UIS
    geuzen-menu-config.ui
)

set(LIBRARIES
  razorqt
)

set(QT_USE_QTXML 1)
set(QT_USE_QTDBUS 1)

#*******************************************
include ("../BuildPlugin.cmake")
BUILD_RAZOR_PLUGIN(${PLUGIN})
