#include "Game.h"
#include "../common/TextureLoader.h"
#include "PlayerInput.h"
#include "LevelGen.h"
#include "Painter.h"
#include "../Bullet/BulletCollision/CollisionDispatch/btGhostObject.h"
#include "../CSC8503/StateMachine.h"
#include "../CSC8503/StateTransition.h"
#include "../CSC8503//GameWorld.h"
#include "../DebugMode/DebugMode.h"

#include <math.h>
#include <thread>
#include <mutex>

//Namespaces?

Game::Game(Tasks* tasks, DebugMode& d) : tasks(tasks) {
	debug = &d;
	int n = 0;
	for (auto i : players) {
		players[n] = nullptr;
		n++;
	}
	n = 0;
	for (auto i : items) {
		items[n] = nullptr;
		n++;
	}
	n = 0;
	for (auto i : walls) {
		walls[n] = nullptr;
		n++;
	}
	for (int i = 0; i < 4; i++) {
		playerInput[i] = nullptr;
	}
	State* gameState = new(Ty<State>()) State([&](float dt)->void {UpdateGame(dt); });
	State* initState = new(Ty<State>()) State([&](float dt)->void { InitWorld();});
	gameStateMachine.AddState(initState);
	gameStateMachine.AddState(gameState);
	gameStateMachine.AddTransition(new(Ty<StateTransition>()) StateTransition(initState, gameState, [&]()->bool {
		InitGame();
		return true;
		}));
}

Game::~Game() {
	//delete Physics
	Game::Destroy();
}

void Game::Init(Tasks* tasks) {
	hasInit = false;
	loading = true;
	tasks->queue([this] {RenderLoading(); });
	//tasks->queue([this] {InitPhysics(); });
	//tasks->queue([this] {InitAudio(); });
	//tasks->queue([this] {InitPlayerInput(); });
	InitAssets();
	DebugMode::runtimeQueue.push(debug->GetFunctionRunTime("InitPhysics()", [this] {InitPhysics(); }));
	DebugMode::runtimeQueue.push(debug->GetFunctionRunTime("InitAudio()", [this] {InitAudio(); }));
	DebugMode::runtimeQueue.push(debug->GetFunctionRunTime("InitPlayerInput()", [this] {InitPlayerInput(); }));
	InitScene();
	LevelGeneration();
	InitCharacter();

	loading = false;
	tasks->waitFinished();
	hasInit = true;
}

void Game::Destroy() {
	delete dynamicsWorld;
	delete broadphase;
	delete collisionConfiguration;
	delete dispatcher;
	delete solver;
	for (int i = 0; i < 4; i++) {
		if(playerInput[i]) delete playerInput[i];
	}
	delete audioManager;
	AssetsManager::UnloadMesh("WallMesh", 0);
	AssetsManager::UnloadMesh("CubeMesh", 0);
	AssetsManager::UnloadMesh("SphereMesh", 0);
	AssetsManager::UnloadMesh("CapsuleMesh", 0);
	AssetsManager::UnloadShader("GameTechShaderSet",0);
	AssetsManager::UnloadTexture("CheckerboardTex", 0);
	AssetsManager::Reset();
	//delete world;
	//renderer.reset();
	//UI.reset();
	//delete GameEntities
	//if(ground) delete ground;
	//for (auto i : players) {
	//	delete i;
	//}

	//for (auto i : items) {
	//	delete i;
	//}

	//for (auto i : walls) {
	//	delete i;
	//}
}

void Game::InitWorld() {
	world = new(Ty<GameWorld>()) GameWorld();
	world->SetLocalGame(true);
	renderer.reset(new(Ty<GameTechRenderer>()) GameTechRenderer(*world));// new GameTechRenderer(*world);
	AssetsManager::SetRenderer(renderer);
	world->SetRenderer(renderer.get());
	InitGUI();
}

void Game::RenderLoading() {
	RendererPtr loadingRenderer;
	loadingRenderer.reset(new(Ty<GameLoadingRenderer>()) GameLoadingRenderer());
	while (loading) {
		loadingRenderer->Render();
		loadingRenderer->NextFrame();
	}
}

void Game::InitGUI() {
	UI.reset(new GameUI());
	UI->Init();
}

