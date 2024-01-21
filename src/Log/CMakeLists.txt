
add_library(Log)
target_sources(Log
	PRIVATE
	PUBLIC
		FILE_SET cxx_modules TYPE CXX_MODULES FILES
		NYANLog.ixx)
	
target_compile_features(Log PUBLIC cxx_std_23)

add_library(NYAN::Log ALIAS Log)