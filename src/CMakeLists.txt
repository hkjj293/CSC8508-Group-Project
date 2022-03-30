add_subdirectory(Audio)
add_subdirectory(common)
add_subdirectory(CSC8503)
add_subdirectory(OpenGLRendering)
#add_subdirectory(VulkanRendering)
#add_subdirectory(Physics)
add_subdirectory(game)
add_subdirectory(assimp)
add_subdirectory(GUI)
add_subdirectory(imgui)
add_subdirectory(DebugMode)


#Targeting Directories & Linking Libraries
find_package(Bullet CONFIG REQUIRED)
if(Bullet_FOUND)
	#target_include_directories(Physics PRIVATE ${BULLET_INCLUDE_DIRS})
else()
	message(WARNING "Bullet Physics Engine Cannot Be Found, Build Might Not Work Properly")
endif()

#FMOD
add_library(FMOD SHARED IMPORTED)
set_property(TARGET FMOD PROPERTY
	IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/FMOD/api/core/lib/x64/fmod.dll"
)
set_property(TARGET FMOD PROPERTY
	IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/FMOD/api/core/lib/x64/fmod_vc.lib"
)
set_property(TARGET FMOD PROPERTY
	INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/FMOD/api/core/inc"
)

#Assimp
#add_library(ASSIMP SHARED IMPORTED)
#set_property(TARGET ASSIMP PROPERTY
#	IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/assimp/bin/Debug/assimp-vc142-mtd.dll"
#)
#set_property(TARGET ASSIMP PROPERTY
#	IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/assimp/lib/Debug/assimp-vc142-mtd.lib"
#)
#set_property(TARGET ASSIMP PROPERTY
#	IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/assimp/contrib/zlib/Debug/zlibstaticd.lib"
#)
#set_property(TARGET ASSIMP PROPERTY
#	INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/assimp/include"
#)

#if (DEFINED VULKAN_SDK_PATH)
#  set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include") # 1.1 Make sure this include path is correct
#  set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/Lib") # 1.2 Make sure lib path is correct
#  set(Vulkan_FOUND "True")
#else()
#  find_package(Vulkan REQUIRED) # throws error if could not find Vulkan
#  message(STATUS "Found Vulkan: $ENV{VULKAN_SDK}")
#endif()
#if (NOT Vulkan_FOUND)
#    message(FATAL_ERROR "Could not find Vulkan library!")
#else()
#    message(STATUS "Using vulkan lib at: ${Vulkan_LIBRARIES}")
#endif()

target_link_libraries(GUI imgui Common)
target_link_libraries(Game OpenGLRendering CSC8503 Common Audio GUI DebugMode) #vk,Physics
#target_link_libraries(Physics ${BULLET_LIBRARIES} Common OpenGLRendering CSC8503) #vk
target_link_libraries(CSC8503 Common OpenGLRendering ${BULLET_LIBRARIES})
target_link_libraries(OpenGLRendering Common)
#target_link_libraries(VulkanRendering Vulkan::Vulkan Common)
target_link_libraries(Common ${BULLET_LIBRARIES} assimp)
target_link_libraries(Audio FMOD CSC8503)

#Copy fmod.dll to Output Directory
get_property(FMOD_DLL TARGET FMOD PROPERTY IMPORTED_LOCATION)
file(COPY ${FMOD_DLL} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
file(COPY ${FMOD_DLL} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)