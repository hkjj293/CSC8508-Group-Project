#pragma once
#include "State.h"

namespace NCL {
	namespace CSC8503 {
		enum class PushdownResult {
			Push, Pop, NoChange
		};
		typedef std::function<PushdownResult(float, PushdownState**)> PSUpdateFunction;
		typedef std::function<void(void)> PSAwakeFunction;
		typedef std::function<void(void)> PSSleepFunction;
		class PushdownState
		{
		public:
			PushdownState(PSUpdateFunction u, PSAwakeFunction a = nullptr, PSSleepFunction s = nullptr) : onUpdate(u), onAwake(a), onSleep(s) {};
			virtual ~PushdownState() {};

			//PushdownResult PushdownUpdate(PushdownState** pushResult) { return PushdownResult::NoChange; };
			virtual PushdownResult OnUpdate(float dt, PushdownState** pushFunc) { 
				if (onUpdate) 
					return onUpdate(dt,pushFunc); 
				return PushdownResult::Pop;
			}
			virtual void OnAwake() { if (onAwake) onAwake(); } //By default do nothing
			virtual void OnSleep() { if (onSleep) onSleep(); } //By default do nothing
		protected:
			PSUpdateFunction onUpdate;
			PSAwakeFunction onAwake;
			PSSleepFunction onSleep;
		};
	}
}

