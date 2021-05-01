cmake_minimum_required(VERSION 3.12)
project(ConnectFour)

set(CMAKE_CXX_STANDARD 14)

file(GLOB sources src/*.cpp src/*.hpp)
file(GLOB_RECURSE sdl_renderer
	src/renderer/renderer.hpp
	src/renderer/sdl_renderer/*.cpp
	src/renderer/sdl_renderer/*.hpp
)
file(GLOB_RECURSE ncurses_renderer
	src/renderer/renderer.hpp
	src/renderer/ncurses_renderer/*.cpp
	src/renderer/ncurses_renderer/*.hpp
)

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

	add_executable(ConnectFour WIN32 ${sources} ${sdl_renderer})
	target_compile_definitions(ConnectFour PUBLIC -DSDL_RENDERER)
	target_link_libraries(ConnectFour ${SDL2_LIBRARY} -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf)

	add_executable(ConnectFourCMD WIN32 ${sources} ${ncurses_renderer})
	target_compile_definitions(ConnectFourCMD PUBLIC -DNCURSES_RENDERER)
	target_link_libraries(ConnectFourCMD ${SDL2_LIBRARY} -lncurses)
endif (WIN32)

if (UNIX)
	add_executable(ConnectFour ${sources} ${sdl_renderer})
	target_compile_definitions(ConnectFour PUBLIC -DSDL_RENDERER)
	target_link_libraries(ConnectFour ${SDL2_LIBRARY} -lSDL2 -lSDL2_ttf)

	add_executable(ConnectFourCMD ${sources} ${ncurses_renderer})
	target_compile_definitions(ConnectFourCMD PUBLIC -DNCURSES_RENDERER)
	target_link_libraries(ConnectFourCMD -lncurses)
endif (UNIX)

# Copy resources folder with ttf font in it.
FILE(COPY resources/ DESTINATION "${CMAKE_BINARY_DIR}/resources")