
cmake_minimum_required(VERSION 3.5.1)

# project information
project(lex VERSION 0.1.0 LANGUAGES C CXX)

include(${PROJECT_SOURCE_DIR}/utils.cmake)
# Compile options
# add_compile_options(-std=c++11)
set (CMAKE_CXX_STANDARD 11)
add_definitions(-DENABLE_DVPP_INTERFACE)

# set(common_compile_definitions _GLIBCXX_USE_CXX11_ABI=01)
set(CMAKE_CXX_FLAGS_RELEASE "-fPIC -O2 -Wall")


IF (PRINT_DEBUG)
add_definitions(-DPRINT_DEBUG)
# message(STATUS "set printf for debug: ")
ENDIF()

if (NOT DEFINED THREEPARTY_LIB)
set(THREEPARTY_LIB /usr/)
endif()

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/3rdparty/common/include)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/utils)
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src SRC)
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/utils UTILS)

add_executable(test ${PROJECT_SOURCE_DIR}/sample/test.cpp ${SRC} ${UTILS})
redefine_file_macro(test)
target_link_libraries(test)

