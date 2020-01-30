project(XercesSingleton)
cmake_minimum_required(VERSION 3.8)
# Add source to this project's executable.
add_executable (${PROJECT_NAME} "Entry.cpp" "Entry.h")

# Set include paths
include_directories(
    #On Windows, xerces config header is saved in cmake cache
    #This line is required to include the config
    ${CMAKE_BINARY_DIR}/lib/xerces-c-3.2.2/src
    ${CMAKE_SOURCE_DIR}/lib/xerces-c-3.2.2/src
)

target_link_libraries(${PROJECT_NAME} xerces-c)

#if (EXISTS "${CMAKE_BINARY_DIR}/lib/xerces-c-3.2.2/src/xercesc/util/Xerces_autoconf_config.hpp")
#    message("Config File Exist")
#endif()
