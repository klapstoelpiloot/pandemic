#pragma once
#include "IOModule.pb.h"

class IState
{
	// Make this an interface, do not allow instantiation
public:
	virtual ~IState() = default;
protected:
	IState() { }
	IState(const IState&) { }
	IState& operator = (const IState&) { return *this; }
public:

	// Methods
	virtual void Enter() = 0;
	virtual void Leave() = 0;
	virtual void Update() = 0;
	virtual void HandleMessage(const IOModule_PiMessage& msg) = 0;
};
