cmake_minimum_required(VERSION 3.10)
project(glow)

# include (cmake/utils.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)
#set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined")
# # Enable C++11
# set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O0 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined")


include_directories(.)
include_directories(/src/include)
# link_directories(/src/lib)

# lib yaml-cpp
include_directories(extern/yaml-cpp/include)
add_subdirectory(extern/yaml-cpp lib)


add_executable(test_config test/test_config.cc)
target_link_libraries(test_config yaml-cpp)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

