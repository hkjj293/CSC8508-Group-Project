#pragma once
#include "Bullet.h"
#include "../DebugMode/DebugMode.h"

class  btDiscreteDynamicsWorld;

class BulletPool {
public:
	BulletPool(int team, GameWorld& world, btDiscreteDynamicsWorld& dynamicsWorld) {
		for (int i = 0; i < poolSize; ++i) {
			bullets[i] = new Bullet(team, world, dynamicsWorld);
		}
	};
	~BulletPool();

	void Create(btRigidBody& player, btVector3 force, int lifeTime, Camera& camera);
	void Animate(btRigidBody& player, float dt);

private:
	int bulletNum = 0;
	static const int poolSize = 100;
	Bullet* bullets[poolSize];
};
