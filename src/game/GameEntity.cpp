#include "GameEntity.h"


GameEntity::GameEntity(string objectName) {
	name = objectName;
	renderObject = nullptr;
	rigidBody = nullptr;
	ghost = nullptr;
	isActive = true;
	isTrigger = false;
	isStatic = true;
}

GameEntity::~GameEntity() {
	if(renderObject) delete renderObject;
	if(rigidBody) delete rigidBody;
	if(ghost) delete ghost;
}

void GameEntity::UpdateRenderPositions()
{
	//if (isStatic) return;
	//else {
		bttransform = rigidBody->getWorldTransform();

		btRot = bttransform.getRotation();
		btPos = bttransform.getOrigin();

		nclRot = { btRot.getX(), btRot.getY(), btRot.getZ(), btRot.getW() };
		nclPos = { btPos.getX(), btPos.getY(), btPos.getZ() };

		transform.SetOrientation(nclRot);
		transform.SetPosition(nclPos);
	//}
}

