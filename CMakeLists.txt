cmake_minimum_required(VERSION 3.5.1)

project(MRA LANGUAGES CXX)

enable_testing()

# system dependencies
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_LIST_DIR}/cmake)

# components
add_subdirectory(datatypes)
#add_subdirectory(libraries)
#add_subdirectory(components)

