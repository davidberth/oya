find_package(OpenGL REQUIRED COMPONENTS OpenGL)

add_executable (oya "main.cpp" "update.ixx")
target_sources(oya PRIVATE FILE_SET CXX_MODULES FILES "window.ixx" "layer.ixx" "layer_stack.ixx" "world_layer.ixx" "gui_layer.ixx" "fbo.ixx" "gui_theme.ixx" "render.ixx" "shader.ixx" "camera.ixx" "persistent_data.ixx" "updatable.ixx" "update_manager.ixx" "update.ixx" "node.ixx" "renderable_buffer.ixx" "vertex.ixx" "scene.ixx"    "event.ixx" "key_event.ixx" "input_event.ixx" "input_manager.ixx" "special_actions.ixx" "window_event.ixx" "viewport.ixx"
"mouse_event.ixx" "render_stats_event.ixx" "scene_serializer.ixx" "scene_event.ixx" "polygon.ixx" "polygon_utility.ixx" "renderable.ixx")

target_compile_options(oya PUBLIC /ifcSearchDir${CMAKE_SOURCE_DIR}/build/src/CMakeFiles/oya.dir)

include_directories(${CMAKE_BINARY_DIR}/configure)
include_directories(${CMAKE_SOURCE_DIR}/external/glfw/include)
include_directories(${CMAKE_SOURCE_DIR}/external/glm)
include_directories(${CMAKE_SOURCE_DIR}/external/imgui)
include_directories(${CMAKE_SOURCE_DIR}/external/imgui/backends)
include_directories(${CMAKE_SOURCE_DIR}/external/glew-2.2.0/include)
include_directories(${CMAKE_SOURCE_DIR}/external/loguru)
include_directories(${CMAKE_SOURCE_DIR}/external/cereal/include)

set(glfw_lib "${CMAKE_SOURCE_DIR}/external/build/glfw/src/${CMAKE_BUILD_TYPE}/glfw3.lib")
set(imgui_lib "${CMAKE_SOURCE_DIR}/external/build/${CMAKE_BUILD_TYPE}/imgui.lib")
set(glew_lib "${CMAKE_SOURCE_DIR}/external/build/${CMAKE_BUILD_TYPE}/glew.lib")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(logoru_lib "${CMAKE_SOURCE_DIR}/external/build/loguru/Debug/logurud.lib")
else()
    set(logoru_lib "${CMAKE_SOURCE_DIR}/external/build/loguru/Release/loguru.lib")
endif()
	
target_compile_definitions(oya PUBLIC GLEW_STATIC)
target_link_libraries(oya ${glfw_lib} ${imgui_lib} ${glew_lib} ${logoru_lib} OpenGL32) 
set_property(TARGET oya PROPERTY CXX_STANDARD 23)
set_target_properties(oya PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set_target_properties(oya PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
