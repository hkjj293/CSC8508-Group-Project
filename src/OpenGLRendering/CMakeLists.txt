cmake_minimum_required(VERSION 3.22.1)
project(Group-Project)

add_compile_definitions(GLEW_STATIC)

find_package(OpenGL REQUIRED)
if(!OpenGL_FOUND)
	message(WARNING "OpenGL Not Found")
endif()

add_subdirectory(KHR)

#Graphic Component
add_library(OpenGLRendering STATIC
	OGLComputeShader.cpp
	OGLMesh.cpp
	OGLRenderer.cpp	
	OGLShader.cpp
	OGLTexture.cpp
	glad.c

	OGLComputeShader.h
	OGLMesh.h
	OGLRenderer.h
	OGLShader.h
	OGLTexture.h
)

target_link_libraries(OpenGLRendering KHR_LIB ${OPENGL_gl_LIBRARY})