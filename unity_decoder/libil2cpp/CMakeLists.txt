#This CMake project file for libil2cpp only exists as a convenience for working on the .cpp and .h files
#for people who like to use CLion as their IDE. this is not used by il2cpp itself
cmake_minimum_required(VERSION 3.4)
project(libil2cpp)

file(GLOB_RECURSE LIBIL2CPP_FILES {PROJECT_SOURCE_DIR} *.cpp *.h)
set(BOEHMGC_FILES "../external/boehmgc/extra/gc.c")
include_directories("." "../external/boehmgc/include")
add_library(libil2cpp ${LIBIL2CPP_FILES} ${BOEHMGC_FILES})