void Game::InitAssets() {
	AssetsManager::LoadMeshFromFile("SphereMesh", "Sphere.msh");
	AssetsManager::LoadMeshFromFile("CubeMesh", "Cube.msh");
	AssetsManager::LoadMeshFromFile("WallMesh", "corridor_wall_6.fbx");
	AssetsManager::LoadMeshFromFile("CapsuleMesh", "Capsule.msh");
	AssetsManager::LoadShaderFromFile("GameTechShaderSet", "GameTechShader.set");
	AssetsManager::LoadTextureFromFile("CheckerboardTex", "checkerboard.png");

	sphereMesh = AssetsManager::FetchMesh("SphereMesh");
	cubeMesh = AssetsManager::FetchMesh("CubeMesh");;
	capsuleMesh = AssetsManager::FetchMesh("CapsuleMesh");;

	basicTex = AssetsManager::FetchTexture("CheckerboardTex");
	basicShader = AssetsManager::FetchShader("GameTechShaderSet");
}

void Game::InitPhysics() {
	maxProxies = 1024;
	worldAabbMin = { -1000, -1000, -1000 };
	worldAabbMax = { 1000, 1000, 1000 };
	broadphase = new btAxisSweep3(worldAabbMin, worldAabbMax, maxProxies);

	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	btGhostPairCallback* ghostPair = new btGhostPairCallback();
	dynamicsWorld->getPairCache()->setInternalGhostPairCallback(ghostPair);
}

void Game::InitAudio() {
	audioManager = new(Ty<AudioManager>()) AudioManager();
	audioManager->InitSystem();
}

void Game::InitScene() {
	world->ClearAndErase();
	dynamicsWorld->clearForces();

	//ground
	ground = new(Ty<GameEntity>()) GameEntity("Ground");
	ground->GetTransform()
		.SetPosition(Vector3(0, 0, 0))
		.SetScale(Vector3(70, 1, 70))
		.SetOrientation(Quaternion(0, 0, 0, 1));
	ground->SetRenderObject(new(Ty<RenderObject>()) RenderObject(&ground->GetTransform(), cubeMesh.get(), basicTex.get(), basicShader.get()));
	transformConverter.BTNCLConvert(ground->GetTransform(), ground->GetbtTransform());
	int groundMass = 0;
	btDefaultMotionState* groundMotion = new btDefaultMotionState(ground->GetbtTransform());
	btCollisionShape* groundShape = new btBoxShape({ 35, 0.5, 35 });
	//btCollisionShape* groundShape = new btStaticPlaneShape({ 0, 1, 0 }, 40); //Breaks Renderer static objects btTransforms work differently
	btRigidBody::btRigidBodyConstructionInfo groundCI(groundMass, groundMotion, groundShape, {0, 0, 0});
	ground->SetRigidBody(new btRigidBody(groundCI));
	ground->GetRigidBody()->setFriction(0.5);
	ground->GetRigidBody()->setRestitution(0.5);
	ground->GetRigidBody()->setUserPointer(ground);
	world->AddGameObject(ground);
	dynamicsWorld->addRigidBody(ground->GetRigidBody());

}

/*
void Game::InitItems() {
	items[0] = new Item({ 25, 2, -20 }, 1);
	world->AddGameObject(items[0]);
	dynamicsWorld->addCollisionObject(items[0]->getGhostObject());

	Transform test;
	test.SetPosition({ 25, 2, -15 });
	walls[0] = new Wall(test);
	dynamicsWorld->addCollisionObject(walls[0]->getGhostObject());
	world->AddGameObject(walls[0]);
}
*/

