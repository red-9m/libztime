cmake_minimum_required(VERSION 3.2)
project (libztime-project C)

if(POLICY CMP0065)
    cmake_policy(SET CMP0065 NEW)
endif()

option(BUILD_SHARED_LIBS "Default build library as shared" ON)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Set Release as default build type" FORCE)
endif()

message(STATUS "------------------------------------------------------------------------------------")
message(STATUS "Project name     (PROJECT_NAME)             : " ${PROJECT_NAME})
message(STATUS "Project dir      (PROJECT_SOURCE_DIR)       : " ${PROJECT_SOURCE_DIR})
message(STATUS "Build type       (CMAKE_BUILD_TYPE)         : " ${CMAKE_BUILD_TYPE})
message(STATUS "Build shared     (BUILD_SHARED_LIBS)        : " ${BUILD_SHARED_LIBS})
message(STATUS "Install prefix   (CMAKE_INSTALL_PREFIX)     : " ${CMAKE_INSTALL_PREFIX})
message(STATUS "Install lib      (CMAKE_INSTALL_LIBDIR)     : " ${CMAKE_INSTALL_LIBDIR})
message(STATUS "Install include  (CMAKE_INSTALL_INCLUDEDIR) : " ${CMAKE_INSTALL_INCLUDEDIR})
message(STATUS "------------------------------------------------------------------------------------")

# Sources
file(GLOB sources ztime*.c)
file(GLOB headers ztime.h)
file(GLOB sample-src sample.c)

# Targets
add_library(ztime ${sources})
add_executable(sample EXCLUDE_FROM_ALL ${sample-src})

# Options
# Additional: -march=core-avx2 -mtune=core-avx2
set(_COMPILE_OPT -pipe -ftree-vectorize -ffast-math -flto -Wall -Wextra -Wpedantic)
set(_LINK_OPT "-flto -fuse-ld=gold")
set(_COMPILE_DEF -D_POSIX_C_SOURCE=199309L -D_XOPEN_SOURCE)

if (CMAKE_BUILD_TYPE EQUAL "Debug")
    set(_COMPILE_OPT ${_COMPILE_OPT} -g)
    set(_LINK_OPT "${_LINK_OPT} -g")
endif()

# ztime
set_target_properties(ztime PROPERTIES C_STANDARD 11 C_STANDARD_REQUIRED ON C_EXTENSIONS OFF)
target_compile_options(ztime PRIVATE ${_COMPILE_OPT})
set_target_properties(ztime PROPERTIES LINK_FLAGS ${_LINK_OPT})
set_target_properties(ztime PROPERTIES PUBLIC_HEADER ${headers})
target_compile_definitions(ztime PRIVATE ${_COMPILE_DEF})
target_include_directories(ztime PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

# sample
set_target_properties(sample PROPERTIES C_STANDARD 11 C_STANDARD_REQUIRED ON C_EXTENSIONS OFF)
target_compile_options(sample PRIVATE ${_COMPILE_OPT})
set_target_properties(sample PROPERTIES LINK_FLAGS ${_LINK_OPT})
target_compile_definitions(sample PRIVATE ${_COMPILE_DEF})
target_link_libraries(sample ztime)

# Install
install(TARGETS ztime LIBRARY)
install(TARGETS ztime PUBLIC_HEADER)
