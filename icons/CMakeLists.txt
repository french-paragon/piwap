configure_file(${CMAKE_CURRENT_SOURCE_DIR}/logo.svg ${CMAKE_CURRENT_BINARY_DIR}/piwap.svg COPYONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/piwap.svg DESTINATION share/icons/default/scalable)

find_program (SVGCONVERT rsvg-convert)

if (SVGCONVERT)

        execute_process(
		COMMAND mkdir -p 32
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/)

	execute_process(
		COMMAND mkdir -p 64
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/)

	execute_process(
		COMMAND mkdir -p 128
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/)

	execute_process(
		COMMAND mkdir -p 256
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/)

        if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/32/piwap.png)
            exec_program(
                ${SVGCONVERT} ${CMAKE_CURRENT_BINARY_DIR}
                ARGS " -w 32 -h 32 piwap.svg -o 32/piwap.png")
	endif()

        if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/64/piwap.png)
            exec_program(
                ${SVGCONVERT} ${CMAKE_CURRENT_BINARY_DIR}
                ARGS " -w 64 -h 64 piwap.svg -o 64/piwap.png")
	endif()

        if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/128/piwap.png)
            exec_program(
                ${SVGCONVERT} ${CMAKE_CURRENT_BINARY_DIR}
                ARGS " -w 128 -h 128 piwap.svg -o 128/piwap.png")
	endif()

        if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/256/piwap.png)
            exec_program(
                ${SVGCONVERT} ${CMAKE_CURRENT_BINARY_DIR}
                ARGS " -w 256 -h 256 piwap.svg -o 256/piwap.png")
	endif()

        install (FILES ${CMAKE_CURRENT_BINARY_DIR}/32/piwap.png DESTINATION share/icons/default/32x32)
        install (FILES ${CMAKE_CURRENT_BINARY_DIR}/64/piwap.png DESTINATION share/icons/default/64x64)
        install (FILES ${CMAKE_CURRENT_BINARY_DIR}/128/piwap.png DESTINATION share/icons/default/128x128)
        install (FILES ${CMAKE_CURRENT_BINARY_DIR}/256/piwap.png DESTINATION share/icons/default/256x256)

endif()
