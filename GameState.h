#ifndef _ASC_GAMESTATE_H_
#define _ASC_GAMESTATE_H_

typedef enum
{
	GS_NONE = 0,
	GS_INIT,
	GS_QUIT
} GAMESTATE;

typedef struct
{
	GAMESTATE State;
	void* Var[256];
} ASC_GameState;

void GameState_Set(GAMESTATE _gs);
void* Game_InitRandomQuads(void);
void Game_DrawRandomQuads(void* _RandomQuad);

#endif //_ASC_GAMESTATE_H_