cmake_minimum_required(VERSION 2.8.11)

# Create new project
project (libFingerPrint)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

# Create variable containing all the CXX sources
set(SOURCES
   ../src/main_1.cpp
   ../src/starter_1.cpp
   ../src/starter_2.cpp
   ../src/main_2.cpp
   ../src/starter_3.cpp
   ../src/main_3.cpp
   ../src/starter_4.cpp
   ../src/main_4.cpp
   ../src/starter_5.cpp
   ../src/main_5.cpp
)


# Add all the sources to the project
add_library(${PROJECT_NAME} ${SOURCES})
