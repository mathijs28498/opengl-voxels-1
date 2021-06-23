#pragma once
#include <list>
#include "../ECS/Component.h"

class Scene;

class Observer {
public:
	virtual void update(Entity* entity) = 0;
};

class AddEntityEvent {
public:
	void notify(Entity* entity) {
		for (Observer* obs : callbacks) {
			obs->update(entity);
		}
	}

	void operator+=(Observer* observer) {
		callbacks.push_back(observer);
	}

	void operator-=(Observer* observer) {
		callbacks.remove(observer);
	}

private:
	std::list<Observer*> callbacks;
};

extern AddEntityEvent addEntityEvent;