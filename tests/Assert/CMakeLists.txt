add_executable(AssertTests AssertTests.cpp)

target_link_libraries(AssertTests 
	PRIVATE 
			NYAN::Assert
			GTest::gtest_main)

			
target_compile_definitions(AssertTests PRIVATE LOGVERBOSITY=15)

include(GoogleTest)
gtest_discover_tests(AssertTests)