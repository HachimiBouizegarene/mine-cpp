#pragma once



namespace RenewEnginePublic
{

	class Engine{
		virtual void Run() = 0;
	};

	std::unique_ptr<RenewEnginePublic::Engine> CreateRenewEngine();
}