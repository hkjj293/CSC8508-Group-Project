#ifndef BT_DEFAULT_MOTION_STATE_H
#define BT_DEFAULT_MOTION_STATE_H

#include "btMotionState.h"
#include "../CSC8503/Transform.h"

using namespace NCL;
using namespace CSC8503;

///The btDefaultMotionState provides a common implementation to synchronize world transforms with offsets.
ATTRIBUTE_ALIGNED16(struct)
btDefaultMotionState : public btMotionState
{
	btTransform m_graphicsWorldTrans;
	btTransform m_centerOfMassOffset;
	btTransform m_startWorldTrans;
	void* m_userPointer;

	Transform nclTransform;

	BT_DECLARE_ALIGNED_ALLOCATOR();

	btDefaultMotionState(const btTransform& startTrans = btTransform::getIdentity(), const btTransform& centerOfMassOffset = btTransform::getIdentity())
		: m_graphicsWorldTrans(startTrans),
		  m_centerOfMassOffset(centerOfMassOffset),
		  m_startWorldTrans(startTrans),
		  m_userPointer(0)

	{
	}

	///synchronizes world transform from user to physics
	virtual void getWorldTransform(btTransform & centerOfMassWorldTrans) const
	{
		centerOfMassWorldTrans = m_graphicsWorldTrans * m_centerOfMassOffset.inverse();
	}

	///synchronizes world transform from physics to user
	///Bullet only calls the update of worldtransform for active objects
	virtual void setWorldTransform(const btTransform& centerOfMassWorldTrans)
	{
		m_graphicsWorldTrans = centerOfMassWorldTrans * m_centerOfMassOffset;
	}

	Transform getNCLTransform() const {
		return nclTransform;
	}

	void setNCLTransform(Transform newNClTransform) {
		nclTransform = newNClTransform;
	}

	void ConvertbtTransform() {

		btQuaternion btRot = m_graphicsWorldTrans.getRotation();
		btVector3 btPos = m_graphicsWorldTrans.getOrigin();

		Quaternion nclRot = nclTransform.GetOrientation();
		Vector3 nclPos = nclTransform.GetPosition();

		nclRot.x = btRot.x();
		nclRot.y = btRot.y();
		nclRot.z = btRot.z();
		nclRot.w = btRot.w();

		nclPos.x = btPos.x();
		nclPos.y = btPos.y();
		nclPos.z = btPos.z();

		nclTransform.SetOrientation(nclRot);
		nclTransform.SetPosition(nclPos);
	}
};

#endif  //BT_DEFAULT_MOTION_STATE_H
