
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
		NYANData.cppm
		StaticBitset.cppm
		StaticVector.cppm
)
	
target_compile_features(Data PUBLIC cxx_std_23)

target_link_libraries(Data PRIVATE NYAN::Assert)

add_library(NYAN::Data ALIAS Data)