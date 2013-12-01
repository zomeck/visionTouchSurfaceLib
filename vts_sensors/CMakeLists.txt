SET (LIB_OUTPUT_NAME vts_sensors)

# Print variables to stdout
MESSAGE ( STATUS )
MESSAGE ( STATUS "Configuring ${LIB_OUTPUT_NAME}:" )
MESSAGE ( STATUS )

# Source code, headers and test files
FILE (GLOB_RECURSE HEADER_FILES include/*.h include/*.inl)
FILE (GLOB_RECURSE SOURCE_FILES src/*.cpp include/*.h include/*.inl)


find_package(OpenCV 2)

include_directories(${OpenCV_INCLUDE_DIRS})

#include_directories(vts_gestures/include)

include_directories(/usr/include/ni)

# Build library
ADD_LIBRARY (${LIB_OUTPUT_NAME} SHARED ${SOURCE_FILES}) 
