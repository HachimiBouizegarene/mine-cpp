#pragma once

#include <memory>
#include "PObject.h"
namespace RenewEnginePublic
{
	class Level
	{
	public :
		virtual ~Level() = default;
		virtual Object* AddObject(std::unique_ptr<Object> o) = 0;
	};

	std::unique_ptr<Level> CreateLevel();
}