
#include "Ascencia.h"

static void GameState_InitState(GAMESTATE _gs)
{
	switch (_gs)
	{
		case GS_INIT:
		{
			
		} break;
	}
}

void GameState_Set(GAMESTATE _gs)
{
	if (!State) return;

	State->GameState = _gs;
	GameState_InitState(_gs);
}

void* Game_InitRandomQuads(void)
{
	typedef struct { r32 x; r32 y; r32 r; r32 g; r32 b; } Vertex;
	typedef struct { Vertex Quad[4]; Vertex QuadTarget[4]; r32 Speed; } S_Quad;
	ASC_Alloc(S_Quad, RandomQuad);

	for (r32* r = (r32*)RandomQuad; (u64)r != (u64)RandomQuad + (u64)sizeof(S_Quad); r++)
	{
		*r = Random_r32m(-1.5f, 1.5f);
	}

	RandomQuad->Speed = 0.002f;

	return RandomQuad;
}

void Game_DrawRandomQuads(void* _RandomQuad)
{
	typedef struct { r32 x; r32 y; r32 r; r32 g; r32 b; } Vertex;
	typedef struct { Vertex Quad[4]; Vertex QuadTarget[4]; r32 Speed; } S_Quad;
	S_Quad* RandomQuad = (S_Quad*)_RandomQuad;

	r32 Speed = RandomQuad->Speed;

	if (Input_KeyRepeat(SDLK_UP))
	{
		if (Input_KeyDown(SDLK_LSHIFT)) Speed += 0.01f;
		else Speed += 0.0001f;
		ASC_Log(LOGLEVEL_INFO, "Speed: %.04f", Speed);
	}

	if (Input_KeyRepeat(SDLK_DOWN))
	{
		if (Input_KeyDown(SDLK_LSHIFT)) Speed -= 0.01f;
		else Speed -= 0.0001f;
		ASC_Log(LOGLEVEL_INFO, "Speed: %.04f", Speed);
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glBegin(GL_QUADS);

	for (int i = 0; i < 4; i++)
	{
		if (RandomQuad->Quad[i].r < RandomQuad->QuadTarget[i].r)
		{
			if (RandomQuad->QuadTarget[i].r - RandomQuad->Quad[i].r < Speed)
			{
				RandomQuad->Quad[i].r = RandomQuad->QuadTarget[i].r;
			}
			else
			{
				RandomQuad->Quad[i].r += Speed;
			}
		}
		else if (RandomQuad->Quad[i].r > RandomQuad->QuadTarget[i].r)
		{
			if (RandomQuad->Quad[i].r - RandomQuad->QuadTarget[i].r < Speed)
			{
				RandomQuad->Quad[i].r = RandomQuad->QuadTarget[i].r;
			}
			else
			{
				RandomQuad->Quad[i].r -= Speed;
			}
		}
		else
		{
			RandomQuad->QuadTarget[i].r = Random_r32m(-1.5f, 1.5f);
		}

		if (RandomQuad->Quad[i].g < RandomQuad->QuadTarget[i].g)
		{
			if (RandomQuad->QuadTarget[i].g - RandomQuad->Quad[i].g < Speed)
			{
				RandomQuad->Quad[i].g = RandomQuad->QuadTarget[i].g;
			}
			else
			{
				RandomQuad->Quad[i].g += Speed;
			}
		}
		else if (RandomQuad->Quad[i].g > RandomQuad->QuadTarget[i].g)
		{
			if (RandomQuad->Quad[i].g - RandomQuad->QuadTarget[i].g < Speed)
			{
				RandomQuad->Quad[i].g = RandomQuad->QuadTarget[i].g;
			}
			else
			{
				RandomQuad->Quad[i].g -= Speed;
			}
		}
		else
		{
			RandomQuad->QuadTarget[i].g = Random_r32m(-1.5f, 1.5f);
		}

		if (RandomQuad->Quad[i].b < RandomQuad->QuadTarget[i].b)
		{
			if (RandomQuad->QuadTarget[i].b - RandomQuad->Quad[i].b < Speed)
			{
				RandomQuad->Quad[i].b = RandomQuad->QuadTarget[i].b;
			}
			else
			{
				RandomQuad->Quad[i].b += Speed;
			}
		}
		else if (RandomQuad->Quad[i].b > RandomQuad->QuadTarget[i].b)
		{
			if (RandomQuad->Quad[i].b - RandomQuad->QuadTarget[i].b < Speed)
			{
				RandomQuad->Quad[i].b = RandomQuad->QuadTarget[i].b;
			}
			else
			{
				RandomQuad->Quad[i].b -= Speed;
			}
		}
		else
		{
			RandomQuad->QuadTarget[i].b = Random_r32m(-1.5f, 1.5f);
		}

		if (RandomQuad->Quad[i].x < RandomQuad->QuadTarget[i].x)
		{
			if (RandomQuad->QuadTarget[i].x - RandomQuad->Quad[i].x < Speed)
			{
				RandomQuad->Quad[i].x = RandomQuad->QuadTarget[i].x;
			}
			else
			{
				RandomQuad->Quad[i].x += Speed;
			}
		}
		else if (RandomQuad->Quad[i].x > RandomQuad->QuadTarget[i].x)
		{
			if (RandomQuad->Quad[i].x - RandomQuad->QuadTarget[i].x < Speed)
			{
				RandomQuad->Quad[i].x = RandomQuad->QuadTarget[i].x;
			}
			else
			{
				RandomQuad->Quad[i].x -= Speed;
			}
		}
		else
		{
			RandomQuad->QuadTarget[i].x = Random_r32m(-1.5f, 1.5f);
		}

		if (RandomQuad->Quad[i].y < RandomQuad->QuadTarget[i].y)
		{
			if (RandomQuad->QuadTarget[i].y - RandomQuad->Quad[i].y < Speed)
			{
				RandomQuad->Quad[i].y = RandomQuad->QuadTarget[i].y;
			}
			else
			{
				RandomQuad->Quad[i].y += Speed;
			}
		}
		else if (RandomQuad->Quad[i].y > RandomQuad->QuadTarget[i].y)
		{
			if (RandomQuad->Quad[i].y - RandomQuad->QuadTarget[i].y < Speed)
			{
				RandomQuad->Quad[i].y = RandomQuad->QuadTarget[i].y;
			}
			else
			{
				RandomQuad->Quad[i].y -= Speed;
			}
		}
		else
		{
			RandomQuad->QuadTarget[i].y = Random_r32m(-1.5f, 1.5f);
		}

		glColor3f(RandomQuad->Quad[i].r, RandomQuad->Quad[i].g, RandomQuad->Quad[i].b);
		glVertex2f(RandomQuad->Quad[i].x, RandomQuad->Quad[i].y);
	}

	glEnd();

	RandomQuad->Speed = Speed;
}