cmake_minimum_required(VERSION 3.22.1)
project(Group-Project)

#Executable
add_library(DebugMode STATIC
	DebugMode.h
	Tasks.h

	DebugMode.cpp
)