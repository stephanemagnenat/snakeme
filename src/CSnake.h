/*  ========= CSnake ========= *\
    (c) 1999 Stephane Magnenat
\*  ========================== */

#ifndef __CSNAKE_H
#define __CSNAKE_H

#include <stdlib.h>
#include "CSnakeArray.h"
#include "SGU.h"

typedef struct
{
	int x,y;
} SSnakePos;

const SSnakePos SSPNULL = {0,0};

class CSnake
{
friend class CDeadSnake;
public:
	CSnake();
	~CSnake();
	void Clear(void);
	SSnakePos GetToken(int n) { if (n<length) return snake[n]; else return SSPNULL;}
	SSnakePos GetHead(void) {if (length>0) return snake[0]; else return SSPNULL;}
	SSnakePos GetPreHead(void) {if (length>1) return snake[1]; else return SSPNULL;}
	SSnakePos GetPrePreHead(void) {if (length>2) return snake[2]; else return SSPNULL;}
	SSnakePos GetTail(void) {if (length>0) return snake[length-1];else return SSPNULL; }
	SSnakePos GetPreTail(void) {if (length>1) return snake[length-2];else return SSPNULL; }
	int GetLength(void) { return length; }

	void SetArray(CSnakeArray *ar) { array=ar; }
	void SetSprite(SGU_Sprite *sp) { sprite=sp; }

protected:
	SSnakePos *snake;
	int *snakesprites;
	CSnakeArray *array;
	int length;
public:
	SGU_Sprite *sprite;
};

#endif