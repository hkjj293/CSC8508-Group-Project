#pragma once

#include "../game/GameEntity.h"
#include "../common/RendererBase.h"
#include "../CSC8503/GameTechRenderer.h"
#include "../CSC8503/GameLoadingRenderer.h"
//#include "../Physics/VkTechRenderer.h"
#include "../game/TransformConverter.h"
#include "ControlsCommand.h"
#include "PlayerInput.h"
#include "Player.h"
#include "Items.h"
#include "Wall.h"
#include "../DebugMode/DebugMode.h"
#include "../Audio/AudioManager.h"
#include "AssetsManager.h"
#include <atomic>
#include "../GUI/GameUI.h"
#include "../GUI/PauseMenu.h"
#include "../CSC8503/StateMachine.h"
#include "../CSC8503/PushdownMachine.h"
#include "../CSC8503/State.h"
#include "../CSC8503/PushdownState.h"

//Encapsulate in namespace?

class Game {
public:
	Game(Tasks* tasks, DebugMode& d);
	~Game();

	void Update(float dt);
	bool End() { return end; };
	void GetPhysicsTestSceneDebugData(std::shared_ptr<DebugMode> d);

protected:
	void Reset();
	void InitWorld();
	void RenderLoading();
	void Init(Tasks* tasks);
	void Destroy();
	void InitGUI();
	void InitPhysics();
	void InitAudio();
	void InitAssets();
	void InitScene();
	//void InitItems();
	//Build State Machine?
	void LevelGeneration();
	void InitCharacter();
	void InitPlayerInput();
	
	// Main Game Gameplay
	void InitGame();
	void UpdateGame(float dt);
	PushdownResult GameUpdateFunc(float dt, PushdownState** state);
	void GameAwakeFunc();
	void GameSleepFunc();
	// Main Game Menu
	PushdownResult MainMenuUpdateFunc(float dt, PushdownState** state);
	void MainMenuAwakeFunc();
	void MainMenuSleepFunc();

	// Setting Menu
	PushdownResult SettingMenuUpdateFunc(float dt, PushdownState** state);
	void SettingMenuAwakeFunc();
	void SettingMenuSleepFunc();
	//void InitHUD
	//InitNetworking?
	void exectureTriggers();

	//Tools
	TransformConverter transformConverter;

	//World
	RendererPtr renderer = nullptr;//GameTechRenderer* renderer;
	GameWorld* world = nullptr;
	StateMachine gameStateMachine;
	PushdownMachine pushDownMachine = PushdownMachine(nullptr);
	int winningTeam = -1;
	int Team1Score = 0;
	int Team2Score = 0;

	//Audio
	AudioManager* audioManager = nullptr;

	//Physics
	int maxProxies;
	btVector3 worldAabbMin;
	btVector3 worldAabbMax;
	btAxisSweep3* broadphase = nullptr;
	btDefaultCollisionConfiguration* collisionConfiguration = nullptr;
	btCollisionDispatcher* dispatcher = nullptr;
	btSequentialImpulseConstraintSolver* solver = nullptr;
	btDiscreteDynamicsWorld* dynamicsWorld = nullptr;

	//Custom motion state?

	//GameEntities
	Player* players[4];
	Vector3 spawnPos[4];
	//Wall boundaryWall[4];
	Item* items[36];
	Wall* walls[100]; //Exact number data driven;
	GameEntity* ground = nullptr;

	//Game Assets? Temp
	MeshPtr sphereMesh = nullptr;
	MeshPtr cubeMesh = nullptr;
	MeshPtr capsuleMesh = nullptr;

	TexturePtr basicTex = nullptr;
	ShaderPtr basicShader = nullptr;

	//Controls
	PlayerInput* playerInput[4];
	std::atomic<bool> loading;
	bool hasInit = false;
	bool end = false;

	//GUI
	std::unique_ptr<GameUI> UI;
	Tasks* tasks = nullptr;
	GameMenuPtr gameMenuPtr = nullptr;
	GameMenuPtr gameHUDPtr = nullptr;
	GameMenuPtr debugMenuPtr = nullptr;

	DebugMode* debug;
};