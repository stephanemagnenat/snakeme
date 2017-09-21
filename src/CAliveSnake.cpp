/*  ======= CAliveSnake ====== *\
    (c) 2000 Stephane Magnenat
\*  ========================== */

#include "CAliveSnake.h"
#include "CDeadSnake.h"
#include "SnakeDirUtils.h"

void CAliveSnake::Clear(void)
{
	// Note : not clean, call super class
	if (snake)
		free(snake);
	snake=NULL;
	if (snakesprites)
		free(snakesprites);
	snakesprites=NULL;
	length=0;
	tol=1;
	dir=3;
}


bool CAliveSnake::TestCollision(int *type)
{
	SSnakePos nextpos;
	int t;
	if (frozen)
		return false;

	// test collision
	nextpos=GetNextStep();
	t=array->obstaclearray[nextpos.x][nextpos.y];
	if (t!=-1)
	{
		*type=t;
		return true;
	}
	return false;
}

void CAliveSnake::DoStepAndDraw(SDL_Surface *si)
{
	int n,dx,dy,dx2,dy2;
	int frame;
	
	// if frozen, exit
	if (frozen)
		return;

	// we only consider existing state
	if (upcoming)
	{
		if (waittime>0)
			waittime--;
		return;
	}

	// delete tail
	if ((length>0) && (tol==0))
	{
		array->SingleBackgroundPosDraw(GetTail().x,GetTail().y,si);
		array->obstaclearray[GetTail().x][GetTail().y]=-1;
	}

	// increase length
	if (tol>0)
	{
		length++;
		tol--;
		if (snake!=NULL)
		{
			snake=(SSnakePos *)realloc(snake,length*sizeof(SSnakePos));
			snakesprites=(int *)realloc(snakesprites,length*sizeof(int));
		}
		else
		{
			snake=(SSnakePos *)malloc(length*sizeof(SSnakePos));
			snakesprites=(int *)malloc(length*sizeof(int));
			snake[0].x=defx;
			snake[0].y=defy;
			array->obstaclearray[defx][defy]=owner;
		}
	}
	for (n=length-2;n>=0;n--)
	{
		snake[n+1]=snake[n];
		snakesprites[n+1]=snakesprites[n];
	}
	if (length>1)
	{
		snake[0].x=snake[1].x+DirtoDx(dir);
		snake[0].y=snake[1].y+DirtoDy(dir);
	}

	// allocate place for head
	array->obstaclearray[GetHead().x][GetHead().y]=owner;

	//#define TESTALPHA 0
	// redraw body
	if (GetLength()==1) 
	{
		// we only have a head to draw
		frame=GetDir()+12;
		array->SingleBackgroundPosDraw(GetHead().x,GetHead().y,si);
		sprite->Display(si,(GetHead().x<<3),(GetHead().y<<3)+32,frame);
		snakesprites[0]=frame;
		//sprite->DisplayAlpha(si,(GetHead().x<<3),(GetHead().y<<3)+16,GetDir()+12,TESTALPHA);
	}
	else if (GetLength()==2)
	{
		// we have a head and a tail to draw
		// draw tail
		array->SingleBackgroundPosDraw(GetTail().x,GetTail().y,si);
		dx=GetHead().x-GetTail().x;
		dy=GetHead().y-GetTail().y;
		frame=DxytoDir(dx,dy)+16;
		sprite->Display(si,(GetTail().x<<3),32+(GetTail().y<<3),frame);
		snakesprites[1]=frame;
		//sprite->DisplayAlpha(si,(GetTail().x<<3),16+(GetTail().y<<3),DxytoDir(dx,dy)+16,TESTALPHA);

		// draw head
		frame=GetDir()+12;
		array->SingleBackgroundPosDraw(GetHead().x,GetHead().y,si);
		sprite->Display(si,(GetHead().x<<3),(GetHead().y<<3)+32,frame);
		snakesprites[0]=frame;
		//sprite->DisplayAlpha(si,(GetHead().x<<3),(GetHead().y<<3)+16,GetDir()+12,TESTALPHA);
	}
	else
	{
		// we have a head and a tail to draw and a body token
		// draw tail
		dx=GetPreTail().x-GetTail().x;
		dy=GetPreTail().y-GetTail().y;
		frame=DxytoDir(dx,dy)+16;
		array->SingleBackgroundPosDraw(GetTail().x,GetTail().y,si);
		sprite->Display(si,(GetTail().x<<3),32+(GetTail().y<<3),frame);
		snakesprites[length-1]=frame;
		//sprite->DisplayAlpha(si,(GetTail().x<<3),16+(GetTail().y<<3),DxytoDir(dx,dy)+32,TESTALPHA);

		// draw head
		frame=GetDir()+12;
		array->SingleBackgroundPosDraw(GetHead().x,GetHead().y,si);
		sprite->Display(si,(GetHead().x<<3),(GetHead().y<<3)+32,frame);
		snakesprites[0]=frame;
		//sprite->DisplayAlpha(si,(GetHead().x<<3),(GetHead().y<<3)+32,GetDir()+12,TESTALPHA);
		// display rest of body
		// 2= actual, 1=next (1-2)
		dx=GetHead().x-GetPreHead().x;
		dy=GetHead().y-GetPreHead().y;
		// 2= actual, 1=previous (1-2)
		dx2=GetPrePreHead().x-GetPreHead().x;
		dy2=GetPrePreHead().y-GetPreHead().y;
		frame=NLtoI(DxytoDir(dx,dy),DxytoDir(dx2,dy2));
		array->SingleBackgroundPosDraw(GetPreHead().x,GetPreHead().y,si);
		sprite->Display(si,(GetPreHead().x<<3),32+(GetPreHead().y<<3),frame);
		snakesprites[1]=frame;
		//sprite->DisplayAlpha(si,(GetPreHead().x<<3),32+(GetPreHead().y<<3),is,TESTALPHA);
	}
}

