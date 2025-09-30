#pragma once
#include "PLevel.h"

namespace RenewEnginePublic
{

	class Engine{
	public:
		virtual ~Engine() = default;
	
		virtual RenewEnginePublic::Level* AddLevel(std::unique_ptr<Level> level) = 0;
		virtual void Run() = 0;
	};

	
	std::unique_ptr<RenewEnginePublic::Engine> CreateRenewEngine();
}