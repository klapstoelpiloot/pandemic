#pragma once
#include <thread>
#include <atomic>

class InputHandler final
{
private:

	// Members
	bool exitonenter;
	std::atomic<bool> exitsignal;
	std::atomic<bool> stopthread;
	std::thread* thread;

	// This is the thread that waits for a signal and handles it.
	void Thread();

public:

	// Constructor/destructor
	InputHandler(bool _exitonenter);
	~InputHandler();

	// Methods
	void Start();
	bool IsExitRequested() const { return exitsignal; }
	void Stop();
};
