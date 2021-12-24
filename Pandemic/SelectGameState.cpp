#include "SelectGameState.h"
#include "MenuStateMachine.h"
#include "GameDesc.h"
#include "Main.h"

SelectGameState::SelectGameState(MenuStateMachine* _statemachine) :
	statemachine(_statemachine),
	woosh(Main::GetResources().GetSound("woosh1.wav")),
	cancel(Main::GetResources().GetSound("cancel.wav")),
	accept(Main::GetResources().GetSound("confirm.wav")),
	currentitem(0),
	callback(nullptr)
{
	// Make a renderer for every game type
	for(const GameDesc& gd : GAMETYPES)
	{
		MenuItemRenderer* r = new MenuItemRenderer(gd.name);
		gameitems.push_back(r);
		horizontalrenderer.AddItem(r);
	}
}

SelectGameState::~SelectGameState()
{
	// Clean up renderers
	for(MenuItemRenderer* r : gameitems)
	{
		SAFE_DELETE(r);
	}
}

void SelectGameState::Enter()
{
	horizontalrenderer.SetAtIndex(currentitem);
	horizontalrenderer.ShowArrows(true);
	statemachine->GetVerticalRenderer()->AddItem(&horizontalrenderer);

	// Turn all button LEDs on
	Main::GetButtons().SetAllMenuLEDsOn();
}

void SelectGameState::Leave()
{
	horizontalrenderer.ShowArrows(false);
	statemachine->GetVerticalRenderer()->RemoveItem(&horizontalrenderer);

	// Turn all button LEDs off
	Main::GetButtons().SetAllMenuLEDsOff();
}

void SelectGameState::Update()
{
}

bool SelectGameState::HandleMessage(const IOModule_IOMessage& msg)
{
	switch(msg.which_Content)
	{
		case IOModule_IOMessage_LeftButtonPressed_tag:
			if(currentitem == 0)
				currentitem = static_cast<int>(GAMETYPES.size() - 1);
			else
				currentitem--;
			horizontalrenderer.MoveToIndex(currentitem, false);
			woosh.Play();
			return true;

		case IOModule_IOMessage_RightButtonPressed_tag:
			if(currentitem == static_cast<int>(GAMETYPES.size() - 1))
				currentitem = 0;
			else
				currentitem++;
			horizontalrenderer.MoveToIndex(currentitem, true);
			woosh.Play();
			return true;

		case IOModule_IOMessage_AcceptButtonPressed_tag:
			accept.Play();
			if(callback != nullptr)
				callback(GAMETYPES[currentitem].id);
			return true;

		case IOModule_IOMessage_CancelButtonPressed_tag:
			statemachine->ChangeState(statemachine->GetTopMenuState());
			cancel.Play();
			return true;

		default:
			return false;
	}
}