void Game::InitCharacter() {
	int numPLayer = 2;
	int numTeams = 2;

	spawnPos[0] = { 25, 5, 25 };
	spawnPos[1] = { 25, 5, -25 };
	spawnPos[2] = { -25, 5, 25 };
	spawnPos[3] = { -25, 5, -25 };

	world->SetLocalPlayerCount(0);
	for (int i = 0; i < 4; i++) {
		players[i] = new(Ty<Player>()) Player(spawnPos[i], i % numTeams + 1, "", *world, *dynamicsWorld); //Positions set from map data	 
		world->AddPlayer(players[i]);
		if ((world->IsLocalGame() || i == 0) && i < numPLayer) {
			world->SetLocalPlayerCount(world->GetLocalPlayerCount() + 1);
			world->AddMainCamera();
			world->GetMainCamera(i)->SetNearPlane(0.1f); //Graphics - Check planes Positions, can they be default
			world->GetMainCamera(i)->SetFarPlane(1000.0f); //Graphics - Check planes Positions
			world->GetMainCamera(i)->SetDistance(8.0f);

		}
		dynamicsWorld->addRigidBody(players[i]->GetRigidBody(), 16, 0 | 2 | 8);
		world->AddGameObject(players[i]);
	}
	GameHUD* hud = dynamic_cast<GameHUD*>(gameHUDPtr.get());
	if (hud) {
		hud->numPlayer = numPLayer;
		hud->numTeam = numTeams;
	}
}

void Game::InitPlayerInput() {
	playerInput[0] = new(Ty<PlayerInput>()) PlayerInput();
	playerInput[1] = new(Ty<PlayerInput2>()) PlayerInput2();
	playerInput[2] = nullptr;
	playerInput[3] = nullptr;
}

