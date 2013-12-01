project(visionTouchSurface)
cmake_minimum_required(VERSION 2.8)
#aux_source_directory(src/ SRC_LIST)
#aux_source_directory(include/ HEAD_LIST)
include(FindPkgConfig)
SET(${PROJECT_NAME} ${PROJECT_SOURCE_DIR})

set( CMAKE_CXX_FLAGS_RELEASE "-O3 -j4" )

MESSAGE ( STATUS )
MESSAGE ( STATUS "Building visionTouchSurface library:" )
MESSAGE ( STATUS )

#SET (LIB_OUTPUT_NAME ${PROJECT_NAME})
#add_executable(${PROJECT_NAME} ${SRC_LIST})

#set(CMAKE_BUILD_TYPE Release)

#include_directories(/usr/include/ni)
#find_package(OpenCV 2)

#include_directories(${OpenCV_INCLUDE_DIRS})

#include_directories(include)

INCLUDE_DIRECTORIES (
  "vts_gestures/include"
  "vts_ioevents/include"
  "vts_sensors/include"
  "vts_utils/include"
)

FILE (GLOB HEADER_FILES include/*.h include/*.inl)
FILE (GLOB_RECURSE SOURCE_FILES src/*.cpp include/*.h include/*.inl)

#add_library(visionTouchSurfaceLib SHARED src/projectivemapping.cpp src/leapmapping.cpp src/leaphandler.cpp src/kinecthandler.cpp src/kalmanfilteringswipe.cpp)

#add_library(visionTouchSurface SHARED ${SRC_LIST})

#SET (${PROJECT_NAME} ${BUILD_DIR}/${PROJECT_NAME}.so )

ADD_SUBDIRECTORY ( vts_gestures  ${BUILD_DIR})
SET (vts_gestures ${BUILD_DIR}/vts_gestures.so)
ADD_SUBDIRECTORY ( vts_ioevents  ${BUILD_DIR})
SET (vts_ioevents ${BUILD_DIR}/vts_ioevents.so)
ADD_SUBDIRECTORY ( vts_sensors  ${BUILD_DIR})
SET (vts_sensors ${BUILD_DIR}/vts_sensors.so)
ADD_SUBDIRECTORY ( vts_utils  ${BUILD_DIR})
SET (vts_utils ${BUILD_DIR}/vts_utils.so)

