########################################################################################################################
### MAGIC SQUARE APPLICATION
########################################################################################################################

cmake_minimum_required(VERSION 3.23)

########################################################################################################################
### PROJECT DESCRIPTION AND META INFO
########################################################################################################################

set(MAGIC_SQUARE_PROJECT_NAME           "MagicSquare")
set(MAGIC_SQUARE_PROJECT_DESCRIPTION    "Tool to create solutions for magic squares of size N")

########################################################################################################################
### DECLARE PROJECT
########################################################################################################################

project(${MAGIC_SQUARE_PROJECT_NAME})

set(MAGIC_SQUARE_SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(MAGIC_SQUARE_INC_DIR ${PROJECT_SOURCE_DIR}/include)
set(MAGIC_SQUARE_TEST_DIR ${PROJECT_SOURCE_DIR}/tests)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)
add_subdirectory(tests)
