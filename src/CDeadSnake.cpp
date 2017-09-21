/*  ======= CDeadSnake ======= *\
    (c) 2000 Stephane Magnenat
\*  ========================== */

#include <string.h>
#include "CDeadSnake.h"

CDeadSnake::CDeadSnake(CSnake *snaketemplate,int lifetime)
{
	length=snaketemplate->length;
	array=snaketemplate->array;
	sprite=snaketemplate->sprite;
	snake=(SSnakePos *)malloc(length*sizeof(SSnakePos));
	memcpy(snake,snaketemplate->snake,length*sizeof(SSnakePos));
	snakesprites=(int *)malloc(length*sizeof(int));
	memcpy(snakesprites,snaketemplate->snakesprites,length*sizeof(int));
	lifetimeleft=lifetime;
	maxlifetime=lifetime;
	for (int n=0;n<length;n++)
	{
		array->obstaclearray[snake[n].x][snake[n].y]=14;
	}
}

CDeadSnake::CDeadSnake(CSnake *snaketemplate,int lifetime,int pos)
{
	length=snaketemplate->length-pos;
	array=snaketemplate->array;
	sprite=snaketemplate->sprite;
	snake=(SSnakePos *)malloc(length*sizeof(SSnakePos));
	memcpy(snake,snaketemplate->snake+pos,length*sizeof(SSnakePos));
	snakesprites=(int *)malloc(length*sizeof(int));
	memcpy(snakesprites,snaketemplate->snakesprites+pos,length*sizeof(int));
	lifetimeleft=lifetime;
	maxlifetime=lifetime;
	for (int n=0;n<length;n++)
	{
		array->obstaclearray[snake[n].x][snake[n].y]=14;
	}
}

CDeadSnake::~CDeadSnake()
{
}

bool CDeadSnake::DoStepAndDraw(SDL_Surface *si)
{
	if (lifetimeleft>0)
	{
		for (int n=0;n<length;n++)
		{
			if (array->obstaclearray[snake[n].x][snake[n].y]!=-1)
			{
				array->SingleBackgroundPosDraw(snake[n].x,snake[n].y,si);
				sprite->DisplayAlpha(si,(snake[n].x<<3),(snake[n].y<<3)+32,snakesprites[n],255-(179-((lifetimeleft*179)/(maxlifetime))));
			}
		}
		lifetimeleft--;
		return true;
	}
	else
	{
		for (int n=0;n<length;n++)
		{
			array->SingleBackgroundPosDraw(snake[n].x,snake[n].y,si);
			array->obstaclearray[snake[n].x][snake[n].y]=-1;
		}
		return false;
	}
}

