# CMakeList.txt : CMake project for TestTinyXML2, include source and define
# project specific logic here.
#
project(ParsingXml)
cmake_minimum_required(VERSION 3.8)
# Add source to this project's executable.
add_executable (${PROJECT_NAME} "TestTinyXML2.cpp" "TestTinyXML2.h")

# TODO: Add tests and install targets if needed.

#file(GLOB srcs "${PROJECT_SOURCE_DIR}/src/*.cpp")
#add_executable(${PROJECT_NAME} ${srcs})
#target_link_libraries(${PROJECT_NAME} OpenTestSystem.Lua.Adapter)
