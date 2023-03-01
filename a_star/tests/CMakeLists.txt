cmake_minimum_required(VERSION 3.24)
project(cmake-example-tests C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(GTest REQUIRED)

add_executable(test_main test_main.cc)

# Targets
foreach(_target
  test_main)
  target_link_libraries(${_target}
    ${GTEST_LIBRARIES})
endforeach()

