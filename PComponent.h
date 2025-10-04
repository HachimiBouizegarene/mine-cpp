#pragma once


namespace RenewEnginePublic {
	class Component {
	public:
		virtual ~Component() = default;
		enum class Type : char
		{
			None, Camera, Mesh
		};

		static Type GetStaticType() {
			return Type::None;
		};

		Type GetType()
		{
			return m_type;
		}

		Type m_type;
	};
}