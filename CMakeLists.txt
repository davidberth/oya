cmake_minimum_required (VERSION 3.30)
project (oya VERSION 0.1.1)

if(NOT CMAKE_BUILD_TYPE AND CMAKE_BUILD_TYPE_INIT)
    set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE_INIT})
endif()

set(APPNAME ${CMAKE_PROJECT_NAME})
set(APPVERSION ${CMAKE_PROJECT_VERSION})

add_subdirectory(configure)
add_subdirectory(src)
add_subdirectory(shaders)