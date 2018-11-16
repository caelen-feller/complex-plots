project(libcplot VERSION 0.1 LANGUAGES C)

# Build Library
file(GLOB_RECURSE SRCS src/*.c)
add_library(cplot ${SRCS})

# Include Directories
target_include_directories(cplot PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)

# Linker
target_link_libraries(cplot m gmp arb flint imagemagick)

# Install (using GNUInstallDirs).
install(TARGETS cplot EXPORT LibCPlotConfig
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION )
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# This makes the project importable from the install directory
install(EXPORT LibCPlotConfig DESTINATION share/libcplot/cmake)

# This makes the project importable from the build directory
export(TARGETS cplot FILE LibCPlotConfig.cmake)

# Testing
add_subdirectory(test)