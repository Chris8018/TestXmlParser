# CMakeList.txt : CMake project for TestTinyXML2, include source and define
# project specific logic here.
#
project(TestTinyXML2)
cmake_minimum_required(VERSION 3.8)
# Add source to this project's executable.
add_executable (${PROJECT_NAME} "TestTinyXML2.cpp" "TestTinyXML2.h")

# Set include paths
include_directories(
    ${CMAKE_SOURCE_DIR}/lib/tinyxml2/
)

target_link_libraries(${PROJECT_NAME}  tinyxml2)
