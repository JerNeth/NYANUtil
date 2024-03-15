
add_library(Coro)
target_sources(Coro
	PRIVATE
	PUBLIC
		FILE_SET cxx_modules TYPE CXX_MODULES FILES
		NYANCoro.ixx)
	
target_compile_features(Coro PUBLIC cxx_std_23)

add_library(NYAN::Coro ALIAS Coro)