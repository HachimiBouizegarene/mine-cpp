#include "PLevel.h"
#include "Level.h"
#include <memory>
#include <stdexcept>
std::unique_ptr<RenewEnginePublic::Level> RenewEnginePublic::CreateLevel() {
	return std::make_unique<RenewEngine::Level>();
}

RenewEnginePublic::Object* RenewEngine::Level::AddObject(std::unique_ptr<RenewEnginePublic::Object> o)
{
	Object* cast = dynamic_cast<Object*>(o.release());
	if (!cast) std::runtime_error("Invalid Object type passed to Level !");
	m_objects.push_back(std::unique_ptr<Object>(cast));
	return cast;
}