bool CAliveSnake::KillAtPos(int x,int y,CDeadSnake **dead,int lifetime)
{
	if (frozen)
		return false;

	int n;
	n=length-1;
	while (n>=0)
	{
		if ((snake[n].x!=x) || (snake[n].y!=y))
		{
			n--;
		}
		else
		{
			break;
		}
	}
	// if we haven't touch
	if (n==-1)
	{
		*dead=NULL;
		return false;
	}
	// now we have in n the index of the needed token to be cut
	if (n>=2)
	{
		// create new dead snake
		*dead=new CDeadSnake(this,lifetime,n);
		length=n;
		snake=(SSnakePos *)realloc(snake,length*sizeof(SSnakePos));
		snakesprites=(int *)realloc(snakesprites,length*sizeof(int));
		return false;
	}
	else
	{
		*dead=new CDeadSnake(this,lifetime);
		return true;
	}
}


SSnakePos CAliveSnake::GetNextStep(void)
{
	SSnakePos temp;
	if (length>0)
	{
		temp.x=snake[0].x+DirtoDx(dir);
		temp.y=snake[0].y+DirtoDy(dir);
	}
	else
	{
		temp.x=defx;
		temp.y=defy;
	}
	return temp;
}

void CAliveSnake::SetDir(int n)
{
	if (frozen)
		return;

	if (upcoming)
	{
		dir=n;
	}
	else
	{
		if (n!=((dir+2)&0x3))
			dir=n; 
	}
}

void CAliveSnake::SetUpcoming(SDL_Surface *si,int t)
{
	if (frozen)
		return;
	
	int x,y;
	for (x=defx-1;x<=defx+1;x++)
	{
		for (y=defy-1;y<=defy+1;y++)
		{
			array->obstaclearray[x][y]=20;
		}
	}
	array->obstaclearray[defx][defy]=-1;
	sprite->Display(si,defx<<3,(defy<<3)+32,13);
	upcoming=true;
	waittime=t;	
}

void CAliveSnake::ClrUpcoming(SDL_Surface *si)
{
	if (frozen)
		return;

	int x,y;
	for (x=defx-1;x<=defx+1;x++)
	{
		for (y=defy-1;y<=defy+1;y++)
		{
			array->obstaclearray[x][y]=-1;
			array->SingleBackgroundPosDraw(x,y,si);
		}
	}
	upcoming=false;
}
