#pragma once

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "../Common/RenderObject.h"
#include "../CSC8503/GameTechRenderer.h"
#include "../common/TextureLoader.h"
#include "../Bullet/BulletCollision/CollisionDispatch/btGhostObject.h"

#include "../CSC8503/Transform.h"
#include "AssetsManager.h"

#include <vector>

using std::vector;
using std::string;


class GameEntity {
public:
	GameEntity(string name = "");
	virtual ~GameEntity();

	//Graphics
	RenderObject* GetRenderObject() const {
		return renderObject;
	}

	void SetRenderObject(RenderObject* newObject) {
		renderObject = newObject;
	}

	//Physics
	virtual btRigidBody* GetRigidBody() const {
		return rigidBody;
	}

	virtual void SetRigidBody(btRigidBody* newRigidBody) {
		rigidBody = newRigidBody;
	}

	//General
	virtual Transform& GetTransform(){
		return transform;
	}

	virtual void SetTransform(Transform newtransform) {
		transform = newtransform;
	}

	btTransform& GetbtTransform() {
		return bttransform;
	}

	void SetbtTransform(btTransform newbtTransform) {
		bttransform = newbtTransform;
	}

	void setActive(bool state) {
		isActive = state;
	}

	bool IsActive() const {
		return isActive;
	}

	void SetWorldID(int newID) {
		worldID = newID;
	}

	int	GetWorldID() const {
		return worldID;
	}

	std::string GetName() {
		return name;
	}

	bool getTrigger() {
		return isTrigger;
	}

	void setTrigger(bool state) {
		isTrigger = state;
	}

	btGhostObject* getGhostObject()
	{
		if (isTrigger)
			return ghost;
		else return false;
	}

	virtual void UpdateRenderPositions();

protected:
	RenderObject* renderObject = nullptr;
	btRigidBody* rigidBody = nullptr;
	btGhostObject* ghost = nullptr;

	Transform transform;
	btTransform bttransform;
	TransformConverter transformConverter;
	btQuaternion btRot;
	btVector3 btPos;
	Quaternion nclRot;
	Vector3 nclPos;

	string name;

	bool isActive;
	bool isTrigger;
	bool isStatic;
	int	worldID;
};


