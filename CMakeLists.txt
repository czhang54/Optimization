cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("Optimization")

# Require Eigen
#find_package( Eigen REQUIRED ) # find_package() is NOT needed since Eigen is a header-only library
include_directories(/usr/local/Cellar/eigen/3.3.1/include) # Replace with actual include path of Eigen

set (CMAKE_CXX_STANDARD 11) # Choose C++ version to compile

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(include) # Optional, maybe because this include directory is added in src/CMakeLists.txt

add_subdirectory(src)

add_executable(optimization example/example.cpp)

target_link_libraries(optimization optimization_source)

#enable_testing()

#add_test(optimization_test optimization)


set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "A flag showing that CMake has configured at least once.")
