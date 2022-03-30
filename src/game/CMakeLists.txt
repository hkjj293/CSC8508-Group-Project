cmake_minimum_required(VERSION 3.22.1)
project(Group-Project)

#Executable
add_executable(Game 
	Controls.h
	ControlsCommand.h
	Game.h
	GameEntity.h
	Items.h
	Player.h
	PlayerInput.h
	TransformConverter.h
	Wall.h
	Bullet.h	
	Painter.h
	BulletPool.h
	AssetsManager.h
	LevelGen.h


	main.cpp
	Game.cpp
	GameEntity.cpp
	Items.cpp
	Player.cpp
	PlayerInput.cpp
	TransformConverter.cpp
	Wall.cpp
	Bullet.cpp
	BulletPool.cpp
	Painter.cpp
	AssetsManager.cpp
	LevelGen.cpp

)