# # Add this line to the CMakeLists.txt containing tests
# find_package(OpenCV REQUIRED)
#
# include_directories(${OpenCV_INCLUDE_DIRS})
#
# add_executable(test_finger test_finger.cpp)
#
# target_include_directories(test_finger PUBLIC src tests)
#
# target_link_libraries(test_finger gtest ${OpenCV_LIBS} libFingerPrint)
#
# add_test (NAME finger_test
#     COMMAND test_finger
#     )
