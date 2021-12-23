#pragma once
#include "StateMachine.h"
#include "IMessageHandler.h"
#include "VerticalMenuRenderer.h"
#include "TitleState.h"
#include "TopMenuState.h"
#include "OptionsMenuState.h"
#include "SelectGameState.h"

class MenuStateMachine final : public virtual StateMachine, virtual public IMessageHandler
{
private:

	// Renderers
	VerticalMenuRenderer verticalrenderer;

	// States
	TopMenuState topmenustate;
	OptionsMenuState optionsstate;
	SelectGameState selectgamestate;
	bool ingamemenu;

public:

	MenuStateMachine();
	virtual ~MenuStateMachine();

	// Renderers
	VerticalMenuRenderer* GetVerticalRenderer() { return &verticalrenderer; }

	// States
	TopMenuState* GetTopMenuState() { return &topmenustate; }
	OptionsMenuState* GetOptionsState() { return &optionsstate; }
	SelectGameState* GetSelectGameState() { return &selectgamestate; }

	// Methods
	virtual bool HandleMessage(const IOModule_IOMessage& msg) override;
	virtual void Update() override;
	void Show();
	void Hide();
	bool IsShown() const { return (GetCurrentState() != nullptr); }
	bool IsInGame() const { return ingamemenu; }
};
