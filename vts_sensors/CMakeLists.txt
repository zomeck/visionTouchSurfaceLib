SET (LIB_OUTPUT_NAME vts_sensors)

# Print variables to stdout
MESSAGE ( STATUS )
MESSAGE ( STATUS "Configuring ${LIB_OUTPUT_NAME}:" )
MESSAGE ( STATUS )

# Source code, headers and test files
if(USE_LEAP_SUPPORT)
message (STATUS "Leap support enabled")
FILE (GLOB_RECURSE HEADER_FILES include/*.h include/*.inl)
FILE (GLOB_RECURSE SOURCE_FILES src/*.cpp include/*.h include/*.inl)
endif( USE_LEAP_SUPPORT )

if(NOT USE_LEAP_SUPPORT)
message (STATUS "Leap support disabled")
FILE (GLOB_RECURSE HEADER_FILES include/kinecthandler.h projectivemapping.h)
FILE (GLOB_RECURSE SOURCE_FILES src/kinecthandler.cpp src/projectivemapping.cpp include/*.h include/*.inl)
endif(NOT USE_LEAP_SUPPORT)

find_package(OpenCV 2)

include_directories(${OpenCV_INCLUDE_DIRS})

#include_directories(vts_gestures/include)

include_directories(/usr/include/ni/)

# Build library
ADD_LIBRARY (${LIB_OUTPUT_NAME} SHARED ${SOURCE_FILES}) 
