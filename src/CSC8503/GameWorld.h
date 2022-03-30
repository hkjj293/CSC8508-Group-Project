#pragma once
#include "../common/Camera.h"
#include "../game/TransformConverter.h"
#include "../common/RendererBase.h"

#include <vector>
#include <functional>

class GameEntity;
namespace NCL {
		class Camera;
	namespace CSC8503 {
		
		typedef std::function<void(GameEntity*)> GameObjectFunc;
		typedef std::vector<GameEntity*>::const_iterator GameObjectIterator;

		class GameWorld	{
		public:
			GameWorld();
			~GameWorld();

			void Clear();
			void ClearAndErase();

			void AddGameObject(GameEntity* o);
			void RemoveGameObject(GameEntity* o, bool andDelete = false);

			void AddPlayer(GameEntity* o);
			void RemovePlayer(GameEntity* o, bool andDelete = false);

			GameEntity* GetPlayer(int index) {
				if (index < players.size() && index >= 0) {
					return players[index];
				}
				return nullptr;
			}

			void SetRenderer(RendererBase* renderer) {
				this->renderer = renderer;
			}

			RendererBase* GetRenderer() {
				return renderer;
			}

			void AddMainCamera() {
				Camera* c = new Camera();
				mainCameras.push_back(c);
			}

			void RemoveMainCamera(Camera* c, bool andDelete = false) {
				mainCameras.erase(std::remove(mainCameras.begin(), mainCameras.end(), c), mainCameras.end());
				if (andDelete) {
					delete c;
				}
			}

			Camera* GetMainCamera(int i) const {
				if(i < mainCameras.size() && i >= 0)
					return mainCameras[i];
				return nullptr;
			}

			vector<GameEntity*> GetGameObjects() const {
				return gameObjects;
			}

			void ShuffleConstraints(bool state) {
				shuffleConstraints = state;
			}

			void ShuffleObjects(bool state) {
				shuffleObjects = state;
			}

			virtual void UpdateWorld(float dt);

			void UpdatePositions();

			void OperateOnContents(GameObjectFunc f);

			void GetObjectIterators(
				GameObjectIterator& first,
				GameObjectIterator& last) const;

			void SetLocalGame(bool isLocal) { isLocalGame = isLocal; }
			bool IsLocalGame() { return isLocalGame; }

			void SetLocalPlayerCount(int count) { localPlayerCount = count; }
			int GetLocalPlayerCount() { return localPlayerCount; }

		protected:
			std::vector<GameEntity*> gameObjects;
			std::vector<GameEntity*> players;
			RendererBase* renderer;

			std::vector<Camera*> mainCameras;

			bool	shuffleConstraints;
			bool	shuffleObjects;
			bool	isLocalGame;
			int		localPlayerCount;
			int		worldIDCounter;

			TransformConverter transformConverter;
		};
	}
}

