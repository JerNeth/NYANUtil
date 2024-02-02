add_executable(LogTests LogTests.cpp)

enable_testing()
target_link_libraries(LogTests 
	PRIVATE 
			NYAN::Log
			GTest::gtest_main)

			
target_compile_definitions(LogTests PUBLIC LOGVERBOSITY=15)

include(GoogleTest)
gtest_discover_tests(LogTests)