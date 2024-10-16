
CPMAddPackage(NAME googletest
	GITHUB_REPOSITORY google/googletest
	GIT_TAG	main
	OPTIONS "gtest_force_shared_crt  ON" "BUILD_GMOCK OFF" "INSTALL_GTEST OFF"
)

set(GTEST_BOTH_LIBRARIES
    gtest_main 
    gtest
)