/*  ======= CDeadSnake ======= *\
    (c) 2000 Stephane Magnenat
\*  ========================== */

#ifndef __CDEADSNAKE_H
#define __CDEADSNAKE_H

#include "CSnake.h"

class CDeadSnake:public CSnake
{
public:
	CDeadSnake(CSnake *snaketemplate,int lifetime);
	CDeadSnake(CSnake *snaketemplate,int lifetime,int pos);
	~CDeadSnake();
	bool DoStepAndDraw(SDL_Surface *si);
private:
	int *snakesprites;
	int lifetimeleft;
	int maxlifetime;
};


#endif