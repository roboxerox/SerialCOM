cmake_minimum_required(VERSION 3.1.0)

project(SerialCOM
  VERSION
    1.0
  DESCRIPTION
    "Serial Port Communication"
  LANGUAGES
    CXX
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

find_package(Qt5Core REQUIRED)

set(SOURCES WidgetSerialPort.ui WidgetSerialPort.cpp WidgetSerialPort.h 
		SerialCOM.ui SerialCOM.cpp SerialCOM.h main.cpp icons.qrc)

add_executable(SerialCOM ${SOURCES})

add_definitions(-DQT_NO_VERSION_TAGGING)

target_link_libraries(SerialCOM Qt5::Widgets)

qt5_use_modules(SerialCOM Core SerialPort)

include_directories(. .)
