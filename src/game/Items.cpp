#include "Items.h"


Item::Item(Vector3 position, int score) {
	InitAssets(); //Temp, Replace with loadAsset Class

	name = "Item";
	transform
		.SetPosition(position)
		.SetScale({ 1, 1, 1 })
		.SetOrientation({ 0, 0, 0, 1 });

	this->SetRenderObject(new RenderObject(&transform, itemMesh.get(), itemTex.get(), itemShader.get()));
	transformConverter.BTNCLConvert(transform, bttransform);

	itemScore = score;
	itemShape = new btBoxShape({ 0.5, 0.5, 0.5 });

	ghost = new btGhostObject();
	ghost->setWorldTransform(bttransform);
	ghost->setCollisionShape(itemShape);
	ghost->setCollisionFlags(ghost->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	isTrigger = true;
	ghost->setUserPointer(this);
}

Item::~Item() {
	delete itemMotion;
	delete itemShape;
}

void Item::InitAssets() {
	itemMesh = AssetsManager::FetchMesh("CubeMesh");
	itemTex = AssetsManager::FetchTexture("CheckerboardTex");
	itemShader = AssetsManager::FetchShader("GameTechShaderSet");
}

void Item::OnPlayerCollide() {

}