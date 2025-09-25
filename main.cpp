#include <iostream>
#include "RenewEngine.h"
using namespace RenewEnginePublic;
int main()
{
	std::unique_ptr<Engine> engine = CreateRenewEngine();
}