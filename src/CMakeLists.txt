find_package(OpenGL REQUIRED COMPONENTS OpenGL)

add_executable (oya "main.cpp")
target_sources(oya PRIVATE FILE_SET CXX_MODULES FILES "window.ixx" "layer.ixx" "layer_stack.ixx" "world_layer.ixx" "gui_layer.ixx" "gui_data.ixx" "keyboard_data.ixx" "mouse_data.ixx" "data_trigger.ixx" "fbo.ixx" "viewport_data.ixx" "listener.ixx" "background_layer.ixx" "data_trigger_stack.ixx" "gui_theme.ixx" "window_data.ixx" "render.ixx" "shader.ixx")

include_directories(${CMAKE_BINARY_DIR}/configure)
include_directories(${CMAKE_SOURCE_DIR}/external/glfw/include)
include_directories(${CMAKE_SOURCE_DIR}/external/imgui)
include_directories(${CMAKE_SOURCE_DIR}/external/imgui/backends)
include_directories(${CMAKE_SOURCE_DIR}/external/glew-2.2.0/include)
include_directories(${CMAKE_SOURCE_DIR}/external/loguru)

set(glfw_lib "${CMAKE_SOURCE_DIR}/external/build/glfw/src/${CMAKE_BUILD_TYPE}/glfw3.lib")
set(imgui_lib "${CMAKE_SOURCE_DIR}/external/build/${CMAKE_BUILD_TYPE}/imgui.lib")
set(glew_lib "${CMAKE_SOURCE_DIR}/external/build/${CMAKE_BUILD_TYPE}/glew.lib")
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(logoru_lib "${CMAKE_SOURCE_DIR}/external/build/loguru/Debug/logurud.lib")
else()
	set(logoru_lib "${CMAKE_SOURCE_DIR}/external/build/loguru/Release/loguru.lib")
endif()
	
target_compile_definitions(oya PUBLIC GLEW_STATIC)
target_link_libraries(oya ${glfw_lib} ${imgui_lib} ${glew_lib} ${logoru_lib} OpenGL32) 
set_property(TARGET oya PROPERTY CXX_STANDARD 23)
set_target_properties(oya PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(SHADER_FILES
    ${CMAKE_SOURCE_DIR}/shaders/vertex.glsl
    ${CMAKE_SOURCE_DIR}/shaders/fragment.glsl
)

# Copy shader files to the binary directory
file(COPY ${SHADER_FILES} DESTINATION ${CMAKE_BINARY_DIR}/shaders)