#pragma once
#include <stack>

namespace NCL {
	namespace CSC8503 {
		class PushdownState;

		class PushdownMachine
		{
		public:
			PushdownMachine(PushdownState* initState);
			~PushdownMachine();

			//void Update();
			bool Update(float dt);

		protected:
			PushdownState * activeState = nullptr;
			PushdownState * initialState = nullptr;

			std::stack<PushdownState*> stateStack;
		};
	}
}

