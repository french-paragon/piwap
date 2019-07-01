cmake_minimum_required(VERSION 2.8.9)
project(piwap)

#Qt

find_package(Qt5Core REQUIRED)
find_package(Qt5QuickWidgets REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Svg REQUIRED)

ADD_DEFINITIONS(-DQT_NO_KEYWORDS)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

#OpenCV

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

#libaries

set(BUILD_SHARED_LIBS ON)

#build
set(PROG_NAME ${CMAKE_PROJECT_NAME})
set(PROG_SRC main.cpp
	         image/imageinfos.cpp
			 image/imagetotreatmanager.cpp
			 image/imageio.cpp
	         operations/abstractimageoperation.cpp
			 operations/abstractoperationfactory.cpp
			 operations/operationfactorymanager.cpp
			 operations/operationlistmanager.cpp
			 operations/base_operations/resize.cpp
			 operations/base_operations/save.cpp
			 application/application.cpp
			 icons.qrc
			 qml.qrc)

add_executable(${PROG_NAME} ${PROG_SRC})

target_link_libraries(${PROG_NAME} Qt5::Core)
target_link_libraries(${PROG_NAME} Qt5::QuickWidgets)
target_link_libraries(${PROG_NAME} Qt5::Qml)
target_link_libraries(${PROG_NAME} Qt5::Svg)

target_link_libraries(${PROG_NAME} ${OpenCV_LIBS} )
