cmake_minimum_required(VERSION 3.22.1)

if(_SETTINGS_)
    return()
endif()
set(_SETTINGS_ 1)

include(CMakePlatforms)
include(CMakeMacros)

set(PRODUCT_NAME "Ink!")
set(PRODUCT_COMPANY "Team 6")
set(PRODUCT_VERSION_MAJOR "1")
set(PRODUCT_VERSION_MINOR "9")
set(PRODUCT_VERSION_REVISION "0")

set(PRODUCT_DEV_VERSION_REVISION "0")
string(TIMESTAMP PRODUCT_DEFAULT_BUILD_TIMESTAMP "%Y-%m-%dT%H:%M:%SZ" UTC)

if(NOT REPOSITORY_DIR OR REPOSITORY_DIR STREQUAL "")
    message(FATAL_ERROR "REPOSITORY_DIR not set! Please set REPOSITORY_DIR to appropriate location when running this CMake script.")
endif()
string(REGEX REPLACE "\\\\" "/" REPOSITORY_DIR ${REPOSITORY_DIR})

if( NOT CMAKE_INSTALL_PREFIX OR CMAKE_INSTALL_PREFIX STREQUAL "" )
	message(FATAL_ERROR "CMAKE_INSTALL_PREFIX not set! Please set CMAKE_INSTALL_PREFIX to appropriate location when running this CMake script.")
endif()
string(REGEX REPLACE "\\\\" "/" BINARIES_DIR ${CMAKE_INSTALL_PREFIX})

#set(PACKAGE_CACHE_FILE "${CMAKE_BINARY_DIR}/cmake_package_cache.cmake")
#generate_package_location_cache()

# Set install directories
if( PLATFORM_WINDOWS )
	set(Engine_INSTALL_DIR "${BINARIES_DIR}/engine/win${ARCH_BITS}/$<LOWER_CASE:$<CONFIG>>")
	# PLUGIN REPO CUSTOM CHANGE
	#set(EDITOR_INSTALL_DIR "${BINARIES_DIR}/editor/win${ARCH_BITS}/$<LOWER_CASE:$<CONFIG>>")
else()
	set(Engine_INSTALL_DIR "${BINARIES_DIR}/engine/${PLATFORM_NAME}/$<LOWER_CASE:$<CONFIG>>")
	# PLUGIN REPO CUSTOM CHANGE
	#set(EDITOR_INSTALL_DIR "${BINARIES_DIR}/editor/${PLATFORM_NAME}/$<LOWER_CASE:$<CONFIG>>")
endif()

if (PLATFORM_WINDOWS)
    set(BUILD_SHARED_LIBS ON CACHE BOOL "Build plug-ins as shared libraries.")
else()
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build plug-ins as static libraries.")
endif()

set(BUILD_RUN_TEST OFF CACHE BOOL "Enable special build for run test mode.")

#Engine Options
set(ENGINE_USE_DEBUG_INFO ON CACHE BOOL "Enable debug information in all configurations.")
set(ENGINE_USE_PS4_DATA_COMPILER_PLUGIN ON CACHE BOOL "Enable PS4 data compiler support.")

set(ENGINE_FOLDER_NAME "runtime")
set(ENGINE_PLUGINS_FOLDER_NAME "plugins")
set(ENGINE_USE_SOLUTION_FOLDERS ON)

if (PLATFORM_64BIT AND PLATFORM_WINDOWS)
    set(ENGINE_CAN_COMPILE 1)
endif()

if( NOT ENGINE_CAN_COMPILE )
    set(ENGINE_USE_PS4_DATA_COMPILER_PLUGIN OFF)
endif()

if( ENGINE_USE_PS4_DATA_COMPILER_PLUGIN )
	if( NOT PLATFORM_WINDOWS )
		message(STATUS "PS4 data compiler support disabled, unsupported by platform ${PLATFORM_NAME}.")
		set(ENGINE_USE_PS4_DATA_COMPILER_PLUGIN OFF)
	elseif( NOT (EXISTS "${REPOSITORY_DIR}/runtime/platforms/ps4" AND IS_DIRECTORY "${REPOSITORY_DIR}/runtime/platforms/ps4") )
		message(STATUS "PS4 data compiler support disabled, source not found.")
		set(ENGINE_USE_PS4_DATA_COMPILER_PLUGIN OFF)
	elseif( NOT DEFINED PS4_ROOT OR NOT DEFINED PS4_SDK )
		message(STATUS "PS4 data compiler support disabled, PS4 SDK not found or wrong version.")
		set(ENGINE_USE_PS4_DATA_COMPILER_PLUGIN OFF)
	endif()
	if( EXISTS "${REPOSITORY_DIR}/runtime/platforms/ps4" AND IS_DIRECTORY "${REPOSITORY_DIR}/runtime/platforms/ps4" )
		file(GLOB_RECURSE RESULT "${REPOSITORY_DIR}/runtime/platforms/ps4/*.*")
		list(LENGTH RESULT RES_LEN)
		if( RES_LEN EQUAL 0 ) # DIR is empty
			message(STATUS "PS4 data compiler support disabled, source not found.")
			set(ENGINE_USE_PS4_DATA_COMPILER_PLUGIN OFF)
		endif()
	endif()
endif()