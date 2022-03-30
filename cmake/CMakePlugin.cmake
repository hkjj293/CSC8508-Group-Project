cmake_minimum_required(VERSION 3.22.1)

include(CMakeSettings)

set(CMAKE_C_FLAGS)
set(CMAKE_CXX_FLAGS)
set(CMAKE_C_FLAGS_DEBUG "-D_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-D_DEBUG")
set(CMAKE_C_FLAGS_RELEASE "-DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG")

#Setup Platforms
if( PLATFORM_WINDOWS )
    add_compile_options(-DWINDOWSPC)
elseif( PLATFORM_PS4 )
    if( NOT (EXISTS "${PROJECT_SOURCE_DIR}/platforms/ps4" AND IS_DIRECTORY "${PROJECT_SOURCE_DIR}/platforms/ps4"))
        message(FATAL_ERROR "Could not find PlayStation4 engine source directory, cannot continue.")
    endif()
    add_definitions(-DPS4)
    add_compile_options(-D__ORBIS__ -DX64 -D__SSE3__)
endif()

#Define Standard Configurations
if ( CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_CONFIGURATION_TYPES MATCHES "Debug;Release")
    list(APPEND Configs Debug Release)
    set(CMAKE_CONFIGURATION_TYPES ${Configs} CACHE STRING "List of supported configurations." FORCE)
    set(CMAKE_INSTALL_PREFIX $ENV{SR_BIN_DIR} CACHE STRING "Default installation directory." FORCE)
    message(FATAL_ERROR "Default configuration was reset, please re-run CMake.")
endif()

# Initialize the development configuration using release configuration
set(CMAKE_C_FLAGS_DEV "${CMAKE_C_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_DEV "${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_STATIC_LINKER_FLAGS_DEV "${CMAKE_STATIC_LINKER_FLAGS_RELEASE}")
set(CMAKE_SHARED_LINKER_FLAGS_DEV "${CMAKE_SHARED_LINKER_FLAGS_RELEASE}")
set(CMAKE_MODULE_LINKER_FLAGS_DEV "${CMAKE_MODULE_LINKER_FLAGS_RELEASE}")
set(CMAKE_EXE_LINKER_FLAGS_DEV "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")

# Set global defines
add_compile_options(-D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE)
add_compile_options($<$<CONFIG:DEBUG>:-D_SECURE_SCL_THROWS=0> $<$<CONFIG:DEBUG>:-D_SILENCE_DEPRECATION_OF_SECURE_SCL_THROWS>)
add_compile_options(-D_HAS_ITERATOR_DEBUGGING=$<CONFIG:DEBUG> -D_SECURE_SCL=$<CONFIG:DEBUG>)
add_compile_options(-D_HAS_EXCEPTIONS=0)

add_compile_options($<$<NOT:$<CONFIG:RELEASE>>:-DDEVELOPMENT>)
add_compile_options($<$<NOT:$<CONFIG:RELEASE>>:-DUNIT_TESTS>)
add_compile_options($<$<NOT:$<CONFIG:RELEASE>>:-DUSE_CALLSTACK>)

add_compile_options($<$<NOT:$<CONFIG:RELEASE>>:-DHAS_PROFILER>)
add_compile_options($<$<NOT:$<CONFIG:RELEASE>>:-DHAS_LOADING_PROFILER>)
add_compile_options($<$<NOT:$<CONFIG:RELEASE>>:-DHAS_CONSOLE_SERVER>)

#Define Platform Architecture
if (PLATFORM_WINDOWS OR PLATFORM_PS4)
    add_compile_options(-DPLATFORM_ARCH_x86)
else()
    message(FATAL_ERROR "Unknown platform architecture!")
endif()

# Define 32 versus 64 bit architecture
if( PLATFORM_64BIT )
	add_compile_options(-DPLATFORM_64BIT)
else()
	add_compile_options(-DPLATFORM_32BIT)
endif()