
add_library(Data)
target_sources(Data
	PRIVATE
	PUBLIC
		FILE_SET cxx_modules TYPE CXX_MODULES FILES
		Common.cppm
		DynamicArray.cppm
		DynamicBitset.cppm
		Handle.cppm
		Hash.cppm
		ModuleSettings.cppm
		NYANData.cppm
		StaticBitset.cppm
		StaticVector.cppm
)
	
target_compile_features(Data PUBLIC cxx_std_23)

target_link_libraries(Data PRIVATE NYAN::Assert)

get_target_property(my_dep_runtime Data MSVC_RUNTIME_LIBRARY)
message(STATUS "Runtime library for MyDependency: ${my_dep_runtime}")

add_library(NYAN::Data ALIAS Data)