/*  ======= Snake Engine ===== *\
    (c) 1999 Stephane Magnenat
\*  ========================== */

#include "CSnakeEngine.h"
#include "SnakeDirUtils.h"
#include <stdio.h>

#define MIN_SPACE_FOR_NEW_SNAKE 2
#define MAX_TEST_FOR_NEW_SNAKE 100

extern SGU_Sprite *bigfont;

CSnakeEngine::CSnakeEngine()
{
	nsnake=0;
	array= new CSnakeArray();
	array->SetYStartPos(32);
	theme=NULL;
	mymeal=NULL;
	mybonus=NULL;
	leveltheme=NULL;
}

CSnakeEngine::~CSnakeEngine()
{
	for (lpListNode mynode=DeadSnakeList.GetHead();mynode;mynode=mynode->Next)
	{
		delete (CDeadSnake *)mynode->Element;
	}

	if (nsnake!=0)
	{
		for (int n=0;n<nsnake;n++)
		{
			delete snakes[n].snake;
		}
		free(snakes);
	}
	delete array;
	if (mymeal!=NULL)
		delete mymeal;
	if (mybonus!=NULL)
		delete mybonus;
	if (leveltheme!=NULL) 
		delete leveltheme;
}


void CSnakeEngine::InitEngine(int ns,SDL_Surface *si)
{
	// init random
	srand(params.randseed);
	// set effect sprite
	leveltheme= new SGU_Sprite;
	leveltheme->Load(params.themename,si->format);
	// set array options
	array->SetThemeSprite(leveltheme);
	array->Load(params.levelname,si);
	// set meal options
	if (params.numberofmeal>0)
	{
		mymeal=new CSnakeMeal();
		mymeal->SetBaseValues(params.numberofmeal,si,array,leveltheme,5);
		mealdisplacementcounter=0;
	}
	else
	{
		mymeal=NULL;
	}
	if (params.numberofbonus>0)
	{
		mybonus=new CSnakeMeal();
		mybonus->SetBaseValues(params.numberofbonus,si,array,leveltheme,6);
		bonusdisplacementcounter=0;
	}
	else
	{
		mybonus=NULL;
	}
	// set bonus
	bullets.SetBaseValues(array,leveltheme);
	// set snakes
	nsnake=ns;
	snakes=(SSnakeAttribute *)malloc(nsnake*sizeof(SSnakeAttribute));
	int n;
	for (n=0;n<nsnake;n++)
	{
		snakes[n].snake=new CAliveSnake;
		snakes[n].snake->SetArray(array);
		snakes[n].snake->owner=n;
		snakes[n].point=0;
		snakes[n].bonus=0;
		// check there :
		if (ClearAndReset(n))
		{
			snakes[n].snake->IncreaseLength(params.defaultlength);
			snakes[n].orderstack.pos=0;
		}
		else
		{
			snakes[n].snake->Freeze();
		}
	}
}

void CSnakeEngine::GiveOrder(int ns,EOrders order)
{
	if ((snakes[ns].orderstack.pos<9) && (!snakes[ns].snake->IsWaiting()))
	{
		for (int n=snakes[ns].orderstack.pos;n>0;n--)
		{
			snakes[ns].orderstack.orders[n]=snakes[ns].orderstack.orders[n-1];
		}
		snakes[ns].orderstack.orders[0]=order;
		snakes[ns].orderstack.pos++;
	}
}

