#include "GameDesc.h"

vector<GameDesc> GAMETYPES
{
	// The casual game is just plain "sjoelen" with relaxed game rules.
	// Puck may be halfway through the gate to score.
	GameDesc(GameType::Casual, "CASUAL", false),

	// Same as classic "sjoelen" but with professional game rules.
	// Puck must be fully inside the gate to score.
	GameDesc(GameType::Pro, "PROFESSIONAL", true),

	// Arcade style gives the player mini games to play.
	//GameDesc(GameType::GID_ARCADE_EASY, "ARCADE EASY"),

	// Arcade style gives the player mini games to play.
	//GameDesc(GameType::GID_ARCADE_PRO, "ARCADE PRO")
};

const GameDesc& GameDesc::Find(GameType type)
{
	for(const GameDesc& gd : GAMETYPES)
	{
		if(gd.id == type)
			return gd;
	}
	FAIL("Requested game type descriptor is not registered");
	return GAMETYPES.front();
}
