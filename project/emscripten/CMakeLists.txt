set(CMAKE_VERBOSE_MAKEFILE ON)

cmake_minimum_required (VERSION 3.8)
project(matt2)

# Directory variables.
set(src ../..)

include_directories(${EMSDK}/system/include/emscripten)
include_directories(${src})

add_library (matt2 
	"${src}/build_env.h"
	"${src}/console.h"
	"${src}/daily_chess_scoring.cpp"
	"${src}/daily_chess_scoring.h"
	"${src}/game.cpp"
	"${src}/game.h"
	"${src}/move.cpp"
	"${src}/move.h"
	"${src}/notation.cpp"
	"${src}/notation.h"
	"${src}/piece.cpp"
	"${src}/piece.h"
	"${src}/piece_value_scoring.cpp"
	"${src}/piece_value_scoring.h"
	"${src}/placement.h"
	"${src}/position.cpp"
	"${src}/position.h"
	"${src}/relocation.h"
	"${src}/rules.cpp"
	"${src}/rules.h"
	"${src}/scoring.cpp"
	"${src}/scoring.h"
	"${src}/square.cpp"
	"${src}/square.h"
)

add_definitions(-Dwasm)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-g0 -O3")
set(CMAKE_CXX_FLAGS "-std=c++20 -Werror -fwasm-exceptions")

set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-g -O0")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-g0 -O3")
set(CMAKE_EXE_LINKER_FLAGS "-s ALLOW_MEMORY_GROWTH=1 -s STACK_SIZE=5MB -fwasm-exceptions")