void Game::LevelGeneration() {

	int length = 10;
	int width = 10;

	float scale = 5;

	LevelGen* levelGenerator = new(Ty<LevelGen>()) LevelGen();
	levelGenerator->Generate(length, width);
	vector<string> maze = levelGenerator->GetLevelStrings();

	Transform wallsTransform;
	wallsTransform.SetPosition({ 50,2,0 });
	wallsTransform.SetScale({ scale,scale,scale });
	//wallsTransform.SetOrientation({ 1,0,0,1 });

	Transform stairsTransform;
	stairsTransform.SetScale({ scale, scale ,0.5 });
	//stairsTransform.SetOrientation({ 0.5,0,0,1 });
	stairsTransform.SetPosition({ 10,2,0 });

	Transform floorsTransform;
	floorsTransform.SetScale({ scale, scale ,0.1 });
	//floorsTransform.SetOrientation({ 1,0,0,1 });


	vector<Wall*> vecWalls;
	vector<Wall*> floors;

	float unitLength = scale;
	int numWalls = 0;
	int numFloors = 0;
	for (int i = 0; i < 1; i++)
	{
		for (int level = 0; level < maze.size(); level++)
		{
			for (int l = 0; l < length; l++)
			{
				for (int w = 0; w < width; w++)
				{
					char ch = maze[level][l * width + w];
					Vector3 position({ ((l + 0.5f) * unitLength - 20)  , (level * unitLength) + 3, ((w + 0.5f) * unitLength - 20)  });
					switch (ch)
					{
					case 'P':
						if (level > 0) {
							floorsTransform.SetPosition(position + Vector3(0, -unitLength * .45f, 0));
							floorsTransform.SetScale({ scale, 0.1f, scale });
							floors.push_back(new(Ty<Wall>()) Wall(floorsTransform));
							dynamicsWorld->addCollisionObject(floors[numFloors]->getGhostObject());
							//dynamicsWorld->addRigidBody(floors[numFloors]->GetRigidBody());
							world->AddGameObject(floors[numFloors]);
							numFloors++;
						}
						break;
					case '#':
						wallsTransform.SetPosition(position);
						wallsTransform.SetScale({ scale, scale, scale });
						vecWalls.push_back(new(Ty<Wall>()) Wall(wallsTransform));
						dynamicsWorld->addCollisionObject(vecWalls[numWalls]->getGhostObject());
						//dynamicsWorld->addRigidBody(vecWalls[numWalls]->GetRigidBody());
						world->AddGameObject(vecWalls[numWalls]);
						numWalls++;

						break;
					case 'S':
						break;
					case '<':
						stairsTransform.SetScale({ scale, scale + (scale / 2.5f) ,0.2 });
						stairsTransform.SetOrientation({ 0.42,0,0,1 });
						stairsTransform.SetPosition(position);
						vecWalls.push_back(new(Ty<Wall>()) Wall(stairsTransform));
						world->AddGameObject(vecWalls[numWalls]);
						break;
					case '>':
						stairsTransform.SetScale({ scale, scale + (scale / 2.5f) ,0.2 });
						stairsTransform.SetOrientation({ -0.42,0,0,1 });
						stairsTransform.SetPosition(position);
						vecWalls.push_back(new(Ty<Wall>()) Wall(stairsTransform));
						dynamicsWorld->addCollisionObject(vecWalls[numWalls]->getGhostObject());
						//dynamicsWorld->addRigidBody(vecWalls[numWalls]->GetRigidBody());
						world->AddGameObject(vecWalls[numWalls]);
						numWalls++;
						break;
					case 'V':
						break;
						stairsTransform.SetScale({ scale + (scale / 2.5f), scale ,0.2 });
						stairsTransform.SetOrientation({ 0.39,1,1,0.39 });
						stairsTransform.SetPosition(position);
						vecWalls.push_back(new(Ty<Wall>()) Wall(stairsTransform));
						dynamicsWorld->addCollisionObject(vecWalls[numWalls]->getGhostObject());
						//dynamicsWorld->addRigidBody(vecWalls[numWalls]->GetRigidBody());
						world->AddGameObject(vecWalls[numWalls]);
						numWalls++;
						break;
					case 'A':
						stairsTransform.SetScale({ scale + (scale / 2.5f), scale ,0.2 });
						stairsTransform.SetOrientation({ -0.39,1,1,-0.39 });
						stairsTransform.SetPosition(position);
						vecWalls.push_back(new(Ty<Wall>()) Wall(stairsTransform));
						dynamicsWorld->addCollisionObject(vecWalls[numWalls]->getGhostObject());
						//dynamicsWorld->addRigidBody(vecWalls[numWalls]->GetRigidBody());
						world->AddGameObject(vecWalls[numWalls]);
						numWalls++;
						break;
					}


				}
			}

		}
	}

	Transform collectablesTransform;
	collectablesTransform.SetScale({ scale / 5,scale / 5,scale / 5 });
	//collectablesTransform.SetOrientation({ 1,0,0,1 });

	vector<vector<int>> collectablePos;
	vector<Item*> vecCollectables;
	int numCollectablesPlaced = 0;
	for (int i = 0; i < maze.size(); i++) {

		int numCollectables = ((length * width) / 50);

		for (int x = 0; x < numCollectables; x++) {

			int randomNum = rand() % maze[i].length();

			if (maze[i][randomNum] == 'P') {
				int posLength = randomNum / length;
				int posWidth = randomNum - (posLength * width);

				collectablesTransform.SetPosition({ ((posLength + 0.5f) * unitLength - 20) , (i * unitLength) + 3, ((posWidth + 0.5f) * unitLength - 20) });
				collectablesTransform.SetScale({ 0.1, 0.1, 0.1 });
				vecCollectables.push_back(new(Ty<Item>()) Item(collectablesTransform.GetPosition(),1));
				dynamicsWorld->addCollisionObject(vecCollectables[numCollectablesPlaced]->getGhostObject());
				world->AddGameObject(vecCollectables[numCollectablesPlaced]);
				numCollectablesPlaced++;

			}
			else { x--; }
		}
	}
}

