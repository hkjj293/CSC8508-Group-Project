#include "BulletPool.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

BulletPool::~BulletPool(){}

void BulletPool::Create(btRigidBody& player, btVector3 force, int lifeTime, Camera& camera) {
	//for (int i = 0; i < poolSize; i++) {
		if (!bullets[bulletNum]->inUse()) {
			bullets[bulletNum]->Init(player, force, lifeTime, camera, (bool)bulletNum%2);
			bulletNum = (bulletNum+1) % poolSize;
			return;
		}
	//}
}

void BulletPool::Animate(btRigidBody& player, float dt) {

	for (int i = 0; i < poolSize; i++) {
		bullets[i]->Animate(player, dt);
	}
}