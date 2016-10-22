#include <BezierDemonstration.h>

#include <Windows.h>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow) 
{
	BezierDemonstration app;
	app.Run();
	return EXIT_SUCCESS;
}