PushdownResult Game::GameUpdateFunc(float dt, PushdownState** state) {
	debug->SetPhysicsInfo(dynamicsWorld->getDispatcher()->getNumManifolds());
	GameHUD* hud = dynamic_cast<GameHUD*>(gameHUDPtr.get());
	if (hud) {
		for (int i = 0; i < 4; i++) {
			hud->hp[i] = (float)(dynamic_cast<Player*>(world->GetPlayer(i))->GetHealth()) / 100.0f;
		}
		hud->team1 = Team1Score;
		hud->team2 = Team2Score;
		hud->AddFPS(1.0f / dt);
		hud->AddMem(debug->GetMemUsed());
		hud->physicsInfo = debug->GetPhysicsInfo();
		for (int i = 0; i < msgLimit; i++) {
			if (debug->memQueue.empty() && DebugMode::msgQueue.empty()) break;
			if (!debug->memQueue.empty()) {
				MemoryInformations info = debug->memQueue.front();
				std::string msg = (std::string)(info.name) + ": " + (std::string)(info.info);
				hud->AddMessage(msg);
				debug->memQueue.pop();
			}
			if (!DebugMode::msgQueue.empty()) {
				hud->AddMessage(DebugMode::msgQueue.front());
				DebugMode::msgQueue.pop();
			}
			if (!DebugMode::runtimeQueue.empty()) {
				hud->AddRunTime(DebugMode::runtimeQueue.front());
				DebugMode::runtimeQueue.pop();
			}
		}
	}
	UI->UpdateUI();
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE)) {
		return PushdownResult::Pop;
	}
	dynamicsWorld->stepSimulation(dt, 0);
	audioManager->AudioUpdate(world, dt);
	for (int i = 0; i < world->GetLocalPlayerCount(); i++) {
		players[i]->GetBulletPool()->Animate(*players[i]->GetRigidBody(), dt);
		world->GetMainCamera(i)->UpdateCamera(players[i]->GetTransform().GetPosition(), players[i]->GetTransform().GetOrientation().ToEuler().y, players[i]->GetPitch(), dt);
		players[i]->GetBulletPool()->Animate(*(players[i]->GetRigidBody()), dt);
		players[i]->GetRigidBody()->setAngularVelocity({ 0,0,0 });
		if (playerInput[i]) {
			std::queue<ControlsCommand*>& command = playerInput[i]->handleInput();
			while (command.size() > 0) {
				command.front()->execute(*players[i], *(world->GetMainCamera(i)), *audioManager); //Learn which player from networking
				command.pop();
			}
		}
	}
	world->UpdatePositions(); //Maybe Change
	exectureTriggers();
	renderer->Update(dt);
	renderer->Render();

	UI->DrawUI();
	renderer->NextFrame();
	if (Team1Score >= 3 ) winningTeam = 1;
	else if(Team2Score >= 3) winningTeam = 2;
	if (winningTeam > 0) {
		PauseMenu* pMenu = dynamic_cast<PauseMenu*>(gameMenuPtr.get());
		if (pMenu) {
			pMenu->winning = winningTeam;
		}
		return PushdownResult::Pop;
	}
	return PushdownResult::NoChange;
}

void Game::GameAwakeFunc() {
	UI->PushMenu(gameHUDPtr);
}

void Game::GameSleepFunc() {
	UI->RemoveMenu(gameHUDPtr);
}

PushdownResult Game::MainMenuUpdateFunc(float dt, PushdownState** state) {
	UI->UpdateUI();
	PauseMenu* pMenu = dynamic_cast<PauseMenu*>(gameMenuPtr.get());
	if (pMenu) {
		if (pMenu->menuClose) {
			PSUpdateFunction up = [&](float dt, PushdownState** st)->PushdownResult {return GameUpdateFunc(dt, st); };
			PSAwakeFunction aw = [&]()->void {GameAwakeFunc(); };
			PSSleepFunction sl = [&]()->void {GameSleepFunc(); };
			PushdownState* s = new(Ty<Item>()) PushdownState(up, aw, sl);
			*state = s;
			return PushdownResult::Push;
		}
		if (pMenu->quitGame) {
			end = true;
			return PushdownResult::Pop;
		}
		if (pMenu->settingLevel) {
			PSUpdateFunction up = [&](float dt, PushdownState** st)->PushdownResult {return SettingMenuUpdateFunc(dt, st); };
			PSAwakeFunction aw = [&]()->void {SettingMenuAwakeFunc(); };
			PSSleepFunction sl = [&]()->void {SettingMenuSleepFunc(); };
			PushdownState* s = new(Ty<PushdownState>())  PushdownState(up, aw, sl);
			*state = s;
			return PushdownResult::Push;
		}
		if (pMenu->mainLevel) {
			if (!hasInit) {
				Init(tasks);
				hasInit = true;
				pMenu->hasInit = true;
			}
			Reset();
			PSUpdateFunction up = [&](float dt, PushdownState** st)->PushdownResult {return GameUpdateFunc(dt, st); };
			PSAwakeFunction aw = [&]()->void {GameAwakeFunc(); };
			PSSleepFunction sl = [&]()->void {GameSleepFunc(); };
			PushdownState* s = new(Ty<PushdownState>())  PushdownState(up, aw, sl);
			*state = s;
			return PushdownResult::Push;
		}
	}
	UI->DrawUI();
	renderer->NextFrame();
	return PushdownResult::NoChange;;
}

