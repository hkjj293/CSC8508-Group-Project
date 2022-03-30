#pragma once
#include "../common/Vector3.h"
#include "GameEntity.h"
#include "AssetsManager.h"


class Bullet : public GameEntity{
public:

	Bullet(int team,  GameWorld& world, btDiscreteDynamicsWorld& dynamicsWorld);
	~Bullet();

	void Init(btRigidBody& player, btVector3 force, int lifeTime, Camera& camera, bool paintable);
	void Animate(btRigidBody& player, float dt);
	void InitAssets();
	void RemoveFromPool();

	bool inUse() const { return framesLeft > 0; }

	virtual btRigidBody* GetRigidBody() const override {
		return bulletRigidBody;
	}

	virtual void SetRigidBody(btRigidBody* newRigidBody) override {
		bulletRigidBody = newRigidBody;
	}

	int GetPlayerTeam() const {
		return bulletTeam;
	}

	virtual void UpdateRenderPositions() override {

		bttransform = bulletRigidBody->getWorldTransform();

		btRot = bttransform.getRotation();
		btPos = bttransform.getOrigin();

		nclRot = { btRot.getX(), btRot.getY(), btRot.getZ(), btRot.getW() };
		nclPos = { btPos.getX(), btPos.getY(), btPos.getZ() };

		transform.SetOrientation(nclRot);
		transform.SetPosition(nclPos);
	}

	virtual void SetFrame(float f) { framesLeft = f; }
	bool paintable;
protected:
	float framesLeft;
	float speed;

	MeshPtr bulletMesh = nullptr;
	TexturePtr bulletTex = nullptr;
	ShaderPtr bulletShader = nullptr;

	//Transform transform;
	//btTransform bttransform;
	//TransformConverter transformConverter;

	int bulletTeam;
	int bulletMass;
	btVector3 bulletInertia;
	float bulletFriction;
	float bulletRestitution;

	btDefaultMotionState* bulletMotion = nullptr;
	btCollisionShape* bulletShape = nullptr;
	btRigidBody* bulletRigidBody = nullptr;
};
