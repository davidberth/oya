add_executable (oya "main.cpp")
target_sources(oya PRIVATE FILE_SET CXX_MODULES FILES  "layer.ixx" "layer_stack.ixx" "world_layer.ixx"    "render.ixx" "camera.ixx" "persistent_data.ixx" "updatable.ixx" "update_manager.ixx" "update.ixx" "node.ixx" "vertex.ixx" "scene.ixx" "event.ixx" "key_event.ixx" "input_event.ixx" "special_actions.ixx" "window_event.ixx" "viewport.ixx"
"mouse_event.ixx" "render_stats_event.ixx" "scene_serializer.ixx" "scene_event.ixx" "polygon.ixx" "polygon_utility.ixx" "renderable.ixx" "outline.ixx" "aabb.ixx" "input_manager.ixx"  "init.ixx" "sdl_data.ixx" "cleanup.ixx" "keyboard.ixx" "mouse.ixx" "render_set.ixx" "shader.ixx" "frame_rate.ixx" "textured_quad.ixx" "font_manager.ixx")

# to turn on address sanitizer, uncomment the following lines
# target_compile_options(oya PRIVATE /fsanitize=address)
# target_link_options(oya PRIVATE /fsanitize=address)
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Od")
target_compile_options(oya PUBLIC /ifcSearchDir${CMAKE_SOURCE_DIR}/build/src/CMakeFiles/oya.dir)

include_directories(${CMAKE_BINARY_DIR}/configure)
include_directories(${CMAKE_SOURCE_DIR}/external/glm)
include_directories(${CMAKE_SOURCE_DIR}/external/sdl3/include)

set(lib_config "${CMAKE_BUILD_TYPE}")
if ("${lib_config}" STREQUAL "RelWithDebInfo")
    set(lib_config "Release")
endif()

set(sdl3_lib "${CMAKE_SOURCE_DIR}/external/build/sdl3/${lib_config}/SDL3-static.lib")
	
target_link_libraries(oya ${sdl3_lib} version.lib Winmm.lib Setupapi.lib Imm32) 
set_property(TARGET oya PROPERTY CXX_STANDARD 23)
set_target_properties(oya PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set_target_properties(oya PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

# define the font atlas output files
set(FONT_ATLAS_BMP ${CMAKE_SOURCE_DIR}/resources/textures/font.bmp)
set(FONT_ATLAS_CSV ${CMAKE_SOURCE_DIR}/resources/font_layouts/font.csv)
set(FONT_SOURCE c:\\windows\\fonts\\bell.ttf)

# build the font atlas
add_custom_command(
    OUTPUT ${FONT_ATLAS_BMP} ${FONT_ATLAS_CSV}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/resources/textures
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/resources/font_layouts
    COMMAND ${CMAKE_SOURCE_DIR}/../atlas_gen/msdf-atlas-gen.exe -font ${FONT_SOURCE} -type msdf -format bmp -chars "[42, 90]" -size 100 -pots -imageout ${FONT_ATLAS_BMP} -csv ${FONT_ATLAS_CSV}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating font atlas"
    VERBATIM
)

add_custom_target(generate_font_atlas ALL DEPENDS ${FONT_ATLAS_PNG} ${FONT_ATLAS_CSV})
add_dependencies(oya generate_font_atlas)
