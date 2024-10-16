
add_library(Assert)
target_sources(Assert
	PRIVATE
	PUBLIC
		FILE_SET cxx_modules TYPE CXX_MODULES FILES
		Assert.cppm
		NYANAssert.cppm)
	
target_compile_features(Assert PUBLIC cxx_std_23)

target_compile_definitions(Assert PRIVATE ASSERTION_LEVEL=2)
target_compile_definitions(Assert PRIVATE ASSERTION_EXIT_MODE=3)
target_compile_definitions(Assert PRIVATE ASSERTION_LOG_MODE=3)
target_compile_definitions(Assert PRIVATE ASSERTION_DEBUG_MODE=1)

target_link_libraries(Assert PRIVATE NYAN::Log)
target_link_libraries(Assert PUBLIC std)

add_library(NYAN::Assert ALIAS Assert)