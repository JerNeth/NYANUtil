add_executable(MathTests VectorTests.cpp MatrixTests.cpp)

enable_testing()
target_link_libraries(MathTests 
	PRIVATE 
			NYAN::Math
			GTest::gtest_main)

			
include(GoogleTest)
gtest_discover_tests(MathTests)