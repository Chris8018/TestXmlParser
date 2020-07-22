project(TestPugiXml)

cmake_minimum_required(VERSION 3.8)

# Add source to this project's executable.
add_executable (${PROJECT_NAME} "TestPugiXml.cpp" "TestPugiXml.h")

include_directories(${CMAKE_SOURCE_DIR}/lib/pugixml/src)

target_link_libraries(${PROJECT_NAME} pugixml)

add_custom_command(
    TARGET ${PROJECT_NAME}
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} ARGS -E copy_directory
    ${CMAKE_SOURCE_DIR}/others/XmlStorage
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/XmlStorage
)
