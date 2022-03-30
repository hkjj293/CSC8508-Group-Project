cmake_minimum_required(VERSION 3.22.1)
project(Group-Project)

add_library(PlayStation4 STATIC
	InputBase.cpp
	Ps4AudioEmitter.cpp
	Ps4AudioSource.cpp
	PS4AudioSystem.cpp
	PS4ComputeShader.cpp
	PS4Frame.cpp
	PS4Input.cpp
	PS4MemoryAware.cpp
	PS4Mesh.cpp
	PS4RendererBase.cpp
	PS4Shader.cpp
	PS4Sound.cpp
	PS4Texture.cpp
	PS4Window.cpp



	InputBase.h
	Ps4AudioEmitter.h
	PS4AudioSource.h
	Ps4AudioSystem.h
	PS4ComputeShader.h
	PS4Frame.h
	PS4Input.h
	PS4MemoryAware.h
	PS4Mesh.h
	PS4RendererBase.h
	PS4Shader.h
	PS4Sound.h
	PS4Texture.h
	PS4Window.h
)