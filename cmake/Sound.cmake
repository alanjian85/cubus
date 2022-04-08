function( add_sound TARGET )
    list( SUBLIST ARGV 1 -1 FILES )
    set( OUTPUT_LIST "" )
    foreach( FILE ${FILES} )
        set( FILE ${CMAKE_CURRENT_SOURCE_DIR}/${FILE} )
        get_filename_component( FILENAME "${FILE}" NAME )
        file( RELATIVE_PATH FILE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/sounds "${FILE}" )
        get_filename_component( FILE_DIR "${FILE_PATH}" DIRECTORY)
        set( OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/assets/sounds/${FILE_DIR}/${FILENAME} )
        get_filename_component( OUT_DIR "${OUTPUT_FILE}" DIRECTORY )
        file( MAKE_DIRECTORY ${OUT_DIR} )
        file( RELATIVE_PATH PRINT_NAME ${CMAKE_CURRENT_SOURCE_DIR} ${FILE} )
        add_custom_command(
            MAIN_DEPENDENCY
            ${FILE}
			OUTPUT
			${OUTPUT_FILE}
            COMMAND ${CMAKE_COMMAND} -E copy ${FILE} ${OUTPUT_FILE}
            COMMENT "Copying sound file ${PRINT_NAME}"
        )

        list( APPEND OUTPUT_LIST ${OUTPUT_FILE} )
    endforeach()

    add_custom_target( sound_${TARGET} ALL DEPENDS ${OUTPUT_LIST} )
    add_dependencies( cephalon sound_cephalon )
endfunction()