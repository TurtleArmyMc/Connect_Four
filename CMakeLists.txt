cmake_minimum_required(VERSION 3.12)
project(ConnectFour)

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE sources src/*.cpp src/*.hpp)

if (WIN32)
	# Windows build script untested; may or may not work.
	set(SDL2_PATH "C:\\MinGW\\include\\SDL2")
	find_package(SDL2 REQUIRED)
	include_directories( ${SDL2_INCLUDE_DIR} )

	add_library(SDL2 STATIC IMPORTED)

	link_libraries (
		${SDL2_LIBRARY} -lmingw32 -lSDL2main -lSDL2
		SDL2main
	)

	add_executable(ConnectFour WIN32 ${sources})
	target_link_libraries(ConnectFour ${SDL2_LIBRARY} -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf)
endif (WIN32)

if (UNIX)
	add_executable(ConnectFour ${sources})
	target_link_libraries(ConnectFour ${SDL2_LIBRARY} -lSDL2 -lSDL2_ttf)
endif (UNIX)

# Copy resources folder with ttf font in it.
FILE(COPY resources/ DESTINATION "${CMAKE_BINARY_DIR}/resources")