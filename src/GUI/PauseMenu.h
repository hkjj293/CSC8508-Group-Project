#pragma once
#include "GameMenu.h"
#include <string>
#include "../DebugMode/DebugMode.h"

const int runTimeLimit = 1000;
const int msgLimit = 1000;
const int memoryLimit = 100;
const int physicsLimit = 1000;
const int fpsLimit = 100;

namespace NCL {
	namespace CSC8503 {
		class PauseMenu : public GameMenu
		{
		public:
			PauseMenu(){};
			virtual ~PauseMenu() {};

			virtual void Draw() override;
		
			bool hasInit = false;
			bool mainLevel = false;
			bool menuClose = false;
			bool quitGame = false;
			bool settingLevel = false;
			int winning = -1;

		};

		class SettingMenu : public GameMenu
		{
		public:
			SettingMenu() {};
			virtual ~SettingMenu() {};

			virtual void Draw() override;

			std::string debugStr = "";
			bool toggleDebug = false;
			bool back = false;
		};

		class GameHUD : public GameMenu
		{
		public:
			GameHUD() { 
				for (int i = 0; i < fpsLimit; i++) {
					fps[i] = 0;
				}
				for (int i = 0; i < memoryLimit; i++) {
					memoryUsed[i] = 0;
				}
			};
			virtual ~GameHUD() {};
			
			virtual void Draw() override;

			void AddRunTime(std::string s);
			void AddMessage(std::string s);
			void AddFPS(float s);
			void AddMem(float m);
			void AddPhysicsInfo(std::string s);

			int numPlayer = 4;
			int numTeam = 2;
			int team1 = 0;
			int team2 = 0;
			int team3 = 0;
			int team4 = 0;
			float hp[4] = { 0.0f,0.0f,0.0f,0.0f };
			float time = 0.0f;
			bool debug = false;
			std::string physicsInfo;
		private:
			int fpsPos = 0;
			int fpslastPos = 0;
			int memPos = 0;
			int memLastPos = 0;
			int runTimePos = 0;
			int msgPos = 0;
			int msgLength = 0;
			int physicsPos = 0;
			int physicsLength = 0;
			float avgFps = 0;
			float avgMem = 0;
			std::string runTime[runTimeLimit];
			std::string msg[msgLimit];
			std::string physicsMsg[physicsLimit];
			float fps[fpsLimit];
			float memoryUsed[memoryLimit];
		};
	}
}



