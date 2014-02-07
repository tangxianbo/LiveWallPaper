#include <Windows.h>
#include "application.h"

int main(int argc, char *argv[])
{
	int screenWidth = atoi(argv[1]);
	int screenHeight = atoi(argv[2]);

	Application* app = Application::newInstance();
	app->Init(screenWidth, screenHeight);
	if (app != NULL)
	{
		app->Run();
	}
	Application::deleteInstance();
}