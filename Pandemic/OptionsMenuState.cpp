#include "OptionsMenuState.h"
#include "MenuStateMachine.h"
#include "GameStateMachine.h"
#include "Main.h"

#define	VALUE_SPACING	18

OptionsMenuState::OptionsMenuState(MenuStateMachine* _statemachine) :
	statemachine(_statemachine),
	volumeitem("VOLUME:", "%", VALUE_SPACING),
	brightnessitem("BRIGHTNESS:", "%", VALUE_SPACING),
	testitem("SYSTEM TEST"),
	woosh(Main::GetResources().GetSound("woosh1.wav")),
	cancel(Main::GetResources().GetSound("cancel.wav")),
	selectsound(Main::GetResources().GetSound("select.wav")),
	editsound(Main::GetResources().GetSound("change.wav")),
	currentitem(0),
	isediting(false)
{
}

void OptionsMenuState::Enter()
{
	// Set up the items which we show
	horizontalrenderer.Clear();
	itemslist.clear();
	horizontalrenderer.AddItem(&volumeitem);
	horizontalrenderer.AddItem(&brightnessitem);
	itemslist.push_back(OptionsMenuItems::Volume);
	itemslist.push_back(OptionsMenuItems::Brightness);
	if(!statemachine->IsInGame())
	{
		horizontalrenderer.AddItem(&testitem);
		itemslist.push_back(OptionsMenuItems::SystemTest);
	}

	horizontalrenderer.SetAtIndex(0);
	horizontalrenderer.ShowArrows(true);
	currentitem = 0;

	statemachine->GetVerticalRenderer()->AddItem(&horizontalrenderer);

	volumeitem.SetValue(Main::GetAudio().GetVolume());
	brightnessitem.SetValue(Main::GetGraphics().GetBrightness());
	Main::GetButtons().SetAllMenuLEDsOn();
}

void OptionsMenuState::Leave()
{
	statemachine->GetVerticalRenderer()->RemoveItem(&horizontalrenderer);

	// Turn all button LEDs off
	Main::GetButtons().SetAllMenuLEDsOff();
}

void OptionsMenuState::Update()
{
}

bool OptionsMenuState::HandleMessage(const IOModule_IOMessage& msg)
{
	if(isediting)
		return HandleMessageEditing(msg);
	else
		return HandleMessageMenu(msg);
}

bool OptionsMenuState::HandleMessageEditing(const IOModule_IOMessage& msg)
{
	switch(msg.which_Content)
	{
		case IOModule_IOMessage_LeftButtonPressed_tag:
			ChangeItemValue(currentitem, -1);
			editsound.Play();
			return true;

		case IOModule_IOMessage_RightButtonPressed_tag:
			ChangeItemValue(currentitem, 1);
			editsound.Play();
			return true;

		case IOModule_IOMessage_AcceptButtonPressed_tag:
		case IOModule_IOMessage_CancelButtonPressed_tag:
			isediting = false;
			cancel.Play();
			volumeitem.FlashValue(false);
			brightnessitem.FlashValue(false);
			horizontalrenderer.ShowArrows(true);
			return true;

		default:
			return false;
	}
}

bool OptionsMenuState::HandleMessageMenu(const IOModule_IOMessage& msg)
{
	switch(msg.which_Content)
	{
		case IOModule_IOMessage_LeftButtonPressed_tag:
			if(currentitem == 0)
				currentitem = static_cast<int>(itemslist.size()) - 1;
			else
				currentitem--;
			horizontalrenderer.MoveToIndex(currentitem, false);
			woosh.Play();
			return true;

		case IOModule_IOMessage_RightButtonPressed_tag:
			if(currentitem == static_cast<int>(itemslist.size()) - 1)
				currentitem = 0;
			else
				currentitem++;
			horizontalrenderer.MoveToIndex(currentitem, true);
			woosh.Play();
			return true;

		case IOModule_IOMessage_AcceptButtonPressed_tag:
			ActivateItem(currentitem);
			return true;

		case IOModule_IOMessage_CancelButtonPressed_tag:
			statemachine->ChangeState(statemachine->GetTopMenuState());
			cancel.Play();
			return true;

		default:
			return false;
	}
}

void OptionsMenuState::ActivateItem(int index)
{
	// Change the value
	switch(itemslist[index])
	{
		case OptionsMenuItems::Volume:
			isediting = true;
			selectsound.Play();
			horizontalrenderer.ShowArrows(false);
			volumeitem.FlashValue(true);
			break;

		case OptionsMenuItems::Brightness:
			isediting = true;
			selectsound.Play();
			horizontalrenderer.ShowArrows(false);
			brightnessitem.FlashValue(true);
			break;

		case OptionsMenuItems::SystemTest:
			selectsound.Play();
			statemachine->Hide();
			Main::GetGame().ChangeState(Main::GetGame().GetSystemTestState());
			break;

		default:
			NOT_IMPLEMENTED;
	}
}

void OptionsMenuState::ChangeItemValue(int index, int direction)
{
	// Change the value
	switch(itemslist[index])
	{
		case OptionsMenuItems::Volume:
			Main::GetAudio().SetVolume(Main::GetAudio().GetVolume() + direction * 10);
			volumeitem.SetValue(Main::GetAudio().GetVolume());
			break;

		case OptionsMenuItems::Brightness:
			Main::GetGraphics().SetBrightness(Main::GetGraphics().GetBrightness() + direction * 10);
			brightnessitem.SetValue(Main::GetGraphics().GetBrightness());
			break;

		default:
			NOT_IMPLEMENTED;
	}
}
