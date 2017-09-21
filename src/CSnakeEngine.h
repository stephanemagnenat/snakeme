/*  ======= Snake Engine ===== *\
    (c) 1999 Stephane Magnenat
\*  ========================== */

#ifndef __SNAKEENGINE_H
#define __SNAKEENGINE_H

#include "CSnakeArray.h"
#include "CAliveSnake.h"
#include "CDeadSnake.h"
#include "CSnakePowerUp.h"
#include "SGU.h"
#include "SGU_UI.h"
#ifndef NOSOUND
	#include <SDL_mixer.h>
#endif

typedef enum
{
	Aright,
	Aup,
	Aleft,
	Adown,
	Rright,
	Rleft,
	Uaction
} EOrders;

typedef struct
{
	EOrders orders[10];
	int pos;
} SOrderStack;

typedef struct
{
	// snake length
	int defaultlength;
	int incrementlength;	
	
	// meal and bonus
	int numberofmeal;
	int mealdisplacementspeed;
	int numberofbonus;
	int bonusdisplacementspeed;
	int randseed;
	int maxbonus;

	// point contability
	int maxpoint;
	int ptgetmeal;
	int ptgethit;
	int ptgetsnake;
	int ptloosecrash;
	int ptloosehit;
	
	// levels and themes
	char *levelname;
	char *themename;
} SSnakeEngineParam;

typedef struct
{
	int point;
	int bonus;
	int id;
	bool ai;
	SOrderStack orderstack;
	char *ownername;
	CAliveSnake *snake;
} SSnakeAttribute;

class CSnakeEngine
{
public:
	CSnakeEngine();
	~CSnakeEngine();
	void InitEngine(int ns,SDL_Surface *si);
	
	void SetSnakeOwnerName(int ns,char *name) { snakes[ns].ownername=name; }
	void SetSnakeSprite(int ns,SGU_Sprite *spr) { snakes[ns].snake->SetSprite(spr); }
	void SetSnakeMode(int ns,int ident,bool isai) {snakes[ns].id=ident; snakes[ns].ai=isai; }
	void SetSnakeUpcoming(int ns,SDL_Surface *si) { snakes[ns].snake->SetUpcoming(si,0); }
	void SetLittleFont(SGU_Sprite *ltlfnt) { littlefont=ltlfnt; }
	
	void GiveOrder(int ns,EOrders order);
	int DoStepAndDraw(SDL_Surface *si);
	void ClearArray(SDL_Surface *si);
	
	bool IsUpcoming(int n) { return snakes[n].snake->IsUpcoming(); }
	int GetID(int ns) { return snakes[ns].id; }
	char *GetSnakeOwnerName(int ns) { return snakes[ns].ownername; }
	int GetSnakePoint(int ns) { return snakes[ns].point; }
	int GetNumberOfSnake(void) { return nsnake; }
private:
	void DrawBar(SDL_Surface *si);
	bool ClearAndReset(int l);
public:
	SSnakeEngineParam params;
	CUITheme *theme;
private:
	SSnakeAttribute *snakes;
	CSnakeArray *array;
	SGU_Sprite *leveltheme;
	SGU_Sprite *background;
	SGU_Sprite *littlefont;
	int nsnake;
	int mealdisplacementcounter;
	CSnakeMeal *mymeal;
	int bonusdisplacementcounter;
	CSnakeMeal *mybonus;
	int lastselwgt;
	CSnakeFire bullets;
	List DeadSnakeList;
#ifndef NOSOUND
public:
	void SetSound(int n, Mix_Chunk **s) { numbsnd=n; sounds=s; }
private:
	int numbsnd;
	Mix_Chunk **sounds;
#endif
};

#endif