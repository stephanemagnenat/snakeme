/*  ===== Snake Power Up ===== *\
	Power Up code for SnakeMe
    (c) 2000 Stephane Magnenat
\*  ========================== */

#ifndef __CSNAKEPOWERUP_H
#define __CSNAKEPOWERUP_H

#include "CSnakeArray.h"
#include "CSnake.h"
#include "SGU.h"
#include "SGU_List.h"

#define MEALBYEBYE 4

class CSnakeMeal
{
public:
	CSnakeMeal() { meals=NULL; timeleft=-1; }
	~CSnakeMeal() {  if (meals) delete meals; }
	bool IsAtPos(int x,int y);
	bool EatAtPos(int x,int y,SDL_Surface *si);
	void SetBaseValues(int nbm,SDL_Surface *si,CSnakeArray *ar,SGU_Sprite *spr,int f);
	SSnakePos GetMealPos(int m) { return meals[m]; }
	int GetNbMeal(void) { return nbmeal; }
	void Redraw(SDL_Surface *si);
	void DisplaceAMeal(SDL_Surface *si);
	void DoStepAndDraw(SDL_Surface *si,CSnakeMeal *other);
	void ComputeNewMealPosition(int *nx,int *ny,CSnakeMeal *other);
private:
	int nbmeal;
	int timeleft;
	int mealtobereplaced;
	SSnakePos *meals;
	CSnakeArray *array;
	SGU_Sprite *sprite;
	int frame;
};

class CFireToken
{
public:
	int x;
	int y;
	int dx;
	int dy;
	int owner;
	int dir;
	bool valid;
public:
	int GetNextX() { return x+dx; }
	int GetNextY() { return y+dy; }
};
// dir : 0:right, 1:top, 2:left, 3:bottom

class CSnakeFire
{
public:
	CSnakeFire() { nfire=0; fires=NULL; }
	~CSnakeFire() {  free (fires); }
	void DoStepAndDraw(SDL_Surface *si);
	void AddFire(int x,int y,int dx,int dy,int o,int dir);
	void SetBaseValues(CSnakeArray *ar,SGU_Sprite *spr) { array=ar; sprite=spr; }
public:
	CFireToken *fires;
	int nfire;
private:
	SGU_Sprite *sprite;
	CSnakeArray *array;
};

#endif