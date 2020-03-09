cmake_minimum_required (VERSION 2.8.11)

# project(exe)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
# include_directories(${SOURCES})

set(DEMO
    rectangle_demo
    sym_demo
    rotation_demo
    isotropic_demo
    anisotropic_demo
    convolution_demo
    opti_demo
    morphological_demo
)


# Create variable containing all the CXX sources
set(SOURCES_2
   ../src/main_1.cpp
   ../src/starter_1.cpp
   ../src/main_2.cpp
   ../src/starter_2.cpp
   ../src/main_3.cpp
   ../src/starter_3.cpp
   ../src/starter_4.cpp
   ../src/main_4.cpp
   ../src/starter_5.cpp
   ../src/main_5.cpp
)

foreach(demo IN ITEMS ${DEMO})
  add_executable(${demo} ${demo}.cpp ${SOURCES_2})
  target_include_directories(${demo} PUBLIC src)
  target_link_libraries(${demo} ${OpenCV_LIBS})
endforeach()


# target_link_libraries(exe
#     PUBLIC
#     libFingerPrint
# )

# add_executable(rectangle_demo ../demo/rectangle_demo.cpp ../src/main_1.cpp)
#
# target_include_directories(rectangle_demo PUBLIC src)
#
#
# target_link_libraries(rectangle_demo ${OpenCV_LIBS})
