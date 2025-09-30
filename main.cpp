#include <iostream>
#include "RenewEngine.h"
using namespace RenewEnginePublic;

int main()
{
	std::unique_ptr<Engine> engine = CreateRenewEngine();

	std::unique_ptr<Level> level = CreateLevel();
	engine->AddLevel(std::move(level));
	// engine->Run();
}