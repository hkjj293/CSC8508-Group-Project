#pragma once
#include <functional>

namespace NCL {
	namespace CSC8503 {

		class State;
		typedef std::function<bool()> StateTransitionFunction;
		class StateTransition
		{
		public:
			StateTransition(State* source, State* dest)
				: sourceState(source)
				, destinationState(dest)
				, function(nullptr)
			{};

			StateTransition(State* source, State* dest, StateTransitionFunction func)
				: sourceState(source)
				, destinationState(dest)
				, function(func)
			{};
			virtual ~StateTransition() {};

			virtual bool CanTransition() const 
			{
				if (function)
					return function();
				return false;
			};

			State* GetDestinationState() const { return destinationState; };
			State* GetSourceState() const { return sourceState; };

		protected:
			State * sourceState;
			State * destinationState;
			StateTransitionFunction function;
		};

		template <class T, class U>
		class GenericTransition : public StateTransition
		{
		public:
			typedef bool(*GenericTransitionFunc)(T, U);
			GenericTransition(GenericTransitionFunc f, T testData, U otherData, State* srcState, State* destState)
				: StateTransition(srcState, destState)
				, func(f)
				, dataA(testData)
				, dataB(otherData)
			{};
			~GenericTransition() {}

			virtual bool CanTransition() const override{
				if (func) {
					return func(dataA, dataB);
				}
				return false;
			}
			static bool GreaterThanTransition(T dataA, U dataB) {
				return dataA > dataB;
			}

			static bool LessThanTransition(T dataA, U dataB) {
				return dataA < dataB;
			}

			static bool EqualsTransition(T dataA, U dataB) {
				return dataA == dataB;
			}

			static bool NotEqualsTransition(T dataA, U dataB) {
				return dataA != dataB;
			}

		protected:
			GenericTransitionFunc  func;
			T dataA;
			U dataB;
		};
	}
}

