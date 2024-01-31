add_executable(LogTests LogTests.cpp)

enable_testing()
target_link_libraries(LogTests 
	PRIVATE 
			NYAN::Log
			GTest::gtest_main)

			
include(GoogleTest)
gtest_discover_tests(LogTests)