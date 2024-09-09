find_package(OpenGL REQUIRED COMPONENTS OpenGL)

add_executable (oya oya.cpp)
target_sources(oya PRIVATE FILE_SET CXX_MODULES FILES )

include_directories(${CMAKE_SOURCE_DIR}/external/glfw/include)
include_directories(${CMAKE_SOURCE_DIR}/external/imgui)
include_directories(${CMAKE_SOURCE_DIR}/external/imgui/backends)
include_directories(${CMAKE_SOURCE_DIR}/external/glew-2.2.0/include)
include_directories(${CMAKE_SOURCE_DIR}/external/spdlog/include)

set(glfw_lib "${CMAKE_SOURCE_DIR}/external/build/glfw/src/${CMAKE_BUILD_TYPE}/glfw3.lib")
set(imgui_lib "${CMAKE_SOURCE_DIR}/external/build/${CMAKE_BUILD_TYPE}/imgui.lib")
set(glew_lib "${CMAKE_SOURCE_DIR}/external/build/${CMAKE_BUILD_TYPE}/glew.lib")
	
target_compile_definitions(oya PUBLIC GLEW_STATIC)
target_link_libraries(oya ${glfw_lib} ${imgui_lib} ${glew_lib} OpenGL32) 
set_property(TARGET oya PROPERTY CXX_STANDARD 23)
set_target_properties(oya PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})