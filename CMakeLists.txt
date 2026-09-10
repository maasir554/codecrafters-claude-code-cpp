cmake_minimum_required(VERSION 3.13)

project(codecrafters-claude-code)

set(CMAKE_CXX_STANDARD 23) # Enable the C++23 standard

# Embed the tool schemas so runtime paths do not depend on the working directory.
set(TOOL_DEFINITIONS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/tool-definitions")
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
    "${TOOL_DEFINITIONS_DIR}/read-tool.json"
    "${TOOL_DEFINITIONS_DIR}/write-tool.json")
file(READ "${TOOL_DEFINITIONS_DIR}/read-tool.json" READ_TOOL_JSON)
file(READ "${TOOL_DEFINITIONS_DIR}/write-tool.json" WRITE_TOOL_JSON)
configure_file(cmake/tool-definitions.hpp.in
    "${CMAKE_CURRENT_BINARY_DIR}/generated/tool-definitions.hpp" @ONLY)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.hpp)

find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(claude-code ${SOURCE_FILES})
target_include_directories(claude-code PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/generated")

target_link_libraries(claude-code PRIVATE cpr::cpr nlohmann_json::nlohmann_json)
