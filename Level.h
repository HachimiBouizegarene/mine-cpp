#pragma once
#include <vector>
#include <memory>
#include "Object.h"
#include "PObject.h"
#include "PLevel.h"
#include "CameraComponent.h"

namespace RenewEngine
{
	class Engine;

	class Level : public RenewEnginePublic::Level {

	public:

		RenewEnginePublic::Object* AddObject(std::unique_ptr<RenewEnginePublic::Object> o) override;

		bool SetActiveCamera(RenewEnginePublic::Object* o) {
			CameraComponent* c = o->GetComponent<CameraComponent>();
			if (c)
			{
				m_activeCamera = (Object*)c;
				return true;
			}
			return false;
		};
		RenewEnginePublic::Object* GetActiveCamera() {
			return m_activeCamera;
		};

		void Update() {
			for (std::unique_ptr<Object>& o : m_objects)
			{
				o->Update();
			}
	/*		CameraComponent* c = m_activeCamera->GetComponent<CameraComponent>();*/
		};

	private:
		std::vector<std::unique_ptr<Object>> m_objects;
		Object* m_activeCamera = nullptr;
	};
}