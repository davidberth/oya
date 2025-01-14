cmake_minimum_required (VERSION 3.30)

project(external)

add_subdirectory(glm)
add_subdirectory(sdl3)

add_library(imgui STATIC
	imgui/imgui.cpp
	imgui/imgui_draw.cpp
	imgui/imgui_widgets.cpp
	imgui/imgui_demo.cpp
	imgui/imgui_tables.cpp
	imgui/backends/imgui_impl_sdl3.cpp
	imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC ${CMAKE_SOURCE_DIR}/imgui ${CMAKE_SOURCE_DIR}/sdl3/include)
