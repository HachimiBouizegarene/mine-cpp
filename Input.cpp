#include "Input.h"

std::atomic<bool> RenewEngine::Input::keys[static_cast<int>(KeyboardKey::COUNT)] = {false};