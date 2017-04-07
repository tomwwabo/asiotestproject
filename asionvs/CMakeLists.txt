
cmake_minimum_required(VERSION 3.0)

project(shalom)  # change project name and version!!!

set(appname ${PROJECT_NAME})

add_definitions(-std=c++11)

include_directories(include)  # if you need it
link_directories(libs)  # if you need it

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(warnings "-Wall -Wextra -Wpedantic -Wconversion -Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(warnings "/W4 /WX /EHsc")
endif()

#set(CMAKE_CXX_FLAGS "${warnings}")

# setting so that operating from directory `build` works out of the way
set(CMAKE_INSTALL_PREFIX ".")

add_subdirectory(server)  # main module
add_subdirectory(client)  # main module

# add further modules here (if needed)!!!:
# add_subdirectory(counter)  # an example of a static library
