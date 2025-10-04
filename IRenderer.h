#pragma once


namespace RenewEngine
{
	class IRenderer
	{
	public:
		virtual ~IRenderer() = default;

		virtual void BeginFrame() = 0;

		virtual void EndFrame() = 0;

	};
}