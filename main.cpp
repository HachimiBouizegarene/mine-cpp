#include <iostream>
#include "RenewEngine.h"
using namespace RenewEnginePublic;

int main()
{
	std::unique_ptr<Engine> engine = CreateRenewEngine();
//
	std::unique_ptr<Level> level = CreateLevel();


	std::unique_ptr<Object> camera = CreateObject();
	CameraComponent* cameraPtr = camera->AddNewComponent<CameraComponent>();
	Object * ob = level->AddObject(std::move(camera));
	engine->AddLevel(std::move(level));
	engine->Run();
}