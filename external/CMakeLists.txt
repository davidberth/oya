cmake_minimum_required (VERSION 3.30)

project(external)

set(GLEW_VERSION 2.2.0)
set(GLEW_URL https://sourceforge.net/projects/glew/files/glew/${GLEW_VERSION}/glew-${GLEW_VERSION}.zip/download)
set(GLEW_ZIP ${CMAKE_BINARY_DIR}/glew.zip)
set(GLEW_SOURCE_DIR ${CMAKE_SOURCE_DIR}/glew-${GLEW_VERSION})

if(NOT EXISTS ${GLEW_ZIP})
    file(DOWNLOAD ${GLEW_URL} ${GLEW_ZIP})
endif()

if(NOT EXISTS ${GLEW_SOURCE_DIR})
    file(ARCHIVE_EXTRACT INPUT ${GLEW_ZIP} DESTINATION ${CMAKE_SOURCE_DIR})
endif()


add_subdirectory(glfw)
add_subdirectory(glm)

add_library(glew STATIC
    ${GLEW_SOURCE_DIR}/src/glew.c
)

target_compile_definitions(glew PUBLIC GLEW_STATIC)

target_include_directories(glew PUBLIC
    ${GLEW_SOURCE_DIR}/include
)


add_library(imgui STATIC
	imgui/imgui.cpp
	imgui/imgui_draw.cpp
	imgui/imgui_widgets.cpp
	imgui/imgui_demo.cpp
	imgui/imgui_tables.cpp
	imgui/backends/imgui_impl_glfw.cpp
	imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC ${CMAKE_SOURCE_DIR}/imgui)

file(REMOVE ${GLEW_ZIP})