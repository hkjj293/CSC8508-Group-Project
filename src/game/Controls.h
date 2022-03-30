#pragma once
#include "ControlsCommand.h"
#include "../Audio/AudioManager.h"

class JumpCommand : ControlsCommand {
public:
	virtual void execute(Player& player, Camera& camera, AudioManager& audioManager) override {
		//Needs on Ground Bool
		player.GetRigidBody()->applyCentralImpulse({ 0, 1000, 0});
		audioManager.SetJump();
	}
};

class moveForwardCommand : ControlsCommand {
public:
	virtual void execute(Player& player, Camera& camera, AudioManager& audioManager) override {
		//player.GetRigidBody()->applyCentralImpulse({ 0, 0, 100 });
		//applyCentralImpulse(player.GetRigidBody()->getWorldTransform().getBasis().getColumn(2) * -1000);
		player.GetRigidBody()->setLinearVelocity(player.GetRigidBody()->getLinearVelocity() + player.GetRigidBody()->getWorldTransform().getBasis().getColumn(2) * -10.0f * Window::GetTimer()->GetTimeDeltaSeconds() );
	}
};

class moveBackwardCommand : ControlsCommand {
public:
	virtual void execute(Player& player, Camera& camera, AudioManager& audioManager) override {
		//player.GetRigidBody()->applyCentralImpulse({ 0, 0, -100 });
		//player.GetRigidBody()->applyCentralImpulse(player.GetRigidBody()->getWorldTransform().getBasis().getColumn(2) * 1000);
		player.GetRigidBody()->setLinearVelocity(player.GetRigidBody()->getLinearVelocity() + player.GetRigidBody()->getWorldTransform().getBasis().getColumn(2) * 10.0f * Window::GetTimer()->GetTimeDeltaSeconds());
	}
};

class moveLeftCommand : ControlsCommand {
public:
	virtual void execute(Player& player, Camera& camera, AudioManager& audioManager) override {
		//player.GetRigidBody()->applyCentralImpulse({ 100, 0, 0 });
		//player.GetRigidBody()->applyCentralImpulse(player.GetRigidBody()->getWorldTransform().getBasis().getColumn(0) * -1000);
		player.GetRigidBody()->setLinearVelocity(player.GetRigidBody()->getLinearVelocity() + player.GetRigidBody()->getWorldTransform().getBasis().getColumn(0) * -10.0f * Window::GetTimer()->GetTimeDeltaSeconds());
	}
};

class moveRightCommand : ControlsCommand {
public:
	virtual void execute(Player& player, Camera& camera, AudioManager& audioManager) override {
		//player.GetRigidBody()->applyCentralImpulse({ -100, 0, 0 });
		//player.GetRigidBody()->applyCentralImpulse(player.GetRigidBody()->getWorldTransform().getBasis().getColumn(0) * 1000);
		player.GetRigidBody()->setLinearVelocity(player.GetRigidBody()->getLinearVelocity() + player.GetRigidBody()->getWorldTransform().getBasis().getColumn(0) * 10.0f * Window::GetTimer()->GetTimeDeltaSeconds());
	}
};

class MouseHorizontal : ControlsCommand {
public:
	virtual void execute(Player& player, Camera& camera, AudioManager& audioManager) override {
		player.GetRigidBody()->setAngularVelocity({ 0, -(Window::GetMouse()->GetRelativePosition().x) , 0 });
	}	
};//Update the mouse by how much

class MouseVertical : ControlsCommand {
public:
	virtual void execute(Player& player, Camera& camera, AudioManager& audioManager) override {
		player.SetPitch(player.GetPitch() - Window::GetMouse()->GetRelativePosition().y);
	}
};//Update the mouse by how much

class LookLeftCommand : ControlsCommand {
public:
	virtual void execute(Player& player, Camera& camera, AudioManager& audioManager) override {
		player.GetRigidBody()->setAngularVelocity({ 0, 1.5708f , 0 });
	}
};

class LookRightCommand : ControlsCommand {
public:
	virtual void execute(Player& player, Camera& camera, AudioManager& audioManager) override {
		player.GetRigidBody()->setAngularVelocity({ 0, -1.5708f , 0 });
	}
};

class LookUpCommand : ControlsCommand {
public:
	virtual void execute(Player& player, Camera& camera, AudioManager& audioManager) override {
		if (player.GetPitch() < 90)
			player.SetPitch(player.GetPitch() + 90.0f * Window::GetTimer()->GetTimeDeltaSeconds());
	}
};

class LookDownCommand : ControlsCommand {
public:
	virtual void execute(Player& player, Camera& camera, AudioManager& audioManager) override {
		if(player.GetPitch() > -90)
			player.SetPitch(player.GetPitch() - 90.0f * Window::GetTimer()->GetTimeDeltaSeconds());
	}
};


class leftMouseCommand : ControlsCommand {
public:
	virtual void execute(Player& player, Camera& camera, AudioManager& audioManager) override {
		//Shoot with freelist
		Vector3 shootPos({ 0, 0, player.GetTransform().GetPosition().z }); //Local Position;
		btVector3 force(shootPos.x, shootPos.y, shootPos.z);
		player.GetBulletPool()->Create(*player.GetRigidBody(), force, 1.0f, camera);

		//std::cout << &world << std::endl;
		//std::cout << &physicsWorld << std::endl;

	}
};