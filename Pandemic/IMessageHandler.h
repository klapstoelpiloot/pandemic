#pragma once
#include "IOModuleLink.h"

class IMessageHandler
{
	// Make this an interface, do not allow instantiation
public:
	virtual ~IMessageHandler() = default;
protected:
	IMessageHandler() { }
	IMessageHandler(const IMessageHandler&) { }
	IMessageHandler& operator = (const IMessageHandler&) { return *this; }
public:

	// Use this handle IO messages. The implementation may pass the call on to another handler and
	// must return True when the message was handled or False when the message was not handler
	// (in which case it may still be handled by parent objects)
	virtual bool HandleMessage(const IOModule_IOMessage& msg) = 0;
};
