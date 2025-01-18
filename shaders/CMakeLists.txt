# set the Vulkan SDK path
set(VULKAN_SDK "C:/VulkanSDK/1.4.304.0")

# find all .frag and .vert files in the shaders/source directory
file(GLOB SHADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/source/*.frag" "${CMAKE_CURRENT_SOURCE_DIR}/source/*.vert")

# create a list to hold the output SPIR-V files
set(SPIRV_FILES)

# loop through each shader file and add a custom command to compile it
foreach(SHADER_FILE ${SHADER_FILES})
    get_filename_component(FILE_NAME ${SHADER_FILE} NAME)
    get_filename_component(FILE_BASE_NAME ${SHADER_FILE} NAME_WE)
    get_filename_component(FILE_EXT ${SHADER_FILE} EXT)

    # remove the period from the file extension
    string(REPLACE "." "" FILE_EXT_NO_DOT ${FILE_EXT})

    # set the output SPIR-V file path
    set(SPIRV_FILE "${CMAKE_CURRENT_SOURCE_DIR}/compiled/${FILE_BASE_NAME}_${FILE_EXT_NO_DOT}.spv")

    # add the SPIR-V file to the list
    list(APPEND SPIRV_FILES ${SPIRV_FILE})

    # add a custom command to compile the shader file
    add_custom_command(
        OUTPUT ${SPIRV_FILE}
        COMMAND ${VULKAN_SDK}/Bin/glslc.exe ${SHADER_FILE} -o ${SPIRV_FILE}
        DEPENDS ${SHADER_FILE}
        COMMENT "Compiling ${SHADER_FILE} to ${SPIRV_FILE}"
        VERBATIM
    )
endforeach()

# add a custom target to compile all shaders
add_custom_target(compile_shaders ALL DEPENDS ${SPIRV_FILES})