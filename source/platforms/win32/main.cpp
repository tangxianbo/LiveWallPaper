#include <Windows.h>
#include "application.h"

int main(int argc, char *argv[])
{

	Application* app = Application::newInstance();
	app->Init();
	if (app != NULL)
	{
		app->Run();
	}
	Application::deleteInstance();
}