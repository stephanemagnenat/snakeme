/*  ======= CAliveSnake ====== *\
    (c) 2000 Stephane Magnenat
\*  ========================== */

#ifndef __CALIVESNAKE_H
#define __CALIVESNAKE_H

#include "CSnake.h"

class CDeadSnake;

class CAliveSnake:public CSnake
{
public:
	CAliveSnake() { tol=0; dir=3; upcoming=true; frozen=false;/*snake=NULL; length=0;*/ }
	~CAliveSnake() { /*if(snake) free(snake); snake=NULL;*/ }
	void Clear(void);
	bool TestCollision(int *type);  // true == collision
	void DoStepAndDraw(SDL_Surface *si);
	bool KillAtPos(int x,int y,CDeadSnake **dead,int lifetime);
	SSnakePos GetNextStep(void);
	void IncreaseLength(int l) {tol=l; }
	void TurnRight(void) {dir=(dir-1)&0x3; }
	void TurnLeft(void) {dir=(dir+1)&0x3; }
	//void SetDir(int n) {if (n!=((dir+2)&0x3)) dir=n; }
	void SetDir(int n);
	int GetDir(void) {return dir; }
	void SetUpcoming(SDL_Surface *si, int t);
	void ClrUpcoming(SDL_Surface *si);
	bool IsUpcoming(void) { return upcoming; }
	bool IsWaiting(void) { return (waittime>0); }
	void Freeze(void) { frozen=true; }
	void UnFreeze(void) { frozen=false; }
	bool IsFrozen(void) { return frozen; }
private:
	int tol;
	int dir;
	int waittime;
	bool upcoming;
	bool frozen;
public:
	int defx,defy;
	int owner;
};

#endif
