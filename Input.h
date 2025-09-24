#pragma once
#include <atomic>

namespace RenewEngine
{
	class Input
	{public:

		enum class KeyboardKey
		{
			S, W, A, D, COUNT
		};

	
	public:
		static void SetKeyDown(KeyboardKey k) { keys[static_cast<int>(k)] = true; };
		static void SetKeyUp(KeyboardKey k) { keys[static_cast<int>(k)] = false; };
		static bool IsKeyDown(KeyboardKey k) { return keys[static_cast<int>(k)]; };

	private:
		static std::atomic<bool> keys[static_cast<int>(KeyboardKey::COUNT)];
	};
}