#include <iostream>
#include <unistd.h>
#include "InputHandler.h"

// Constructor
InputHandler::InputHandler(bool _exitonenter) :
	exitonenter(_exitonenter),
	exitsignal(false),
	stopthread(false),
	thread(nullptr)
{
}

// Destructor
InputHandler::~InputHandler()
{
	// Clean up
	pthread_cancel(thread->native_handle());
	thread->join();
	delete thread;
	thread = nullptr;
}

void InputHandler::Start()
{
	// Create a separate thread to handle input
	thread = new std::thread(&InputHandler::Thread, this);
}

void InputHandler::Stop()
{
	if(!exitsignal && thread->joinable())
	{
		stopthread = true;
		thread->join();
	}
}

// This is the thread that waits for a signal and handles it.
void InputHandler::Thread()
{
	pthread_setname_np(pthread_self(), "InputHandler");
	while(stopthread == false)
	{
		if(exitonenter)
		{
			// Check if ENTER is pressed
			if((std::cin.peek() != std::char_traits<char>::eof()) && !std::cin.eof() && (std::cin.get() == '\n'))
			{
				std::cout << "Enter was pressed." << std::endl;
				exitsignal = true;
				return;
			}
		}
		else
		{
			// Ignore all input
			std::cin.ignore();
		}

		// Sleep 10ms
		usleep(10000);
	}
}

