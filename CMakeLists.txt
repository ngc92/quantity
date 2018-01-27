cmake_minimum_required(VERSION 3.9)
project(quantity VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

set(PUBLIC_HEADERS
        include/quantity/dimension.hpp
        include/quantity/quantity.hpp
        include/quantity/predefined.hpp
        include/quantity/vec.hpp
        include/quantity/runtime.hpp
        include/quantity/io.hpp)

set(PRIVATE_HEADERS
        src/runtime_utils.hpp
        src/runtime_ratio.hpp)

set(SOURCES
        src/runtime_utils.cpp
        src/io.cpp
        src/runtime_ratio.cpp)

# The quantity library

add_library(quantity ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES})

target_include_directories(quantity PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

target_compile_features(quantity PUBLIC cxx_std_14)
add_library(quantity::quantity ALIAS quantity)

# and the unit tests
add_executable(unit_tests test/io_tests.cpp test/static.cpp test/runtime_utils_test.cpp test/runtime_ratio_tests.cpp)
target_include_directories(unit_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(unit_tests PRIVATE quantity Boost::unit_test_framework)
