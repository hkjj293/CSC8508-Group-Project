#pragma once
#include "../game/Player.h"
#include "../CSC8503/GameWorld.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "../Audio/AudioManager.h"


class ControlsCommand {
public:
	virtual ~ControlsCommand() {}
	virtual void execute(Player& player, Camera& camera, AudioManager& audioManager) = 0;

};