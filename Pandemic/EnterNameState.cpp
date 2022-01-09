#include "EnterNameState.h"
#include "MenuStateMachine.h"
#include "GameStateMachine.h"
#include "Main.h"

#define MAX_NAME_LEN	10
const char* EDIT_CHAR_LIST = "_ABCDEFGHIJKLMNOPQRSTUVWXYZ&";

EnterNameState::EnterNameState(GameStateMachine* _statemachine) :
	statemachine(_statemachine),
	charindex(0)
{
}

void EnterNameState::Enter()
{
	screendissolve.Begin();

	// TODO: Temporary fix. Remove when renderers are fixed so that menu can stay open.
	Main::GetMenu().Hide();

	namestr = "";
	charindex = 0;
	renderer.SetName("");
	UpdateEditChar(0);

	// Setup rendering
	Main::GetGraphics().ClearRenderers();
	Main::GetGraphics().AddRenderer(&statemachine->GetBackground());
	Main::GetGraphics().AddRenderer(&renderer);
	Main::GetGraphics().AddRenderer(&screendissolve);
	Main::GetGraphics().AddRenderer(&statemachine->GetScreenMelt());

	Main::GetIO().SendButtonRepeat(true);
	Main::GetButtons().SetAllGameLEDsOn();
}

void EnterNameState::Leave()
{
	Main::GetIO().SendButtonRepeat(false);
	Main::GetButtons().SetAllGameLEDsOff();
}

void EnterNameState::Update()
{
}

bool EnterNameState::HandleMessage(const IOModule_IOMessage& msg)
{
	switch(msg.which_Content)
	{
		case IOModule_IOMessage_LeftButtonPressed_tag:
			UpdateEditChar(-1);
			return true;

		case IOModule_IOMessage_RightButtonPressed_tag:
			UpdateEditChar(1);
			return true;

		case IOModule_IOMessage_AcceptButtonPressed_tag:
			if(EDIT_CHAR_LIST[charindex] == '&')
			{
				// Backspace
				namestr = namestr.Substring(0, namestr.Length() - 1);
				renderer.SetName(namestr);
				UpdateEditChar(0);
			}
			else if(namestr.Length() < (MAX_NAME_LEN - 1))
			{
				// Don't let the player make the first character a space
				if((namestr.Length() > 0) || (EDIT_CHAR_LIST[charindex] != '_'))
				{
					// Add character
					if(EDIT_CHAR_LIST[charindex] == '_')
						namestr += ' ';
					else
						namestr += EDIT_CHAR_LIST[charindex];
					renderer.SetName(namestr);
					charindex = 0;
					UpdateEditChar(0);
				}
			}
			return true;

		case IOModule_IOMessage_CancelButtonPressed_tag:
		{
			Main::GetResources().GetSound("continue.wav").Play();
			GameData& gd = statemachine->GetData();

			// Add the last character
			if((EDIT_CHAR_LIST[charindex] != '_') && (EDIT_CHAR_LIST[charindex] != '&'))
				namestr += EDIT_CHAR_LIST[charindex];

			// Only make a highscores entry when a name has been given
			String finalname = String::Trim(namestr, true, true);
			if(finalname.Length() > 0)
			{
				ScoreRecord result = gd.GetResult();
				result.name = finalname;
				Main::GetScores().Insert(result);
			}

			statemachine->AllowTitleMusicRestart();
			statemachine->GetHighscoreState()->SetGameType(gd.GetType());
			statemachine->ChangeState(statemachine->GetHighscoreState());
			return true;
		}

		default:
			return false;
	}
}

void EnterNameState::UpdateEditChar(int change)
{
	charindex += change;
	int numchars = String(EDIT_CHAR_LIST).Length();

	// Warp around the list of characters
	if(charindex < 0)
		charindex = numchars - 1;
	else if(charindex >= numchars)
		charindex = 0;

	// The & is actually an arrow left to indicate backspace
	// but if this is the first character, then skip over it...
	if((namestr.Length() == 0) && (EDIT_CHAR_LIST[static_cast<uint>(charindex)] == '&'))
	{
		UpdateEditChar((change == 0) ? 1 : change);
		return;
	}

	// Display character
	renderer.SetEditChar(EDIT_CHAR_LIST[static_cast<uint>(charindex)]);
}
