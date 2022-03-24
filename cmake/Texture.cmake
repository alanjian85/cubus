function( add_texture TARGET )
    list( SUBLIST ARGV 1 -1 FILES )
	set( OUTPUT_LIST "" )
    foreach( FILE ${FILES} )
        get_filename_component( FILENAME "${FILE}" NAME_WE )
        set( FILE ${CMAKE_CURRENT_SOURCE_DIR}/${FILE} )
        set( OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/textures/${FILENAME}.dds )
        get_filename_component( OUT_DIR ${OUTPUT_FILE} DIRECTORY )
        file( MAKE_DIRECTORY ${OUT_DIR} )
        file( RELATIVE_PATH PRINT_NAME ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${FILE} )
        add_custom_command(
            MAIN_DEPENDENCY
            ${FILE}
			OUTPUT
			${OUTPUT_FILE}
            COMMAND texturec -f ${FILE} -o ${OUTPUT_FILE} -m
            COMMENT "Compiling texture ${PRINT_NAME}"
        )

        list( APPEND OUTPUT_LIST ${OUTPUT_FILE} )
    endforeach()

	add_custom_target( texture_${TARGET} ALL DEPENDS ${OUTPUT_LIST} )
    add_dependencies( ${TARGET} texture_${TARGET} )
endfunction()