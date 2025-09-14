#include <iostream>
#include "RenewEngine.h"
int main()
{
	HINSTANCE hInstance = GetModuleHandleW(NULL);
	RenewEngine::Engine engine(hInstance);
	engine.Run();
}