project(TestPocoXml)
cmake_minimum_required(VERSION 3.8)
# Add source to this project's executable.
add_executable (${PROJECT_NAME} "TestPocoXml.cpp" "TestPocoXml.h")

# Set include paths
include_directories(
    ${CMAKE_SOURCE_DIR}/lib/poco-poco-1.9.4/XML/include
)

target_link_libraries(${PROJECT_NAME} XML)
