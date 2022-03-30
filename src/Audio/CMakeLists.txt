cmake_minimum_required(VERSION 3.22.1)
project(Group-Project)

add_library(Audio STATIC
	common.h
	common_platform.h
	AudioManager.h

	common.cpp
	common_platform.cpp
	AudioManager.cpp
)