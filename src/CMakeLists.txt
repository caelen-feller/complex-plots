cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(cplot VERSION 0.1 LANGUAGES C)

# User Controlled Options
option(BUILD_SHARED_LIBS "Build shared library or not" ON)
option(USE_PARALLEL_LOOPS "Use parallel loops - memory overhead" ON)
option(BUILD_DOCS "Build Sphinx documentation" OFF)
option(ENABLE_TESTING "Enable Tests" ON)
option(USE_PROFILING "Enable gprof flags" OFF)
option(BUILD_EXAMPLES "Build Library Examples" OFF)
option(BUILD_PLOT "Build complex plotter command line utility" OFF)

# Necessary Packages
include(GNUInstallDirs)
include(CTest)
include(ExternalProject)

enable_testing()

# Compiler Flags 
set(CMAKE_C_FLAGS                "-Wall -Wextra")
set(CMAKE_C_FLAGS_DEBUG          "-g -O0 -fno-inline")
set(CMAKE_C_FLAGS_MINSIZEREL     "-Os")
set(CMAKE_C_FLAGS_RELEASE        "-O3")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")

if(USE_PARALLEL_LOOPS)
    find_package(OpenMP REQUIRED)
    if(OPENMP_FOUND)
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCPLOT_USE_PARALLEL_LOOPS")
    endif()
endif()

if(USE_PROFILING)
    set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-pg")
endif()

# Dependencies
find_package(ImageMagick REQUIRED COMPONENTS MagickWand)

add_library(imagemagick SHARED IMPORTED)
set_property(TARGET imagemagick PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${ImageMagick_INCLUDE_DIRS})
set_property(TARGET imagemagick PROPERTY IMPORTED_LOCATION ${ImageMagick_LIBRARIES})

# Init Rest of Build 
add_subdirectory(cplot)