void CSnakeEngine::DrawBar(SDL_Surface *si)
{
	int n,x,y,m;
	char ctemp[16];
	char *unadd;

	// draw black to please Gabriel
	SGU_FillRect(si,0,0,640,32,SGU_RGB(0,0,0,0));
	
	// display user snakes
	for (n=0;n<nsnake;n++)
	{
		x=(n%4)*160;
		y=(n>>2)*16;
		
		snakes[n].snake->sprite->Display(si,x,y+2,14);
		for (m=1;m<4;m++)
		{
			snakes[n].snake->sprite->Display(si,x+(m<<3),y+2,2);
		}
		snakes[n].snake->sprite->Display(si,x+32,y+2,18);

		for (unadd=snakes[n].ownername;SGU_GetTextWidth(littlefont,unadd)>70;unadd++);
		SGU_DisplayText(si,littlefont,x+46,y+2,unadd);

		sprintf(ctemp,": %d",snakes[n].point);
		SGU_DisplayText(si,littlefont,x+50+SGU_GetTextWidth(littlefont,unadd),y+2,ctemp);

		for (m=0;m<snakes[n].bonus;m++)
		{
			SGU_Rect(si,x+10+(m*14),y+12,10,2,SGU_RGB(0,0,255,0));
		}
	}
}

bool CSnakeEngine::ClearAndReset(int l)
{
	int ndefx,ndefy;
	int inc=0;

	snakes[l].snake->Clear();
	ndefx=snakes[l].snake->defx=(rand()%(SNAKEARRAYW-6))+3;
	ndefy=snakes[l].snake->defy=(rand()%(SNAKEARRAYH-6))+3;
	while (!array->TestForFreeSpace(ndefx,ndefy,MIN_SPACE_FOR_NEW_SNAKE))
	{
		ndefx=snakes[l].snake->defx=(rand()%(SNAKEARRAYW-6))+3;
		ndefy=snakes[l].snake->defy=(rand()%(SNAKEARRAYH-6))+3;
		if ((++inc)>MAX_TEST_FOR_NEW_SNAKE)
			return false;
	}
	return true;
}