void Game::MainMenuAwakeFunc() {
	UI->PushMenu(gameMenuPtr);
}

void Game::MainMenuSleepFunc() {
	UI->RemoveMenu(gameMenuPtr);
}

PushdownResult Game::SettingMenuUpdateFunc(float dt, PushdownState** state) {
	UI->UpdateUI();
	SettingMenu* pMenu = dynamic_cast<SettingMenu*>(debugMenuPtr.get());
	GameHUD* hud = dynamic_cast<GameHUD*>(gameHUDPtr.get());
	if (pMenu) {
		debug->SetDebugMode(pMenu->toggleDebug);
		if (pMenu->back) {
			return PushdownResult::Pop;
		}
		if(hud) hud->debug = pMenu->toggleDebug;
	}
	UI->DrawUI();
	renderer->NextFrame();
	return PushdownResult::NoChange;;
}

void Game::SettingMenuAwakeFunc() {
	UI->PushMenu(debugMenuPtr);
}

void Game::SettingMenuSleepFunc() {
	UI->RemoveMenu(debugMenuPtr);
}

void Game::InitGame() {
	gameMenuPtr.reset(new(Ty<PauseMenu>())  PauseMenu());
	debugMenuPtr.reset(new(Ty<SettingMenu>()) SettingMenu());
	gameHUDPtr.reset(new(Ty<GameHUD>()) GameHUD());
	PSUpdateFunction up = [&](float dt, PushdownState** state)->PushdownResult {return MainMenuUpdateFunc(dt, state); };
	PSAwakeFunction aw = [&]()->void {MainMenuAwakeFunc(); };
	PSSleepFunction sl = [&]()->void {MainMenuSleepFunc(); };
	PushdownState* s = new(Ty<PushdownMachine>()) PushdownState(up,aw,sl);
	pushDownMachine = PushdownMachine(s);
}

void Game::Reset() {
	dynamic_cast<GameTechRenderer*>(renderer.get())->SetTextureInit(false);
}

void Game::UpdateGame(float dt) {
	pushDownMachine.Update(dt);
}



/////////////////Build Level//////////////////////////

