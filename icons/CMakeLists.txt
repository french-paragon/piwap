find_program (INKSCAPEEXE inkscape)

if (INKSCAPEEXE)

	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/logo.svg ${CMAKE_CURRENT_BINARY_DIR}/piwap.svg COPYONLY)

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
		execute_process(
			COMMAND ${INKSCAPEEXE} "piwap.svg" "--export-png=32/piwap.png" "--export-width=32" "--export-height=32" "--export-background=\"rgb(0%,0%,0%)\"" "--export-background-opacity=\"0\""
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/)
	endif()

	if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/64/piwap.png)
		execute_process(
			COMMAND ${INKSCAPEEXE} "piwap.svg" "--export-png=64/piwap.png" "--export-width=64" "--export-height=64" "--export-background=\"rgb(0%,0%,0%)\"" "--export-background-opacity=\"0\""
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/)
	endif()

	if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/128/piwap.png)
		execute_process(
			COMMAND ${INKSCAPEEXE} "piwap.svg" "--export-png=128/piwap.png" "--export-width=128" "--export-height=128" "--export-background=\"rgb(0%,0%,0%)\"" "--export-background-opacity=\"0\""
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/)
	endif()

	if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/256/piwap.png)
		execute_process(
			COMMAND ${INKSCAPEEXE} "piwap.svg" "--export-png=256/piwap.png" "--export-width=256" "--export-height=256" "--export-background=\"rgb(0%,0%,0%)\"" "--export-background-opacity=\"0\""
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/)
	endif()

	install (FILES ${CMAKE_CURRENT_BINARY_DIR}/32/piwap.png DESTINATION usr/share/icons/default/32x32)
	install (FILES ${CMAKE_CURRENT_BINARY_DIR}/64/piwap.png DESTINATION usr/share/icons/default/64x64)
	install (FILES ${CMAKE_CURRENT_BINARY_DIR}/128/piwap.png DESTINATION usr/share/icons/default/128x128)
	install (FILES ${CMAKE_CURRENT_BINARY_DIR}/256/piwap.png DESTINATION usr/share/icons/default/256x256)
	install (FILES ${CMAKE_CURRENT_BINARY_DIR}/piwap.svg DESTINATION usr/share/icons)

endif()
