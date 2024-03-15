add_executable(CoroTests CoroTests.cpp)

enable_testing()
target_link_libraries(CoroTests 
	PRIVATE 
			NYAN::Coro
			GTest::gtest_main)

	
include(GoogleTest)
gtest_discover_tests(CoroTests)