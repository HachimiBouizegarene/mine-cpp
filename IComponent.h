#pragma once


namespace RenewEngine {
	class IComponent {
	public:
		enum class Type {
			Camera
		};

	public:

		IComponent(Type t) : m_type(t) {};
		virtual void Update() = 0;
		Type GetType() {
			return m_type;
		}
	
	protected:
		Type m_type;
	
	};
}