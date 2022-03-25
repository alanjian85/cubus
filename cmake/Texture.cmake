function( add_texture TARGET )
    list( SUBLIST ARGV 1 -1 FILES )
	set( OUTPUT_LIST "" )
    foreach( FILE ${FILES} )
        set( FILE ${CMAKE_CURRENT_SOURCE_DIR}/${FILE} )
        get_filename_component( FILENAME "${FILE}" NAME_WE )
        file( RELATIVE_PATH FILE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/textures "${FILE}" )
        get_filename_component( FILE_DIR "${FILE_PATH}" DIRECTORY)
        set( OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/textures/${FILE_DIR}/${FILENAME}.dds )
        get_filename_component( OUT_DIR ${OUTPUT_FILE} DIRECTORY )
        file( MAKE_DIRECTORY ${OUT_DIR} )
        file( RELATIVE_PATH PRINT_NAME ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${FILE} )
        add_custom_command(
            MAIN_DEPENDENCY
            ${FILE}
			OUTPUT
			${OUTPUT_FILE}
            COMMAND texturec -f ${FILE} -o ${OUTPUT_FILE}
            COMMENT "Compiling texture ${PRINT_NAME}"
        )

        list( APPEND OUTPUT_LIST ${OUTPUT_FILE} )
    endforeach()

	add_custom_target( texture_${TARGET} ALL DEPENDS ${OUTPUT_LIST} )
    add_dependencies( ${TARGET} texture_${TARGET} )
endfunction()