CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT("SvenHax")

SET(CMAKE_CXX_FLAGS "-std=c++2a -m32 -ldl -lpthread -march=native -mtune=native -ftree-vectorize -Wall -fno-gnu-unique -fopenmp -fpic -O3")
SET(CMAKE_C_FLAGS "-m32 -fpic -O3")
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

FILE(GLOB SOURCES
	"src/*.cpp"
	"src/ImGUI/*.cpp"
	"src/ImGUI/examples/*.cpp"
	"src/ImGUI/examples/libs/gl3w/GL/*.c"
	"src/SDK/*.cpp"
	"src/Util/*.cpp"
	"src/Hooks/*.cpp"
	"src/UI/*.cpp"
)

INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/ImGUI/")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/ImGUI/examples/")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/ImGUI/examples/libs/")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/ImGUI/examples/libs/gl3w/")

ADD_SUBDIRECTORY(libraries/fmt)
ADD_LIBRARY(${CMAKE_PROJECT_NAME} SHARED ${SOURCES})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} fmt dl SDL2 GL)
