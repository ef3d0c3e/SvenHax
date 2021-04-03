CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT("SvenHax")

SET(CMAKE_CXX_FLAGS "-std=c++2a -m32 -ldl -lpthread -march=native -mtune=native -ftree-vectorize -Wall -fno-gnu-unique -fopenmp -fpic -O3")
SET(CMAKE_C_FLAGS "-m32 -march=native -mtune=native -Wall -fno-gnu-unique -fpic -O3")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -s -fvisibility=hidden -fvisibility-inlines-hidden")
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -s -fvisibility=hidden -fvisibility-inlines-hidden")
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

FILE(GLOB SOURCES
	"src/*.cpp"
	"src/ImGui/imgui_draw.cpp"
	"src/ImGui/examples/*.cpp"
	"src/ImGui/examples/libs/gl3w/GL/*.c"
	"src/Util/*.cpp"
	"src/Hooks/*.cpp"
	"src/UI/*.cpp"
	"src/Hacks/*.cpp"
)

INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/ImGui/")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/ImGui/examples/")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/ImGui/examples/libs/")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/ImGui/examples/libs/gl3w/")

ADD_SUBDIRECTORY(libraries/fmt)
ADD_LIBRARY(${CMAKE_PROJECT_NAME} SHARED ${SOURCES})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} fmt dl SDL2 GL)
