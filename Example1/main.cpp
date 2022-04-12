#include "../State.h" // Include State Machine Lib

int main()
{
	// Create Our Nodes...
	auto lightOnTransition = MakeRef<BoolEqTransition>(true);
	auto lightOffTransition = MakeRef<BoolEqTransition>(false);
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
		char key = '\0';

		if(_kbhit())
			key = _getch(); // Get Key Input

		if (key == 27) break; // Quit on ESC

		// Simple Key Toggle 
		if (key == '1')
		{
			// Setting Light State
			lightOnTransition->state = lightOffTransition->state = !lightOffTransition->state;
		}

		// Update State Machine
		currentNode->On();

		// Prevents wasted CPU Cycles
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}