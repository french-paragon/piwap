cmake_minimum_required(VERSION 3.0)
project(piwap)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_SHARED_MODULE_PREFIX "")

set(CMAKE_EXECUTE_PROCESS_COMMAND_ECHO STDOUT)

#Qt

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5QuickWidgets REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Svg REQUIRED)

ADD_DEFINITIONS(-DQT_NO_KEYWORDS)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

#Images libraries

find_package(JPEG REQUIRED)
find_package(PNG REQUIRED)
find_package(TIFF REQUIRED)

find_package(ImageMagick REQUIRED COMPONENTS Magick++ MagickCore)

include_directories( ${ImageMagick_INCLUDE_DIRS} )

#LZip

find_library(LZIP zip)

if(NOT LZIP)
  message(FATAL_ERROR "libzip library not found")
endif()

#Exiv2

find_package(exiv2 REQUIRED CONFIG NAMES exiv2)

#libaries

set(BUILD_SHARED_LIBS ON)

#build
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(PROG_NAME ${CMAKE_PROJECT_NAME})
set(PROG_SRC main.cpp
	         image/image.cpp
			 image/imageinfos.cpp
			 image/imagetotreatmanager.cpp
			 image/imageio.cpp
	         operations/abstractimageoperation.cpp
			 operations/abstractoperationfactory.cpp
			 operations/operationfactorymanager.cpp
			 operations/operationlistmanager.cpp
			 operations/base_operations/abstractinterpolatingoperation.cpp
			 operations/base_operations/fit.cpp
			 operations/base_operations/crop.cpp
			 operations/base_operations/reload.cpp
			 operations/base_operations/checkpoint.cpp
			 operations/base_operations/backgroundcolor.cpp
			 operations/operationerror.cpp
			 operations/helper_functions/alphachannelsutils.cpp
			 operations/base_operations/resize.cpp
			 operations/base_operations/save.cpp
			 application/application.cpp
			 application/operationplugin.cpp
			 icons.qrc
			 qml.qrc)

add_executable(${PROG_NAME} ${PROG_SRC})

target_link_libraries(${PROG_NAME} Qt5::Core)
target_link_libraries(${PROG_NAME} Qt5::QuickWidgets)
target_link_libraries(${PROG_NAME} Qt5::Qml)
target_link_libraries(${PROG_NAME} Qt5::Svg)
target_link_libraries(${PROG_NAME} ${LZIP})
target_link_libraries(${PROG_NAME} JPEG::JPEG)
target_link_libraries(${PROG_NAME} PNG::PNG)
target_link_libraries(${PROG_NAME} TIFF::TIFF)
target_link_libraries(${PROG_NAME} exiv2)

#target_link_libraries(${PROG_NAME} ${OpenCV_LIBS} )
target_link_libraries(${PROG_NAME} ${ImageMagick_LIBRARIES} )

#icons processing
add_subdirectory(icons)
add_subdirectory(modules)

#install
install (TARGETS ${PROG_NAME} DESTINATION bin)
install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/application/operationplugin.h DESTINATION  include/piwap)
install (FILES desktops/Piwap.desktop DESTINATION share/applications)
