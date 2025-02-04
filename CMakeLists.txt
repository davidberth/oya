cmake_minimum_required (VERSION 3.30)
project (oya VERSION 0.1.1)

# enable multicore compilation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")

if(NOT CMAKE_BUILD_TYPE AND CMAKE_BUILD_TYPE_INIT)
    set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE_INIT})
endif()

set(APPNAME ${CMAKE_PROJECT_NAME})
set(APPVERSION ${CMAKE_PROJECT_VERSION})

add_subdirectory(configure)
add_subdirectory(src)
add_subdirectory(shaders)