int CSnakeEngine::DoStepAndDraw(SDL_Surface *si)
{

	SSnakePos nextpos;
	int n;
	EOrders order;
	CDeadSnake *deadtemp;

	// Unfreeze
	for (n=0;n<nsnake;n++)
	{
		if (snakes[n].snake->IsFrozen())
		{
			if (ClearAndReset(n))
			{
				snakes[n].snake->UnFreeze();
				snakes[n].snake->SetUpcoming(si,10);
				snakes[n].orderstack.pos=0;
				snakes[n].snake->IncreaseLength(params.defaultlength);				
			}
		}
	}


	// kill already dead members of snake's world
	for (lpListNode mynode=DeadSnakeList.GetHead();mynode;mynode=mynode->Next)
	{
		deadtemp=((CDeadSnake *)(mynode->Element));
		if (!deadtemp->DoStepAndDraw(si))
		{
			mynode=mynode->Next;
			DeadSnakeList.DeleteElement(deadtemp);
			delete deadtemp;		
		}
		if (!mynode)
			break;
	}

	// displace meals
	if (mymeal)
		mymeal->DoStepAndDraw(si,mybonus);
	if (mybonus)
		mybonus->DoStepAndDraw(si,mymeal);

	// do VERY stupid meal displacement because snake world is crazy isn't it ?
	if (mymeal)
	{
		if (mealdisplacementcounter<params.mealdisplacementspeed)
		{
			mealdisplacementcounter++;
		}
		else
		{
			mealdisplacementcounter=0;
			mymeal->DisplaceAMeal(si);
		}
	}
	// same with bonus
	if (mybonus)
	{
		if (bonusdisplacementcounter<params.bonusdisplacementspeed)
		{
			bonusdisplacementcounter++;
		}
		else
		{
			bonusdisplacementcounter=0;
			mybonus->DisplaceAMeal(si);
		}
	}

	// do order
	for (n=0;n<nsnake;n++)
	{
		if (snakes[n].snake->IsFrozen())
			continue;
		if (!snakes[n].ai)
		{
			if (snakes[n].orderstack.pos>0)
			{
				order=snakes[n].orderstack.orders[--snakes[n].orderstack.pos];
				//snakes[n].orderstack.pos--;
				
				if (order==Rright)
				{
					if (snakes[n].snake->IsUpcoming())
					{
						snakes[n].snake->SetDir(0);
						snakes[n].snake->ClrUpcoming(si);
						#ifndef NOSOUND
							Mix_PlayChannel(-1,sounds[0],0);
						#endif
					}
					else
					{
						snakes[n].snake->TurnRight();
					}
				}
				if (order==Rleft)
				{
					if (snakes[n].snake->IsUpcoming())
					{
						snakes[n].snake->SetDir(2);
						snakes[n].snake->ClrUpcoming(si);
						#ifndef NOSOUND
							Mix_PlayChannel(-1,sounds[0],0);
						#endif
					}
					else
					{
						snakes[n].snake->TurnLeft();
					}
				}
				if (order==Aright)
					snakes[n].snake->SetDir(0);
				if (order==Aup)
					snakes[n].snake->SetDir(1);
				if (order==Aleft)
					snakes[n].snake->SetDir(2);
				if (order==Adown)
					snakes[n].snake->SetDir(3);
				
				if (snakes[n].snake->IsUpcoming())
				{
					snakes[n].snake->ClrUpcoming(si);
					#ifndef NOSOUND
						Mix_PlayChannel(-1,sounds[0],0);
					#endif
				}
				if (order==Uaction)
				{
					if (snakes[n].bonus>0)
					{
						snakes[n].bonus--;
						if (((snakes[n].snake->GetDir())==0) || ((snakes[n].snake->GetDir())==2))
						{
							SSnakePos pos;
							pos=snakes[n].snake->GetNextStep();
							bullets.AddFire(pos.x,pos.y,0,1,n,3);
							bullets.AddFire(pos.x,pos.y,0,-1,n,1);
						}
						if (((snakes[n].snake->GetDir())==1) || ((snakes[n].snake->GetDir())==3))
						{
							SSnakePos pos;
							pos=snakes[n].snake->GetNextStep();
							bullets.AddFire(pos.x,pos.y,1,0,n,0);
							bullets.AddFire(pos.x,pos.y,-1,0,n,2);
						}
						// update bar
						DrawBar(si);
						#ifndef NOSOUND
							Mix_PlayChannel(-1,sounds[3],0);
						#endif
					}
				}
			}
		}
		else
		{
			// artificial intelligence
			if (snakes[n].snake->IsUpcoming())
				snakes[n].snake->ClrUpcoming(si);
			int mindistn,mindist;
			int dx,dy;
			static int randomfactor;
			mindist=2000000000;
			mindistn=0;
			for (int z=0;z<mymeal->GetNbMeal();z++)
			{
				dx=mymeal->GetMealPos(z).x-snakes[n].snake->GetHead().x;
				dy=mymeal->GetMealPos(z).y-snakes[n].snake->GetHead().y;
				if ((dx*dx+dy*dy)<mindist)
				{
					mindist=dx*dx+dy*dy;
					mindistn=z;
				}
			}
			dx=mymeal->GetMealPos(mindistn).x-snakes[n].snake->GetHead().x;
			dy=mymeal->GetMealPos(mindistn).y-snakes[n].snake->GetHead().y;
			if (dx>0)
			{
				if (dx>dy)
				{
					snakes[n].snake->SetDir(0);
				}
				else
				{
					snakes[n].snake->SetDir(3);
				}
			}
			else if (dx<0)
			{
				if (dx<dy)
				{
					snakes[n].snake->SetDir(2);
				}
				else
				{
					snakes[n].snake->SetDir(1);
				}
			}
			// set stupid random
			if ((rand()%2)==0)
			{
				for (int i=0;i<4;i++)
				{
					nextpos=snakes[n].snake->GetNextStep();
					if (array->obstaclearray[nextpos.x][nextpos.y]!=-1)
					{
						snakes[n].snake->TurnLeft();
					}
					else
						break;
				}
			}
			else
			{
				for (int i=0;i<4;i++)
				{
					nextpos=snakes[n].snake->GetNextStep();
					if (array->obstaclearray[nextpos.x][nextpos.y]!=-1)
					{
						snakes[n].snake->TurnRight();
					}
					else
						break;
				}
			}
			if ((snakes[n].bonus>0) && ((rand()%16)==0))
			{
				snakes[n].bonus--;
				if (((snakes[n].snake->GetDir())==0) || ((snakes[n].snake->GetDir())==2))
				{
					SSnakePos pos;
					pos=snakes[n].snake->GetNextStep();
					bullets.AddFire(pos.x,pos.y+1,0,1,n,3);
					bullets.AddFire(pos.x,pos.y-1,0,-1,n,1);
				}
				if (((snakes[n].snake->GetDir())==1) || ((snakes[n].snake->GetDir())==3))
				{
					SSnakePos pos;
					pos=snakes[n].snake->GetNextStep();
					bullets.AddFire(pos.x+1,pos.y,1,0,n,0);
					bullets.AddFire(pos.x-1,pos.y,-1,0,n,2);
				}
				// update bar
				DrawBar(si);
				#ifndef NOSOUND
					Mix_PlayChannel(-1,sounds[3],0);
				#endif
			}
		}
	}

	// test collision
	bool iscollision;
	int m,t;
	for (n=0;n<nsnake;n++)
	{
		iscollision=false;
		// detect normal collision
		if (snakes[n].snake->TestCollision(&t))
		{
			if (t>=10)
			{
				// it is a wall
				snakes[n].point-=params.ptloosecrash;
			}
			else
			{
				// it is a snake
				snakes[n].point-=params.ptloosecrash;
				snakes[t].point+=params.ptgetsnake;
			}

			#define DEADTIME 100

			CDeadSnake *tmpsnake=new CDeadSnake((snakes[n].snake),DEADTIME);
			DeadSnakeList.AddElement(tmpsnake);
			if (ClearAndReset(n))
			{
				snakes[n].snake->SetUpcoming(si,10);
				snakes[n].orderstack.pos=0;
				snakes[n].snake->IncreaseLength(params.defaultlength);
			}
			else
			{
				snakes[n].snake->Freeze();
			}
			snakes[n].bonus=0;
			DrawBar(si);
			#ifndef NOSOUND
				Mix_PlayChannel(-1,sounds[5],0);
			#endif
		}
		// detect head to head collision
		for (m=0;m<nsnake;m++)
		{
			if ((m!=n) && (snakes[n].snake->GetNextStep().x==snakes[m].snake->GetNextStep().x) && (snakes[n].snake->GetNextStep().y==snakes[m].snake->GetNextStep().y))
			{
				// apply to first snake
				snakes[m].point-=params.ptloosecrash;
				CDeadSnake *tmpsnake=new CDeadSnake((snakes[m].snake),DEADTIME);
				DeadSnakeList.AddElement(tmpsnake);
				if (ClearAndReset(m))
				{
					snakes[m].snake->SetUpcoming(si,10);
					snakes[m].orderstack.pos=0;
					snakes[m].snake->IncreaseLength(params.defaultlength);
				}
				else
				{
					snakes[m].snake->Freeze();
				}
				// apply to second snake
				snakes[n].point-=params.ptloosecrash;
				tmpsnake=new CDeadSnake((snakes[n].snake),DEADTIME);
				DeadSnakeList.AddElement(tmpsnake);
				if (ClearAndReset(n))
				{
					snakes[n].snake->SetUpcoming(si,10);
					snakes[n].orderstack.pos=0;
					snakes[n].snake->IncreaseLength(params.defaultlength);
				}
				else
				{
					snakes[n].snake->Freeze();
				}
				// update bar
				snakes[n].bonus=0;
				snakes[m].bonus=0;
				DrawBar(si);
				#ifndef NOSOUND
					Mix_PlayChannel(-1,sounds[5],0);
				#endif
			}
		}

	}
	// now get points
	for (n=0;n<nsnake;n++)
	{
		if (snakes[n].point>=params.maxpoint)
		{
			return n;
		}
	}

	// check for meal and bonus
	for (n=0;n<nsnake;n++)
	{
		if (snakes[n].snake->IsFrozen())
			continue;
		if (mymeal)
		{	
			if (mymeal->EatAtPos(snakes[n].snake->GetNextStep().x,snakes[n].snake->GetNextStep().y,si))
			{
				snakes[n].snake->IncreaseLength(params.incrementlength);
				snakes[n].point+=params.ptgetmeal;
				DrawBar(si);
				#ifndef NOSOUND
					Mix_PlayChannel(-1,sounds[1],0);
				#endif
			}
		}
		if (mybonus)
		{
			if (mybonus->EatAtPos(snakes[n].snake->GetNextStep().x,snakes[n].snake->GetNextStep().y,si))
			{
				if (snakes[n].bonus<params.maxbonus)
				{	
					snakes[n].bonus++;
					DrawBar(si);
					#ifndef NOSOUND
						Mix_PlayChannel(-1,sounds[2],0);
					#endif
				}
			}
		}
	}

	// apply effect for each fire token
	int x,y,i;
	for (n=0;n<bullets.nfire;n++)
	{
		// prevent going out of array
		if ((bullets.fires[n].x>SNAKEARRAYW-2)||(bullets.fires[n].y>SNAKEARRAYH-2)||(bullets.fires[n].x<1)||(bullets.fires[n].y<1))
		{
			bullets.fires[n].valid=false;
		}
		// if wall, destroy wall
		else 
		{
			x=bullets.fires[n].GetNextX();
			y=bullets.fires[n].GetNextY();
			i=array->obstaclearray[x][y];
			if (i>=12)
			{
				array->obstaclearray[x][y]=-1;
				array->SingleBackgroundPosDraw(x,y,si);
				bullets.fires[n].valid=false;
			}
			else if (i>=10)
			{
				bullets.fires[n].valid=false;
			}
			else if (i>=0)
			{
				if (snakes[i].snake->KillAtPos(x,y,&deadtemp,DEADTIME))
				{
					// snake is dead		
					if (ClearAndReset(i))
					{
						snakes[i].snake->SetUpcoming(si,10);
						snakes[i].orderstack.pos=0;
						snakes[i].snake->IncreaseLength(params.defaultlength);
					}
					else
					{
						snakes[i].snake->Freeze();
					}
					// update bar
					snakes[i].bonus=0;
					#ifndef NOSOUND
						Mix_PlayChannel(-1,sounds[5],0);
					#endif
				}
				else
				{
					#ifndef NOSOUND
						Mix_PlayChannel(-1,sounds[4],0);
					#endif
				}
				if (deadtemp!=NULL)
				{
					snakes[bullets.fires[n].owner].point+=params.ptgethit;
					snakes[i].point-=params.ptloosehit;
					DeadSnakeList.AddElement(deadtemp);
					DrawBar(si);
				}
				bullets.fires[n].valid=false;
			}
		} 
	}

	// fire
	bullets.DoStepAndDraw(si);

	// do step
	for (n=0;n<nsnake;n++)
	{
		if (!snakes[n].snake->IsFrozen())
			snakes[n].snake->DoStepAndDraw(si);
	}


	// draw circles
	for (n=0;n<nsnake;n++)
	{
		if (snakes[n].snake->IsFrozen())
			continue;
		if (snakes[n].snake->IsUpcoming())
		{
			if (snakes[n].snake->IsWaiting())
			{
				leveltheme->Display(si,snakes[n].snake->defx<<3,(snakes[n].snake->defy<<3)+32,11);
			}
			else
			{
				leveltheme->Display(si,snakes[n].snake->defx<<3,(snakes[n].snake->defy<<3)+32,12);
			}
			snakes[n].snake->sprite->Display(si,snakes[n].snake->defx<<3,(snakes[n].snake->defy<<3)+32,13);
		}
	}

	// return no winner
	return -1;
}

void CSnakeEngine::ClearArray(SDL_Surface *si)
{
	// draw background
	array->FullBackgroundDraw(si);

	// draw bar
	DrawBar(si);

	// draw meal
	if (mymeal)
	{
		mymeal->Redraw(si);
	}

	// draw meal
	if (mybonus)
	{
		mybonus->Redraw(si);
	}
}



