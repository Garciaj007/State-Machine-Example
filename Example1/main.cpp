#include "../State.h"

struct BoolTransition : Transition
{
	bool condition;
	bool state;

	BoolTransition(bool condition) : condition(condition), state(false) {}
	BoolTransition(bool condition, bool initialState) : condition(condition), state(initialState) {}

	virtual bool Evaluate() { return condition == state; }
};

int main()
{
	// Create Our Nodes...
	auto lightOnTransition = MakeRef<BoolTransition>(true);
	auto lightOffTransition = MakeRef<BoolTransition>(false);
	auto lightOnState = MakeRef<State>("Light On");
	auto lightOffState = MakeRef<State>("Light Off");

	// Bind Our Nodes
	lightOnState->next = lightOffTransition;
	lightOffTransition->next = lightOffState;
	lightOffState->next = lightOnTransition;
	lightOnTransition->next = lightOnState;

	// Initial State
	currentNode = lightOnState;

	while (true)
	{
		// Get Keyboard Input
		char key = getchar();

		if (key == 'q') break;

		if (key == '1')
		{
			lightOnTransition->state = lightOffTransition->state = !lightOffTransition->state;
		}

		currentNode->On();
	}
}