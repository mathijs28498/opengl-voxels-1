#pragma once
#include <list>
#include <vector>

template <class T>
class Observer {
public:
	virtual void update(T object) = 0;
};

template <class T0, class T1>
class Observer2 {
public:
	virtual void update(T0 object0, T1 object1) = 0;
};

template<class T>
class Event {
public:
	void notify(T object) {
		for (Observer<T>* obs : callbacks) {
			obs->update(object);
		}
	}

	void operator+=(Observer<T>* observer) {
		callbacks.push_back(observer);
	}

	void operator-=(Observer<T>* observer) {
		callbacks.remove(observer);
	}

private:
	std::list<Observer<T>*> callbacks;
};

template<class T0, class T1>
class Event2 {
public:
	void notify(T0 object0, T1 object1) {
		for (Observer2<T0, T1>* obs : callbacks) {
			obs->update(object0, object1);
		}
	}

	void operator+=(Observer2<T0, T1>* observer) {
		callbacks.push_back(observer);
	}

	void operator-=(Observer2<T0, T1>* observer) {
		callbacks.remove(observer);
	}

private:
	std::list<Observer2<T0, T1>*> callbacks;
};