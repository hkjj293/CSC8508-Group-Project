#include "GameWorld.h"
#include "../game/GameEntity.h"
#include "../Common/Camera.h"

#include <algorithm>

using namespace NCL;
using namespace NCL::CSC8503;

GameWorld::GameWorld()	{
	shuffleConstraints	= false;
	shuffleObjects		= false;
	worldIDCounter		= 0;
	localPlayerCount = 0;
	isLocalGame = true;
}

GameWorld::~GameWorld()	{
	ClearAndErase();
}

void GameWorld::Clear() {
	gameObjects.clear();
	players.clear();
}

void GameWorld::ClearAndErase() {
	for (int i = 0; i < gameObjects.size(); i++) {
			delete gameObjects[i];
	}
	for (int i = 0; i < mainCameras.size(); i++) {
		delete mainCameras[i];
	}
	Clear();
}

void GameWorld::AddGameObject(GameEntity* o) {
	gameObjects.emplace_back(o);
	o->SetWorldID(worldIDCounter++);
}

void GameWorld::RemoveGameObject(GameEntity* o, bool andDelete) {
	gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), o), gameObjects.end());
	if (andDelete) {
		delete o;
	}
}

void GameWorld::AddPlayer(GameEntity* o) {
	players.emplace_back(o);
	o->SetWorldID(worldIDCounter++);
}

void GameWorld::RemovePlayer(GameEntity* o, bool andDelete) {
	players.erase(std::remove(players.begin(), players.end(), o), players.end());
	if (andDelete) {
		delete o;
	}
}

void GameWorld::GetObjectIterators(
	GameObjectIterator& first,
	GameObjectIterator& last) const {

	first	= gameObjects.begin();
	last	= gameObjects.end();
}

void GameWorld::OperateOnContents(GameObjectFunc f) {
	for (GameEntity* g : gameObjects) {
		f(g);
	}
}

void GameWorld::UpdateWorld(float dt) {
	if (shuffleObjects) {
		std::random_shuffle(gameObjects.begin(), gameObjects.end());
	}
}

void GameWorld::UpdatePositions() {
	for (GameEntity* g : gameObjects) {
		g->UpdateRenderPositions();
	}
}
