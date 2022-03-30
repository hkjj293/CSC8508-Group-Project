#pragma once
#include "ControlsCommand.h"
#include "Controls.h"
#include "../common/Window.h"
#include <queue>

class PlayerInput {
public:
	//will need deconstructor
	//Check Memory Leaks!
	~PlayerInput() {
		delete spaceBar;
		delete wKey;
		delete sKey;
		delete aKey;
		delete dKey;
		delete leftMouse;
		delete mouseHorizontal;
		delete mouseVertical;
	}

	virtual std::queue<ControlsCommand*>& handleInput() {

		if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::SHIFT)) {
			ControlQueue.push(leftMouse);
		}
		if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::W)) {
			ControlQueue.push(wKey);
		}
		if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::S)) {
			ControlQueue.push(sKey);
		}
		if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::A)) {
			ControlQueue.push(aKey);
		}
		if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::D)) {
			ControlQueue.push(dKey);
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE)) {
			ControlQueue.push(spaceBar);
		}
		if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::T)) {
			ControlQueue.push(tKey);
		}

		if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::G)) {
			ControlQueue.push(gKey);
		}	
		if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::F)) {
			ControlQueue.push(fKey);
		}

		if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::H)) {
			ControlQueue.push(hKey);
		}
		return ControlQueue;
	}

protected:
	ControlsCommand* spaceBar = (ControlsCommand*) new JumpCommand();
	ControlsCommand* wKey = (ControlsCommand*) new moveForwardCommand();
	ControlsCommand* sKey = (ControlsCommand*) new moveBackwardCommand();
	ControlsCommand* aKey = (ControlsCommand*) new moveLeftCommand();
	ControlsCommand* dKey = (ControlsCommand*) new moveRightCommand();

	ControlsCommand* fKey = (ControlsCommand*) new LookLeftCommand();
	ControlsCommand* hKey = (ControlsCommand*) new LookRightCommand();
	ControlsCommand* tKey = (ControlsCommand*) new LookUpCommand();
	ControlsCommand* gKey = (ControlsCommand*) new LookDownCommand();

	ControlsCommand* leftMouse = (ControlsCommand*) new leftMouseCommand();
	ControlsCommand* mouseHorizontal = (ControlsCommand*) new MouseHorizontal();
	ControlsCommand* mouseVertical = (ControlsCommand*) new MouseVertical();
	std::queue<ControlsCommand*> ControlQueue;
};

class PlayerInput2 : public PlayerInput {
public:
	PlayerInput2() : PlayerInput() {}
	~PlayerInput2() {};
	std::queue<ControlsCommand*>& handleInput() override {
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::CONTROL)) {
			ControlQueue.push(spaceBar);
		}
		if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::UP)) {
			ControlQueue.push(wKey);
		}
		if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::DOWN)) {
			ControlQueue.push(sKey);
		}
		if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::LEFT)) {
			ControlQueue.push(aKey);
		}
		if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::RIGHT)) {
			ControlQueue.push(dKey);
		}
		if (Window::GetMouse()->ButtonDown(MouseButtons::LEFT)) {
			ControlQueue.push(leftMouse);
		}
		if (Window::GetMouse()->GetRelativePosition().x) {
			ControlQueue.push(mouseHorizontal);
		}

		if (Window::GetMouse()->GetRelativePosition().y) {
			ControlQueue.push(mouseVertical);
		}
		return ControlQueue;
	}

protected:

};