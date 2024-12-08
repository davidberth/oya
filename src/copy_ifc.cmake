file(GLOB IFC_FILES "${SOURCE_DIR}/*.ifc")
foreach(IFC_FILE ${IFC_FILES})
    get_filename_component(FILENAME ${IFC_FILE} NAME)
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${IFC_FILE}
            "${DEST_DIR}/${FILENAME}"
    )
endforeach()