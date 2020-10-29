cmake_minimum_required (VERSION 3.8)

project(HexGrid VERSION 1.0.0)

set(SFML_DIR "lib/SFML-2.5.1/lib/cmake/SFML")
set(CMAKE_CXX_FLAGS "-O2")

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

file( GLOB BASE_SOURCES src/*.cpp )
file( GLOB BASE_HEADERS src/*.h )

add_executable (HexGrid ${BASE_SOURCES} ${BASE_HEADERS} )

target_link_libraries(HexGrid PRIVATE sfml-graphics)
