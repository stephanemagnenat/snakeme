/*  ====== SnakeMe Demo ====== *\
    (c) 1999 Stephane Magnenat
\*  ========================== */

#ifndef __SNAKEDEMO_H
#define __SNAKEDEMO_H

#include "SDL.h"

//#define NOSOUND

#ifndef HIWORD
//#define HIWORD(l)   ((WORD) (((DWORD) (l) >> 16) & 0xFFFF)) 
#endif
#ifndef LOWORD
//#define LOWORD(l)   ((WORD) (l)) 
#endif

typedef struct
{
	SDLKey Aright;
	SDLKey Aup;
	SDLKey Aleft;
	SDLKey Adown;
	SDLKey Rright;
	SDLKey Rleft;
	SDLKey Uaction;
} SButtonMap;

typedef struct
{
	int speed;

	int deflng;
	int inclng;

	int nbmeal;
	int mealchangetime;
	int nbbonus;
	int bonuschangetime;
	
	int maxpoint;
	int ptgetmeal;
	int ptgethit;
	int ptgetsnake;
	int ptloosecrash;
	int ptloosehit;
} SAdvancedOptions;

#endif