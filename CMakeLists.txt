cmake_minimum_required(VERSION 3.5)

project(modern-UI VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

file(GLOB PACKET_PARSING
    "src/packet_parsing/asdu/file_service/*.h"
    "src/packet_parsing/*.h"
    "src/packet_parsing/*.cpp"
    "src/packet_parsing/asdu/*.h"
    "src/packet_parsing/asdu/*.cpp"
    "src/packet_parsing/default/*.h"
    "src/packet_parsing/default/*.cpp"
    "src/packet_parsing/south/*.h"
    "src/packet_parsing/south/*.cpp"
    "src/packet_parsing/south/ASDUSouth/*.h"
    "src/packet_parsing/south/ASDUSouth/*.cpp"
    "src/packet_parsing/nlohmann/json.hpp"
)

set(PROJECT_SOURCES
    ${PACKET_PARSING}
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    resource.qrc
    qtitlebar.h qtitlebar.cpp qtitlebar.ui
    qpacketform.h qpacketform.cpp qpacketform.ui
    customproxymodel.h customproxymodel.cpp
    qjsonmodel.h qjsonmodel.cpp
    highlighter.h highlighter.cpp
    openthread.h openthread.cpp
    qdetaildialog.h qdetaildialog.cpp qdetaildialog.ui
    outputthread.h outputthread.cpp
    jsonparser.h jsonparser.cpp
    positionfinder.h positionfinder.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(modern-UI
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET modern-UI APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(modern-UI SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(modern-UI
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(modern-UI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(modern-UI PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(modern-UI)
endif()
