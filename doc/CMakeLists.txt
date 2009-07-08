find_package(Doxygen)

if(DOXYGEN_FOUND)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
	add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
	install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION doc)
endif(DOXYGEN_FOUND)

