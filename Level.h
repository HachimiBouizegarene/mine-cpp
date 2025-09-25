#pragma once
#include <vector>
#include <memory>
#include "Object.h"

#include "PLevel.h"

namespace RenewEngine
{
	class Level : RenewEnginePublic::Level {

	public:
		void AddObject(std::unique_ptr<Object> o) {
			m_objects.push_back(std::move(o));
		};

		void Update(ID3D12GraphicsCommandList * commandList) {
			for (std::unique_ptr<Object>& o : m_objects)
			{
				o->Update(commandList);
			}
		};

	private:
		std::vector<std::unique_ptr<Object>> m_objects;
	};
}