
set(PLUGIN_NAME "BorderOperations")
set(PLUGIN_SRC plugin.cpp
	            operations/bevels.cpp
                    operations/dropshadow.cpp
				borderopicons.qrc
				borderopqml.qrc)

add_library(${PLUGIN_NAME} MODULE ${PLUGIN_SRC})

target_link_libraries(${PLUGIN_NAME} Qt5::Core)
target_link_libraries(${PLUGIN_NAME} Qt5::Gui)
target_link_libraries(${PLUGIN_NAME} ${ImageMagick_LIBRARIES} )

install (TARGETS ${PLUGIN_NAME} DESTINATION usr/lib/piwap/modules)
