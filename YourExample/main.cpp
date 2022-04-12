#include "../State.h"

int main()
{
	// TODO: Create your State Nodes Here
	// ...


	// TODO: Bind your Nodes Here
	// ...

	// TODO: Set Intial State Here
	currentNode = nullptr;
	assert(currentNode); // Current Node Must be set! 

	while (true)
	{
		char key = '\0';

		if (_kbhit())
			key = _getch(); // Get Key Input

		if (key == 27) break; // Quit on ESC

		// TODO: Program your own state changes here...
		// ...

		// Update State Machine
		currentNode->On();

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}