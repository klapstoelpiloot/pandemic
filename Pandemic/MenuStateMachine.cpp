#include "MenuStateMachine.h"
#include "GameStateMachine.h"
#include "Main.h"

#define IN_GAME_OPACITY		220
#define IN_TITLE_OPACITY	120

MenuStateMachine::MenuStateMachine() :
	topmenustate(this),
	optionsstate(this),
	selectgamestate(this)
{
}

MenuStateMachine::~MenuStateMachine()
{
}

void MenuStateMachine::Update()
{
	StateMachine::Update();
}

void MenuStateMachine::Show()
{
	Main::GetResources().GetSound("confirm.wav").Play();

	// Determine if we're in a game
	IState* currentgamestate = Main::GetGame().GetCurrentState();
	ingamemenu = (currentgamestate == Main::GetGame().GetIntermissionState()) ||
	             (currentgamestate == Main::GetGame().GetPlayingState()) ||
	             (currentgamestate == Main::GetGame().GetRemovePuckState());

	// Determine background dim level
	bool extradim = (currentgamestate != Main::GetGame().GetTitleState());

	// Add to the renderers
	verticalrenderer.SetOpacity(extradim ? IN_GAME_OPACITY : IN_TITLE_OPACITY);
	Main::GetGame().GetTitleState()->GetRenderer()->SetBackgroundMode(true);
	Main::GetGraphics().AddRenderer(&verticalrenderer);

	// Begin with the top menu
	topmenustate.SetBeginPosition(!ingamemenu);
	ChangeState(&topmenustate);
	Main::GetButtons().ShowMenuLEDs(true);
}

void MenuStateMachine::Hide()
{
	Main::GetButtons().ShowMenuLEDs(false);

	// Remove the renderer
	Main::GetGame().GetTitleState()->GetRenderer()->SetBackgroundMode(false);
	Main::GetGraphics().RemoveRenderer(&verticalrenderer);

	// Change to state null
	ChangeState(nullptr);
}

bool MenuStateMachine::HandleMessage(const IOModule_IOMessage& msg)
{
	// If the current state implements IMessageHandler, let it handle the message
	IMessageHandler* handler = dynamic_cast<IMessageHandler*>(currentstate);
	if(handler != nullptr)
		return handler->HandleMessage(msg);
	else
		return false;
}
