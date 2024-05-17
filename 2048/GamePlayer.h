#ifndef _GAMEPLAYER_H_
#define _GAMEPLAYER_H_

#include "2048.h"

struct GamePlayer {
	Game* game;
	
	GamePlayer();
	~GamePlayer();
};

#endif