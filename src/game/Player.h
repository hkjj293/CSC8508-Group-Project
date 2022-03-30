#pragma once
#include "GameEntity.h"
#include "../common/TextureLoader.h"
#include "../CSC8503/GameTechRenderer.h"
#include "BulletPool.h"

class Player : public GameEntity {
public:
	Player(Vector3 position, int team, string name, GameWorld& world, btDiscreteDynamicsWorld& dynamicsWorld );
	~Player();

	virtual float GetPitch() {
		return pitch;
	}

	virtual void SetPitch(float p) {
		pitch = p;
	}

	virtual btRigidBody* GetRigidBody() const override{
		return playerRigidBody;
	}

	virtual void SetRigidBody(btRigidBody* newRigidBody) override {
		playerRigidBody = newRigidBody;
	}

	btGeneric6DofConstraint* GetPlayerConstraints() const {
		return playerConstraint;
	}

	virtual Transform& GetTransform() override {
		return transform;
	}

	virtual void SetTransform(Transform newtransform) override {
		transform = newtransform;
	}

	BulletPool* GetBulletPool() const {
		return bullets;
	}

	int GetPlayerTeam() const {
		return playerTeam;
	}

	int GetHealth() {
		return health;
	}

	void OnHeal() {
		health += 1;
		if (health > 100) health = 100;
	}

	void OnDamaged() {
		health -= 2;
		if (health < 0) health = 0;
	}

	virtual void UpdateRenderPositions() override {

		bttransform = playerRigidBody->getWorldTransform();

		btRot = bttransform.getRotation();
		btPos = bttransform.getOrigin();

		nclRot = { btRot.getX(), btRot.getY(), btRot.getZ(), btRot.getW() };
		nclPos = { btPos.getX(), btPos.getY(), btPos.getZ() };

		transform.SetOrientation(nclRot);
		transform.SetPosition(nclPos);

	}


protected:
	void InitAssets();
	//Temp
	MeshPtr playerMesh = nullptr;
	TexturePtr playerTex = nullptr;
	ShaderPtr playerShader = nullptr;

	//general
	int playerTeam;
	int health = 100;
	TransformConverter transformConverter;
	Transform transform;
	btTransform bttransform;
	BulletPool* bullets = nullptr;


	//player Physics
	int playerMass;
	float pitch;
	float playerFriction;
	float playerRestitution;
	btVector3 playerInertia;

	btDefaultMotionState* playerMotion = nullptr;
	btCollisionShape* playerShape = nullptr;
	btGeneric6DofConstraint* playerConstraint = nullptr;
	btRigidBody* playerRigidBody = nullptr;
};