/////////////////Other Functions//////////////////////
//void Game::GetPhysicsTestSceneDebugData(std::shared_ptr<DebugMode> d) {
//	d->GetMemoryAllocationSize(*world);
//	d->GetMemoryAllocationSize(*audioManager);
//	d->GetMemoryAllocationSize(*renderer);
//}
void Game::exectureTriggers() {
	GameHUD* hud = dynamic_cast<GameHUD*>(gameHUDPtr.get());
	for (int i = 0; i < world->GetGameObjects().size(); i++) {
		if (world->GetGameObjects()[i]->getTrigger() && world->GetGameObjects()[i]->getGhostObject()->getNumOverlappingObjects()) {
			GameEntity* objA = world->GetGameObjects()[i];
				{
					GameEntity* objB = (GameEntity*)world->GetGameObjects()[i]->getGhostObject()->getOverlappingObject(0)->getUserPointer();
						//Execute triggers
					std::string msg = objA->GetName() + " collided with " + objB->GetName();
					if (hud) { hud->AddPhysicsInfo(msg); }
					if (objA->GetName() == "Item" && objB->GetName() == "Player") {
						Player* team = (Player*)objB;
						if (team->GetPlayerTeam() == 1) {
							Team1Score++;
						}
						else{ 
							Team2Score++;
						}
						audioManager->PlayPickupSound({ objB->GetTransform().GetPosition().x, objB->GetTransform().GetPosition().y , objB->GetTransform().GetPosition().z }, { 0, 0,0 });
						dynamicsWorld->removeCollisionObject(objA->getGhostObject());
						world->RemoveGameObject(objA);
						//dynamicsWorld->removeCollisionObject(world->GetGameObjects()[i]->getGhostObject());
						//world->RemoveGameObject(world->GetGameObjects()[i]);
					}
					//if (objA->GetName() == "Bullet" && objB->GetName() == "Player") {
					//	std::cout << "Player Shot" << std::endl;
					//	Bullet* b = dynamic_cast<Bullet*>(objA);
					//	Player* p = dynamic_cast<Player*>(objB);
					//	if (b->GetPlayerTeam() == p->GetPlayerTeam()) {
					//		p->OnHeal();
					//		//Same Team
					//		//Restore Health
					//		//canControl Bool
					//	}
					//	else{
					//		p->OnDamaged();
					//		//different Team
					//		//reduce health
					//		//canControl Bool	
					//	}	
					//	//David Sound Function
					//	//(Bullet*) world->GetGameObjects()[i]->setFr
					//	//Remove Bullet
					//}
					if (objA->GetName() == "Wall" && objB->GetName() == "Bullet") {
						std::cout << "Wall Painted" << std::endl;
						Bullet* b = dynamic_cast<Bullet*>(objB);
						if(b->paintable) Painter::Paint(objA, objB->GetRenderObject()->GetTransform()->GetPosition());
						//audioManager->PlaySplashSound({ b->GetTransform().GetPosition().x, b->GetTransform().GetPosition().x, b->GetTransform().GetPosition().x }, { 0, 0, 0 });
						//David Sound Function
						//Chris's Painting
					}
					if (objA->GetName() == "Bullet") {
						Bullet* b = dynamic_cast<Bullet*>(objA);
						b->SetFrame(0);
						b->RemoveFromPool();
					}
					if (objB->GetName() == "Bullet") {
						Bullet* b = dynamic_cast<Bullet*>(objB);
						b->SetFrame(0);
						b->RemoveFromPool();
					}
				}	
				for (int i = 0; i < dynamicsWorld->getDispatcher()->getNumManifolds(); i++) {
					btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
					GameEntity* objA = (GameEntity*)contactManifold->getBody0()->getUserPointer();
					GameEntity* objB = (GameEntity*)contactManifold->getBody1()->getUserPointer();
					if (objA->GetName() == "Bullet" && objB->GetName() == "Player") {
						std::cout << "Player Shot" << std::endl;
						Bullet* b = dynamic_cast<Bullet*>(objA);
						Player* p = dynamic_cast<Player*>(objB);

						if (b->GetPlayerTeam() == p->GetPlayerTeam()) {
							p->OnHeal();
						}
						else {
							p->OnDamaged();
							audioManager->PlayHurtSound({ p->GetTransform().GetPosition().x, p->GetTransform().GetPosition().y, p->GetTransform().GetPosition().z }, { 0, 0, 0 });
							//audioManager->PlaySplashSound({ b->GetTransform().GetPosition().x, b->GetTransform().GetPosition().x, b->GetTransform().GetPosition().x }, { 0, 0, 0 });
						}	
						//David Sound Function
						//(Bullet*) world->GetGameObjects()[i]->setFr
						//Remove Bullet
					}
					if (objA->GetName() == "Player" && objB->GetName() == "Bullet") {
						std::cout << "Player Shot" << std::endl;
						Bullet* b = dynamic_cast<Bullet*>(objB);
						Player* p = dynamic_cast<Player*>(objA);

						if (b->GetPlayerTeam() == p->GetPlayerTeam()) {
							p->OnHeal();
						}
						else {
							p->OnDamaged();
							audioManager->PlayHurtSound({ p->GetTransform().GetPosition().x, p->GetTransform().GetPosition().y, p->GetTransform().GetPosition().z }, { 0, 0, 0 });
							//audioManager->PlaySplashSound({ b->GetTransform().GetPosition().x, b->GetTransform().GetPosition().x, b->GetTransform().GetPosition().x }, { 0, 0, 0 });
						}
						//Chris's Painting
					}
					if (objA->GetName() == "Bullet") {
						Bullet* b = dynamic_cast<Bullet*>(objA);
						b->SetFrame(0);
						b->RemoveFromPool();
					}
					if (objB->GetName() == "Bullet") {
						Bullet* b = dynamic_cast<Bullet*>(objB);
						b->SetFrame(0);
						b->RemoveFromPool();
					}
				}
		}
	}
}
/////////////////Other Functions///////////////////////

/////////////////Update Game//////////////////////////
void Game::Update(float dt) {
	gameStateMachine.Update(dt);
}
/////////////////Update Game//////////////////////////


