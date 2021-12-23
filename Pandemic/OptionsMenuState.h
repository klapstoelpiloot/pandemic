#pragma once
#include "IState.h"
#include "IMessageHandler.h"
#include "Sound.h"
#include "IntEditorRenderer.h"
#include "MenuItemRenderer.h"
#include "HorizontalMenuRenderer.h"

class MenuStateMachine;

enum class OptionsMenuItems
{
	Volume,
	Brightness,
	SystemTest
};

class OptionsMenuState : public virtual IState, public virtual IMessageHandler
{
private:

	MenuStateMachine* statemachine;
	HorizontalMenuRenderer horizontalrenderer;
	IntEditorRenderer volumeitem;
	IntEditorRenderer brightnessitem;
	MenuItemRenderer testitem;
	vector<OptionsMenuItems> itemslist;
	const Sound& woosh;
	const Sound& cancel;
	const Sound& selectsound;
	const Sound& editsound;
	int currentitem;
	bool isediting;

	// Methods
	void ActivateItem(int index);
	void ChangeItemValue(int index, int direction);
	bool HandleMessageEditing(const IOModule_IOMessage& msg);
	bool HandleMessageMenu(const IOModule_IOMessage& msg);

public:

	OptionsMenuState(MenuStateMachine* _statemachine);

	virtual void Enter() override final;
	virtual void Leave() override final;
	virtual void Update() override final;
	virtual bool HandleMessage(const IOModule_IOMessage& msg) override;
};
