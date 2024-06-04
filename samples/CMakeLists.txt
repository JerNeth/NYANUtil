

add_executable(Usage
		Usage.cpp)

target_link_libraries(Usage 
		PRIVATE NYAN::Math)

add_executable(CoroUsage
		CoroUsage.cpp)

target_link_libraries(CoroUsage 
		PRIVATE NYAN::Coro)