#include "Wall.h"
#include "AssetsManager.h"
#include "../DebugMode/DebugMode.h"

Wall::Wall(Transform buildTransform) {
	InitAssets(); //Temp, Replace with loadAsset Class
	name = "Wall";
	transform = buildTransform;		

	this->SetRenderObject(new(Ty<RenderObject>()) RenderObject(&transform, wallMesh.get(), wallTex.get(), wallShader.get()));
	transformConverter.BTNCLConvert(transform, bttransform);

	Vector3 scale = transform.GetScale();
	wallShape = new btBoxShape({ (scale.x / 2.0f), (scale.y / 2.0f), (scale.z / 2.0f) });
	ghost = new btGhostObject();
	ghost->setCollisionShape(wallShape);
	ghost->setWorldTransform(bttransform);
	ghost->isStaticObject();
	ghost->setUserPointer(this);
	isTrigger = true;
}

Wall::~Wall() {
	delete wallMotion;
	delete wallShape;
	delete ghost;
	AssetsManager::UnloadTexture("WallTex", texID);
}

void Wall::InitAssets() {

	wallMesh = AssetsManager::FetchMesh("WallMesh");
	texID = AssetsManager::LoadTextureFromFile("WallTex","checkerboard.png",false);
	if(texID != -1)
		wallTex = AssetsManager::FetchTexture("WallTex",texID);
	wallTex.get()->Init({ "FBO" });
	wallShader = AssetsManager::FetchShader("GameTechShaderSet");
}

