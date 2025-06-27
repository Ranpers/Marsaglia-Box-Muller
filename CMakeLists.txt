cmake_minimum_required(VERSION 3.10)
project(box_muller_lib VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 动态库 (.so)
add_library(box_muller_shared SHARED src/box_muller.cpp)
target_include_directories(box_muller_shared
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
install(TARGETS box_muller_shared
    EXPORT box_muller-targets
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)

# 静态库 (.a)
add_library(box_muller_static STATIC src/box_muller.cpp)
target_include_directories(box_muller_static
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
install(TARGETS box_muller_static
    EXPORT box_muller-targets
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)

# 统一导出（供find_package使用）
install(EXPORT box_muller-targets
    FILE box_muller-config.cmake
    NAMESPACE box_muller::
    DESTINATION lib/cmake/box_muller
)