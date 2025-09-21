#pragma once
#include <atomic>

namespace RenewEngine
{
	class IResource {
	public:
		virtual void Bind(ID3D12GraphicsCommandList* commandList) = 0;
		
		bool IsReady()
		{
			return m_ready;
		}

		void SetReady(bool b) { m_ready = b; }

		void MarkReady()
		{
			SetReady(true);
		}

	protected:
		std::atomic<bool> m_ready;
	};
}