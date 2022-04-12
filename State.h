#pragma once

#include <memory>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <conio.h>
#include <assert.h>

// Alias for std::shared_ptr
template<typename T>
using Ref = std::shared_ptr<T>;

// Alias for std::make_shared
template<typename T, typename... Args>
constexpr Ref<T> MakeRef(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

struct State;
struct Branch;

/// <summary>
/// The Base Node, and is overriden for more conditional and branching functionality.
/// </summary>
struct Node
{
	std::string name;
	Node() = default;
	virtual void On() = 0;
};

/// <summary>
/// The current node. This is also the initial node.
/// </summary>
static Ref<Node> currentNode = nullptr;

/// <summary>
/// Transition switches from one State to the next.
/// </summary>
struct Transition : public Node
{
	Ref<Node> next;

	void On()
	{
		if (Evaluate())
		{
			printf("\nTransitioning to %s", next->name.c_str());
			currentNode = next;
		}
	}
	virtual bool Evaluate() = 0;
};

/// <summary>
/// Make sure that at least one Transition's Condition evaluates.
/// </summary>
struct Branch : public Node
{
	std::vector<Ref<Transition>> branches;

	void On()
	{
		for (auto& branch : branches)
			branch->On();
	}
};

/// <summary>
/// Basic Node
/// Prints out to console...
/// </summary>
struct State : public Node
{
	std::vector<Ref<Transition>> transitions;

	State(const std::string& name) { this->name = name; }
	virtual void On() 
	{ 
		printf("\nOn: %s", name.c_str()); 
		for (auto& transtion : transitions)
			transtion->On();
		//currentNode = next; 
	}
};

///////////////////////////////////
/// Transition Variants
/////////////////////////////////

//* Preprocessor helpers to dynamically generate classes with less code... *//

#define SINGULAR_TRANSITION_HELPER(x, type, defaultState) \
struct x : Transition \
{ \
type condition; type state; \
x(type condition) : condition(condition), state(defaultState) {} \
x(type condition, type initialState) : condition(condition), state(initialState) {} \
virtual bool Evaluate() { return condition == state; } \
};

#define SINGULAR_TRANSITION_HELPER_CONDITION(x, type, defaultState, _condition) \
struct x : Transition \
{ \
type condition; type state; \
x(type condition) : condition(condition), state(defaultState) {} \
x(type condition, type initialState) : condition(condition), state(initialState) {} \
virtual bool Evaluate() { return _condition; } \
};

//* Different Transition Types for differeing conditions *//

SINGULAR_TRANSITION_HELPER(BoolEqTransition, bool, false)
SINGULAR_TRANSITION_HELPER(IntEqTransition, int, 0)
SINGULAR_TRANSITION_HELPER_CONDITION(IntGreaterTransition, int, 0, state > condition)
SINGULAR_TRANSITION_HELPER_CONDITION(IntGreaterEqTransition, int, 0, state >= condition)
SINGULAR_TRANSITION_HELPER_CONDITION(IntLessTransition, int, 0, state < condition)
SINGULAR_TRANSITION_HELPER_CONDITION(IntLessEqTransition, int, 0, state <= condition)
SINGULAR_TRANSITION_HELPER(FloatEqTransition, float, 0.0f)
SINGULAR_TRANSITION_HELPER_CONDITION(FloatGreaterTransition, float, 0.0f, state > condition)
SINGULAR_TRANSITION_HELPER_CONDITION(FloatGreaterEqTransition, float, 0.0f, state >= condition)
SINGULAR_TRANSITION_HELPER_CONDITION(FloatLessTransition, float, 0.0f, state < condition)
SINGULAR_TRANSITION_HELPER_CONDITION(FloatLessEqTransition, float, 0.0f, state <= condition)
SINGULAR_TRANSITION_HELPER(CharEqTransition, char, '\0')