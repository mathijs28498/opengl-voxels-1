#pragma once

#include "Component.h"
#include "System.h"

#include <vector>

class EventSystem : public System {
public:
	EventSystem(std::vector<std::string> requirements) : System(requirements) {}
};