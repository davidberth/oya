cmake_minimum_required (VERSION 3.30)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("oya")

find_package(OpenGL REQUIRED COMPONENTS OpenGL)

add_executable (oya "oya.cpp" )

include_directories(${CMAKE_SOURCE_DIR}/external/glfw/include)
include_directories(${CMAKE_SOURCE_DIR}/external/imgui)
include_directories(${CMAKE_SOURCE_DIR}/external/imgui/backends)
include_directories(${CMAKE_SOURCE_DIR}/external/glew-2.2.0/include)

set(glfw_lib "${CMAKE_SOURCE_DIR}/external/build/glfw/src/${CMAKE_BUILD_TYPE}/glfw3.lib")
set(imgui_lib "${CMAKE_SOURCE_DIR}/external/build/${CMAKE_BUILD_TYPE}/imgui.lib")

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
	set(glew_lib "${CMAKE_SOURCE_DIR}/external/build/lib/release/glew32.lib")
else()
	set(glew_lib "${CMAKE_SOURCE_DIR}/external/build/lib/debug/glew32d.lib")
endif()
target_link_libraries(oya ${glfw_lib} ${imgui_lib} ${glew_lib} OpenGL32) 


set_property(TARGET oya PROPERTY CXX_STANDARD 23)
