cmake_minimum_required(VERSION 3.22.1)
project(Group-Project)

#Physics Component
add_library(Physics STATIC
	PhysicsTestScene.cpp
	PhysicsObject.cpp
	VkTechRenderer.cpp

	PhysicsTestScene.h
	PhysicsObject.h
	VkTechRenderer.h
)