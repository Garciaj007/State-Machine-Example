#include "../State.h" // Include State Machine Lib

int main()
{
	// Create Our Nodes...
	auto sOpenTransition = MakeRef<BoolEqTransition>(true);
	auto sClosedTransition = MakeRef<BoolEqTransition>(false);

	auto subSetOpenTransition = MakeRef<BoolEqTransition>(true);
	auto subSetClosedTransition = MakeRef<BoolEqTransition>(false);
	auto subSettingBranch = MakeRef<Branch>();

	auto audioTransition = MakeRef<IntEqTransition>(1);
	auto videoTransition = MakeRef<IntEqTransition>(2);
	auto controllerTransition = MakeRef<IntEqTransition>(3);

	auto mainMenuState = MakeRef<State>("Main Menu");
	auto settingsState = MakeRef<State>("Settings");
	auto audioSettingsState = MakeRef<State>("Audio Settings");
	auto videoSettingsState = MakeRef<State>("Video Settings");
	auto controllerSettingsState = MakeRef<State>("Controller Settings");

	// Bind Our Nodes...
	mainMenuState->transitions = { sOpenTransition };
	sOpenTransition->next = settingsState;
	settingsState->transitions = { sClosedTransition, subSetOpenTransition };

	sClosedTransition->next = mainMenuState;

	subSetOpenTransition->next = subSettingBranch;

	subSettingBranch->branches = { audioTransition, videoTransition, controllerTransition };

	audioTransition->next = audioSettingsState;
	videoTransition->next = videoSettingsState;
	controllerTransition->next = controllerSettingsState;

	audioSettingsState->transitions = { subSetClosedTransition };
	videoSettingsState->transitions = { subSetClosedTransition };
	controllerSettingsState->transitions = { subSetClosedTransition };

	subSetClosedTransition->next = settingsState;

	// Setting Initial State
	currentNode = mainMenuState;

	while (true)
	{
		char key = '\0';

		if (_kbhit())
			key = _getch(); // Get Key Input

		if (key == 27) break; // Quit on ESC

		switch (key)
		{
		case 's': sOpenTransition->state = sClosedTransition->state = !sClosedTransition->state;
			break;
		case '1':
		case '2':
		case '3':
		if (sOpenTransition->Evaluate())
		{
			subSetOpenTransition->state = subSetClosedTransition->state = !subSetClosedTransition->state;
			audioTransition->state = videoTransition->state = controllerTransition->state = key - '0';
		}
		break;
		}

		// Update State Machine
		currentNode->On();

		// Prevents wasted CPU Cycles
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}