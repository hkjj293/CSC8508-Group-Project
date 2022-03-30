#pragma once

#include "../CSC8503/Transform.h"
#include "../Bullet/btBulletCollisionCommon.h"
#include "../Bullet/btBulletDynamicsCommon.h"

class TransformConverter {
public:

	void NCLBTConvert(btTransform& newbtTransform, Transform& newNCLTransform) {

		btRot = newbtTransform.getRotation();
		btPos = newbtTransform.getOrigin();

		nclRot = { btRot.getX(), btRot.getY(), btRot.getZ(), btRot.getW() };
		nclPos = { btPos.getX(), btPos.getY(), btPos.getZ() };

		newNCLTransform.SetOrientation(nclRot);
		newNCLTransform.SetPosition(nclPos);
	}

	 void BTNCLConvert(Transform& newNCLTransform, btTransform& newbtTransform) {
		nclRot = newNCLTransform.GetOrientation();
		nclPos = newNCLTransform.GetPosition();
		
		btRot = { nclRot.x, nclRot.y, nclRot.z, nclRot.w };
		btPos = { nclPos.x, nclPos.y, nclPos.z };

		newbtTransform.setRotation(btRot);
		newbtTransform.setOrigin(btPos);
	}

private:
	 btVector3 btPos;
	 btQuaternion btRot;

	 Vector3 nclPos;
	 Quaternion nclRot;
};
