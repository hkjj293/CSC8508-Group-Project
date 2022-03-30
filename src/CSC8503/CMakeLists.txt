cmake_minimum_required(VERSION 3.22.1)
project(Group-Project)

#CSC8503 Component
add_library(CSC8503 STATIC
	GameLoadingRenderer.cpp
	GameTechRenderer.cpp
	GameWorld.cpp
	PushdownMachine.cpp
	StateMachine.cpp
	StateTransition.cpp
	Transform.cpp

	GameLoadingRenderer.h
	GameTechRenderer.h
	GameWorld.h
	PushdownMachine.h	
	PushdownState.h
	State.h
	StateMachine.h
	StateTransition.h
	Transform.h
)