
add_library(Math)
target_sources(Math
	PRIVATE
	PUBLIC
		FILE_SET cxx_modules TYPE CXX_MODULES FILES
		NYANMath.ixx
		Half.ixx
		Vector.ixx
		Matrix.ixx
		Quaternion.ixx 
		Util.ixx 
		Constants.ixx)
	
target_compile_features(Math PUBLIC cxx_std_23)

add_library(NYAN::Math ALIAS Math)