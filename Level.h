#pragma once
#include <vector>
#include <memory>
#include "Object.h"
namespace RenewEngine
{
	class Level {

	public:
		void AddObject(std::unique_ptr<Object> o) {
			m_objects.push_back(std::move(o));
		};

	private:
		std::vector<std::unique_ptr<Object>> m_objects;
	};
}