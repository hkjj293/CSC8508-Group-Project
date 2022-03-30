#pragma once	
#include "GameMenu.h"

class PhysicsTestScene;

namespace NCL {
	namespace CSC8503 {
		class MainMenu : public GameMenu
		{
		public:
			MainMenu() {};
			virtual ~MainMenu() {};

			virtual void Draw() override;
		protected:
		};

	}
}


