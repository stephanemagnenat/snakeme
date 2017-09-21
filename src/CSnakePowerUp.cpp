/*  ===== Snake Power Up ===== *\
	Power Up code for SnakeMe
    (c) 2000 Stephane Magnenat
\*  ========================== */

#include "CSnakePowerUp.h"
#include <string.h>

void CSnakeMeal::ComputeNewMealPosition(int *nx,int *ny,CSnakeMeal *other)
{
	*nx=(rand()%(SNAKEARRAYW-2))+1;
	*ny=(rand()%(SNAKEARRAYH-2))+1;
	if (other)
	{
		while ((array->obstaclearray[*nx][*ny]!=-1) || (other->IsAtPos(*nx,*ny)) )
		{
			*nx=(rand()%(SNAKEARRAYW-2))+1;
			*ny=(rand()%(SNAKEARRAYH-2))+1;
		}
	}
	else
	{
		while (array->obstaclearray[*nx][*ny]!=-1)
		{
			*nx=(rand()%(SNAKEARRAYW-2))+1;
			*ny=(rand()%(SNAKEARRAYH-2))+1;
		}
	}
}

bool CSnakeMeal::IsAtPos(int x,int y)
{
	for (int n=0;n<nbmeal;n++)
	{
		if ((x==meals[n].x) && (y==meals[n].y))
		{
			return true;
		}
	}
	return false;
}

bool CSnakeMeal::EatAtPos(int x,int y,SDL_Surface *si)
{
	int tx,ty;
	for (int n=0;n<nbmeal;n++)
	{
		if ((x==meals[n].x) && (y==meals[n].y))
		{
			// compute a position
			ComputeNewMealPosition(&tx,&ty,this);
			meals[n].x=tx;
			meals[n].y=ty;
			// draw
			sprite->Display(si,(tx<<3),(ty<<3)+32,frame);
			return true;
		}
	}
	return false;
}

void CSnakeMeal::DisplaceAMeal(SDL_Surface *si)
{
	timeleft=MEALBYEBYE;
	mealtobereplaced=rand()%nbmeal;
}

void CSnakeMeal::DoStepAndDraw(SDL_Surface *si,CSnakeMeal *other)
{
	if (timeleft==0)
	{
		// delete meal
		int tx,ty;
		// draw bgd at old pos
		array->SingleBackgroundPosDraw(meals[mealtobereplaced].x,meals[mealtobereplaced].y,si);
		// compute a position
		ComputeNewMealPosition(&tx,&ty,other);
		// draw at new pos
		sprite->Display(si,(tx<<3),(ty<<3)+32,frame);
		meals[mealtobereplaced].x=tx;
		meals[mealtobereplaced].y=ty;
		timeleft=-1;
	}
	else if (timeleft>0)
	{
		array->SingleBackgroundPosDraw(meals[mealtobereplaced].x,meals[mealtobereplaced].y,si);
		sprite->DisplayAlpha(si,(meals[mealtobereplaced].x<<3),(meals[mealtobereplaced].y<<3)+32,frame,255-(timeleft*128)/MEALBYEBYE);
		timeleft--;
	}
}

void CSnakeMeal::SetBaseValues(int nbm,SDL_Surface *si,CSnakeArray *ar,SGU_Sprite *spr,int f)
{
	int n,tx,ty;
	nbmeal=nbm;
	if (meals)
		delete meals;
	array=ar;
	sprite=spr;
	frame=f;
	meals=new SSnakePos[nbm]; 
	for (n=0;n<nbmeal;n++)
	{
		// compute a position
		tx=(rand()%(SNAKEARRAYW-10))+4;
		ty=(rand()%(SNAKEARRAYH-10))+4;
		while (array->obstaclearray[tx][ty]!=-1)
		{
			tx=(rand()%(SNAKEARRAYW-10))+4;
			ty=(rand()%(SNAKEARRAYH-10))+4;
		}		
		meals[n].x=tx;
		meals[n].y=ty;
		// draw
		sprite->Display(si,(tx<<3),(ty<<3)+32,frame);
	}
}

void CSnakeMeal::Redraw(SDL_Surface *si)
{
	for (int n=0;n<nbmeal;n++)
	{
		// draw
		sprite->Display(si,(meals[n].x<<3),(meals[n].y<<3)+32,frame);
	}
}


void CSnakeFire::DoStepAndDraw(SDL_Surface *si)
{
	CFireToken *firestmp;
	int nfiretmp,n;

	// garbage collect
	nfiretmp=0;
	firestmp=(CFireToken *)malloc(nfire*sizeof(CFireToken));
	for (n=0;n<nfire;n++)
	{
		array->SingleBackgroundPosDraw(fires[n].x,fires[n].y,si);
		if (fires[n].valid)
		{
			memcpy(&(firestmp[nfiretmp]),&(fires[n]),sizeof(CFireToken));
			nfiretmp++;
		}
	}
	firestmp=(CFireToken *)realloc(firestmp,nfiretmp*sizeof(CFireToken));
	free (fires);
	fires=firestmp;
	nfire=nfiretmp;

	// Draw
	for (n=0;n<nfire;n++)
	{
		fires[n].x+=fires[n].dx;
		fires[n].y+=fires[n].dy;
		sprite->Display(si,fires[n].x<<3,(fires[n].y<<3)+32,7+fires[n].dir);
	}
}

void CSnakeFire::AddFire(int x,int y,int dx,int dy,int o,int dir)
{
	fires=(CFireToken *)realloc(fires,(++nfire)*sizeof(CFireToken));
	fires[nfire-1].x=x;
	fires[nfire-1].y=y;
	fires[nfire-1].dx=dx;
	fires[nfire-1].dy=dy;
	fires[nfire-1].owner=o;
	fires[nfire-1].valid=true;
	fires[nfire-1].dir=dir;
}