# CMakeList.txt : CMake project for ParsingXml, include source and define
# project specific logic here.
#
project(ParsingXml)
cmake_minimum_required(VERSION 3.8)
# Add source to this project's executable.
add_executable (${PROJECT_NAME} "ParsingXml.cpp" "ParsingXml.h")

# Set include paths
include_directories(
    ${CMAKE_SOURCE_DIR}/lib/tinyxml2/
)	

target_link_libraries(${PROJECT_NAME}  tinyxml2)
