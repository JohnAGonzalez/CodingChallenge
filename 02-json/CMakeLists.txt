
cmake_minimum_required(VERSION 3.20)

project(JsonParser
    VERSION 1.0.0
    LANGUAGES CXX
)

# Keep the project compatible with common C++ toolchains.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(json
    src/json.cpp
    src/JsonParser.cpp
    src/InputProcessor.cpp
    src/CommandLine.cpp
)

target_include_directories(json
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/inc"
)

# Compiler-specific warning options.
if(MSVC)
    target_compile_options(json PRIVATE /W4)
else()
    target_compile_options(json PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Keep the executable inside the build directory.
set_target_properties(json PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)


# ------------------------------------------------------------
# Regression testing
# ------------------------------------------------------------

include(CTest)

if(BUILD_TESTING)
    find_package(Python3 REQUIRED COMPONENTS Interpreter)

    add_test(
        NAME json_numbers
        COMMAND
            "${CMAKE_COMMAND}" -E env
            "JSON_PARSER_EXECUTABLE=$<TARGET_FILE:json>"
            "${Python3_EXECUTABLE}"
            "${CMAKE_CURRENT_SOURCE_DIR}/tests/regression/test_numbers.py"
    )

    add_test(
        NAME json_strings
        COMMAND
            "${CMAKE_COMMAND}" -E env
            "JSON_PARSER_EXECUTABLE=$<TARGET_FILE:json>"
            "${Python3_EXECUTABLE}"
            "${CMAKE_CURRENT_SOURCE_DIR}/tests/regression/test_strings.py"
    )
endif()

