/*  ======== Snake Array ======== *\
	an array container for SnakeMe
    (c) 2000 Stephane Magnenat
\*  ============================= */

#ifndef __CSNAKEARRAY_H
#define __CSNAKEARRAY_H

#include "SGU.h"

// constants
#define SNAKEARRAYW 80
#define SNAKEARRAYH 56


class CSnakeArray
{
public:
	CSnakeArray();
	~CSnakeArray(); 
	void ClearArray(void);
	void SetYStartPos(int y) { ystartpos=y; }
	bool Load(char *filename,SDL_Surface *si);
	bool Save(char *filename);
	void SetThemeSprite(SGU_Sprite *s) { sprite=s; }
	void FullBackgroundDraw(SDL_Surface *si);
	void SingleBackgroundPosDraw(int x,int y,SDL_Surface *si);
	bool TestForFreeSpace(int x,int y,int r);
private:
	// start y position
	int ystartpos;
	// theme sprite
	SGU_Sprite *sprite;
public:
	// arrays
	unsigned long int bgspritearray[SNAKEARRAYW][SNAKEARRAYH];
	signed long int obstaclearray[SNAKEARRAYW][SNAKEARRAYH];
};

#endif