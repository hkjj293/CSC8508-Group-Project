#include "PushdownMachine.h"
#include "PushdownState.h"
using namespace NCL::CSC8503;

PushdownMachine::PushdownMachine(PushdownState* initState)
{
	if (initialState) { delete initialState; }
	initialState = initState;
	activeState = nullptr;
}

PushdownMachine::~PushdownMachine()
{
}

bool PushdownMachine::Update(float dt)
{
	if (activeState)
	{
		PushdownState* newState = nullptr;
		PushdownResult result = activeState->OnUpdate(dt, &newState);
		switch (result)
		{
		case PushdownResult::Pop:
			{
				activeState->OnSleep();
				if(activeState != initialState) delete activeState;
				stateStack.pop();
				if (stateStack.empty())
				{
					activeState = nullptr;
					return false;
				}
				else
				{
					activeState = stateStack.top();
					activeState->OnAwake();
				}
			}
			break;
		case PushdownResult::Push:
			{
				activeState->OnSleep();
				stateStack.push(newState);
				activeState = newState;
				activeState->OnAwake();
			}
			break;
		}
	}
	else
	{
		if (initialState) {
			stateStack.push(initialState);
			activeState = initialState;
		}
		if(activeState) activeState->OnAwake();
	}
	return true;
}

//void PushdownMachine::Update() {
//	if (activeState) {
//		PushdownState* newState = nullptr;
//		PushdownState::PushdownResult result = activeState->PushdownUpdate(&newState);
//
//		switch (result) {
//			case PushdownState::Pop: {
//				activeState->OnSleep();
//				stateStack.pop();
//				if (stateStack.empty()) {
//					activeState = nullptr; //??????
//				}
//				else {
//					activeState = stateStack.top();
//					activeState->OnAwake();
//				}
//			}break;
//			case PushdownState::Push: {
//				activeState->OnSleep();
//				stateStack.push(newState);
//				newState->OnAwake();
//			}break;
//		}
//	}
//}