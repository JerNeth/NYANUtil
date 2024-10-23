add_executable(DataTests 
	"DynamicArrayTests.cpp" 
	"DynamicBitsetTests.cpp"  
	"NotNullTests.cpp"
	"HashTests.cpp" 
	"StaticBitsetTests.cpp" 
	"StaticVectorTests.cpp"
)

target_link_libraries(DataTests 
	PRIVATE 
			NYAN::Data
			GTest::gtest_main)

			
include(GoogleTest)
gtest_discover_tests(DataTests)