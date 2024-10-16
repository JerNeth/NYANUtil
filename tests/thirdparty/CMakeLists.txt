
CPMAddPackage(NAME googletest
	GITHUB_REPOSITORY google/googletest
	GIT_TAG	main
)

set(GTEST_BOTH_LIBRARIES
    gtest_main 
    gtest
)