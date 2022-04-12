#pragma once

#include <memory>
#include <vector>
#include <variant>
#include <string>
#include <atomic>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T, typename... Args>
constexpr Ref<T> MakeRef(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

//#define FORWARD_TRANSITION(x) x(Ref<Node> next) : Transition(next) {}
//#define FORWARD_STATE(x) x(const std::string& name, Ref<Node> next): State(name, next) {}

static std::atomic<Ref<char>> keypressed;

void inputFunc()
{
	if(!keypressed.load())
		cin >> keypressed;

}

struct State;
struct Branch;

/// <summary>
/// The Base node that acts as a State, and is overriden for more conditional and branching functionality.
/// </summary>
struct Node
{
	std::string name;
	Ref<Node> next;

	Node() = default;
	//Node(Ref<Node> next) : next(next) {}
	//Node(const std::string& name, Ref<Node> next) : name(name), next(next) {}

	virtual void On() = 0;
};

/// <summary>
/// The current node this is the initial node.
/// </summary>
static Ref<Node> currentNode = nullptr;

/// <summary>
/// Transition switches from on State to the next.
/// </summary>
struct Transition : public Node
{
	//Transition(Ref<Node> next) : Node(next) {}

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
	std::vector<Transition> branches;

	void On()
	{
		printf("\nBranching");
		for (auto& branch : branches)
			branch.On();
	}
};

/// <summary>
/// Basic Node
/// Prints out to console...
/// </summary>
struct State : public Node
{
	State(const std::string& name) { this->name = name; }
	//State(const std::string& name, Ref<Node> next) : Node(name, next) {}
	virtual void On() { printf("\nOn: %s", name.c_str()); currentNode = next; }
};