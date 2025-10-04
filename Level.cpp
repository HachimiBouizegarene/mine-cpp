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
	if (!cast) throw std::runtime_error("Invalid Object type passed to Level !");
	std::unique_ptr<Object> uCast = std::unique_ptr<Object>(cast);
	Object* ret = uCast.get();
	m_objects.push_back(std::move(uCast));
	return ret;
}
