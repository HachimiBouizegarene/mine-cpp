#pragma once
#include <vector>
#include <memory>
#include "Object.h"
#include "PObject.h"
#include "PLevel.h"

namespace RenewEngine
{
	class Level : public RenewEnginePublic::Level {

	public:
		RenewEnginePublic::Object* AddObject(std::unique_ptr<RenewEnginePublic::Object> o) override;

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