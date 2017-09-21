/*  ========= CSnake ========= *\
    (c) 1999 Stephane Magnenat
\*  ========================== */

#include "CSnake.h"

CSnake::CSnake()
{ 
	snake=NULL; 
	snakesprites=NULL;
	length=0;
}

CSnake::~CSnake()
{ 
	if (snake) 
		free(snake); 
	snake=NULL; 
	if (snakesprites)
		free(snakesprites);
	snakesprites=NULL;
	length=0;
}

void CSnake::Clear(void)
{
	if (snake)
		free(snake);
	snake=NULL;
	if (snakesprites)
		free(snakesprites);
	snakesprites=NULL;
	length=0;
}



