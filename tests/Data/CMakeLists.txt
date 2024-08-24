add_executable(DataTests 
	"DynamicArrayTests.cpp" 
	"DynamicBitsetTests.cpp"  
	"HashTests.cpp" 
	"StaticBitsetTests.cpp" 
	"StaticVectorTests.cpp")

enable_testing()
target_link_libraries(DataTests 
	PRIVATE 
			NYAN::Data
			GTest::gtest_main)

			
include(GoogleTest)
gtest_discover_tests(DataTests)