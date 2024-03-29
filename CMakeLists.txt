cmake_minimum_required(VERSION 3.0)

project(detectcpu C)

# written by: Oliver Cordes 2019-07-07
# changed by: Oliver Cordes 2019-07-07



include(CheckIncludeFile)
check_include_file("cpuid.h" HAVE_CPUID_H)
if(NOT HAVE_CPUID_H)
  message( FATAL_ERROR "cpuid.h Not Found" )
endif()


file(GLOB_RECURSE target_sources src/detect-cpu.c )
#set(target_sources "../src/detect-cpu.c" )

# get current date
execute_process(COMMAND "date" "+%Y-%m-%d" OUTPUT_VARIABLE BUILD)
string( REPLACE "\n" "" BUILD ${BUILD})
message(STATUS "Compilation date = ${BUILD}")

message(STATUS "Sources = ${target_sources}")


# for Linux gcc compiler only
IF(CMAKE_COMPILER_IS_GNUCC)
    ADD_DEFINITIONS(-Wall -Wextra -Wshadow)
ENDIF()


add_executable(detect-cpu ${target_sources})


target_compile_options(detect-cpu PUBLIC  -g -O3 -Wall)

install(TARGETS detect-cpu DESTINATION bin)
