#include <BezierDemonstration.h>

#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow) 
{
	BezierDemonstration app;
	app.Run();
	return EXIT_SUCCESS;
}