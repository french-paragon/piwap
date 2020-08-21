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

#find_package( OpenCV REQUIRED )
#include_directories( ${OpenCV_INCLUDE_DIRS} )

#ImageMagick

find_package(ImageMagick COMPONENTS Magick++ MagickCore)
include_directories( ${ImageMagick_INCLUDE_DIRS} )

add_definitions(-DMAGICKCORE_QUANTUM_DEPTH=16)
add_definitions(-DMAGICKCORE_HDRI_ENABLE=0)

find_library(LZIP zip)

if(NOT LZIP)
  message(FATAL_ERROR "libzip library not found")
endif()


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
                         operations/base_operations/abstractinterpolatingoperation.cpp
                         operations/base_operations/fit.cpp
						 operations/base_operations/reload.cpp
						 operations/base_operations/checkpoint.cpp
						 operations/base_operations/backgroundcolor.cpp
						 operations/operationerror.cpp
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
target_link_libraries(${PROG_NAME} ${LZIP})

#target_link_libraries(${PROG_NAME} ${OpenCV_LIBS} )
target_link_libraries(${PROG_NAME} ${ImageMagick_LIBRARIES} )

#icons processing
add_subdirectory(icons)

#install
install (TARGETS ${PROG_NAME} DESTINATION usr/bin)
install (FILES desktops/Piwap.desktop DESTINATION usr/share/applications)
