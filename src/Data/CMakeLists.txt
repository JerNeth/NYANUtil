
add_library(Data)
target_sources(Data
	PRIVATE
	PUBLIC
		FILE_SET cxx_modules TYPE CXX_MODULES FILES
		DynamicArray.ixx
		DynamicBitset.ixx
		NYANData.ixx
		StaticBitset.ixx
)
	
target_compile_features(Data PUBLIC cxx_std_23)

add_library(NYAN::Data ALIAS Data)