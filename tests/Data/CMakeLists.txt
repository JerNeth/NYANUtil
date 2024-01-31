add_executable(DataTests "DynamicBitsetTests.cpp"  "DynamicArrayTests.cpp" "StaticBitsetTests.cpp")

enable_testing()
target_link_libraries(DataTests 
	PRIVATE 
			NYAN::Data
			GTest::gtest_main)

			
include(GoogleTest)
gtest_discover_tests(DataTests)