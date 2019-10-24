find_program (INKSCAPEEXE inkscape)

if (INKSCAPEEXE)

	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/logo.svg ${CMAKE_CURRENT_BINARY_DIR}/piwap.svg COPYONLY)

	exec_program(
		"mkdir"
		${CMAKE_CURRENT_BINARY_DIR}/
		ARGS "-p 32")

	exec_program(
		"mkdir"
		${CMAKE_CURRENT_BINARY_DIR}/
		ARGS "-p 64")

	exec_program(
		"mkdir"
		${CMAKE_CURRENT_BINARY_DIR}/
		ARGS "-p 128")

	exec_program(
		"mkdir"
		${CMAKE_CURRENT_BINARY_DIR}/
		ARGS "-p 256")

	if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/32/piwap.png)
		exec_program(
			${INKSCAPEEXE}
			${CMAKE_CURRENT_BINARY_DIR}/
			ARGS "piwap.svg --export-png=32/piwap.png --export-width=32 --export-height=32 --export-background=\"rgb(0%,0%,0%)\" --export-background-opacity=\"0\"")
	endif()

	if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/64/piwap.png)
		exec_program(
			${INKSCAPEEXE}
			${CMAKE_CURRENT_BINARY_DIR}/
			ARGS "piwap.svg --export-png=64/piwap.png --export-width=64 --export-height=64 --export-background=\"rgb(0%,0%,0%)\" --export-background-opacity=\"0\"")
	endif()

	if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/128/piwap.png)
		exec_program(
			${INKSCAPEEXE}
			${CMAKE_CURRENT_BINARY_DIR}/
			ARGS "piwap.svg --export-png=128/piwap.png --export-width=128 --export-height=128 --export-background=\"rgb(0%,0%,0%)\" --export-background-opacity=\"0\"")
	endif()

	if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/256/piwap.png)
		exec_program(
			${INKSCAPEEXE}
			${CMAKE_CURRENT_BINARY_DIR}/
			ARGS "piwap.svg --export-png=256/piwap.png --export-width=256 --export-height=256 --export-background=\"rgb(0%,0%,0%)\" --export-background-opacity=\"0\"")
	endif()

	install (FILES ${CMAKE_CURRENT_BINARY_DIR}/32/piwap.png DESTINATION usr/share/icons/default/32x32)
	install (FILES ${CMAKE_CURRENT_BINARY_DIR}/64/piwap.png DESTINATION usr/share/icons/default/64x64)
	install (FILES ${CMAKE_CURRENT_BINARY_DIR}/128/piwap.png DESTINATION usr/share/icons/default/128x128)
	install (FILES ${CMAKE_CURRENT_BINARY_DIR}/256/piwap.png DESTINATION usr/share/icons/default/256x256)
	install (FILES ${CMAKE_CURRENT_BINARY_DIR}/piwap.svg DESTINATION usr/share/icons)

endif()
