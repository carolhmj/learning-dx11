#include "systemclass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	SystemClass *system;
	bool result;

	//Create system object
	system = new SystemClass;
	if (!system) {
		return 0;
	}

	//Initialize and run system object
	result = system->initialize();
	if (result) {
		system->run();
	}
	
	//Shutdown and release system object
	system->shutdown();
	delete system;
	system = nullptr;

	return 0;
}