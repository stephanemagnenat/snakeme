/*
	SnakeMe preview maker
	Copyright (C) 2000 Stephane Magnenat

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Stephane Magnenat
    stephane@magnenat.net
*/

#include "SDL.h"
#include "SGU.h"
#include "SGU_Sprite.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// constants
#define SNAKEARRAYW 80
#define SNAKEARRAYH 56

void DoLevel(char *filename)
{
	SDL_Surface *bitmap;
	int x,y,d;

	bitmap=SDL_CreateRGBSurface(SDL_SWSURFACE,160,112,24,0x0000FF,0x00FF00,0xFF0000,0x000000);
	SGU_FillRect(bitmap,0,0,160,112,SGU_RGB(63,63,127,0));
	
	FILE *fp;
	fp=fopen(filename,"rb");
	if (fp)
	{
		// load obstacles
		for (x=0;x<SNAKEARRAYW;x++)
		{
			for (y=0;y<SNAKEARRAYH;y++)
			{
				fscanf(fp,"%d\n",&d);
				switch (d)
				{
				case 10:
				case 11:
					SGU_SetPixel(bitmap,x<<1,y<<1,0,0,0);
					SGU_SetPixel(bitmap,(x<<1)+1,(y<<1),0,0,0);
					SGU_SetPixel(bitmap,(x<<1),(y<<1)+1,0,0,0);
					SGU_SetPixel(bitmap,(x<<1)+1,(y<<1)+1,0,0,0);
					break;
				case 12:
				case 13:
					SGU_SetPixel(bitmap,x<<1,y<<1,255,0,0);
					SGU_SetPixel(bitmap,(x<<1)+1,(y<<1),255,0,0);
					SGU_SetPixel(bitmap,(x<<1),(y<<1)+1,255,0,0);
					SGU_SetPixel(bitmap,(x<<1)+1,(y<<1)+1,255,0,0);
					break;
				}
			}
		}
		fclose(fp);
	}
	
	char string[256];
	strcpy( string, filename);
	strcat( string, ".bmp");
	SDL_SaveBMP(bitmap,string);
	SDL_FreeSurface(bitmap);
}


void ThemeBackDraw(int x,int y,SDL_Surface *si,SGU_Sprite *theme)
{
	// Draw background
	SDL_Rect r1,r2;
	r1.x=((x<<3)%theme->GetW(0));
	r1.y=((y<<3)%theme->GetH(0));
	r1.w=8;
	r1.h=8;
	r2.x=x<<3;
	r2.y=(y<<3);
	r2.w=8;
	r2.h=8;
	SDL_BlitSurface(theme->GetFrameSurface(0),&r1,si,&r2);
}

void DoTheme(char *filename)
{
	SDL_Surface *bitmap;
	SGU_Sprite theme;
	int x,y;

	bitmap=SDL_CreateRGBSurface(SDL_SWSURFACE,160,112,24,0x0000FF,0x00FF00,0xFF0000,0x000000);
	theme.Load(filename,bitmap->format);

	// background
	for (y=13;y>=0;--y)
	{
		for (x=19;x>=0;--x)
		{
			ThemeBackDraw(x,y,bitmap,&theme);
		}
	}

	// undestructable wall
	for (y=13;y>=0;--y)
	{
		theme.Display(bitmap,0,y<<3,1);
		theme.Display(bitmap,152,y<<3,1);
	}
	for (x=19;x>=0;--x)
	{
		theme.Display(bitmap,x<<3,0,1);
		theme.Display(bitmap,x<<3,104,1);
	}
	for (y=3;y<11;y++)
	{
		theme.Display(bitmap,24,y<<3,2);
		theme.Display(bitmap,128,y<<3,2);
	}

	// destructable wall
	for (x=6;x<14;x++)
	{
		theme.Display(bitmap,x<<3,24,3);
		theme.Display(bitmap,x<<3,80,3);
	}
	for (y=5;y<9;y++)
	{
		theme.Display(bitmap,48,y<<3,4);
		theme.Display(bitmap,104,y<<3,4);
	}
	theme.Display(bitmap,56,40,4);
	theme.Display(bitmap,96,40,4);
	theme.Display(bitmap,56,64,4);
	theme.Display(bitmap,96,64,4);


	char string[256];
	strcpy( string, filename);
	strcat( string, ".bmp");
	SDL_SaveBMP(bitmap,string);
	SDL_FreeSurface(bitmap);
}


void DoSnake(char *filename)
{
	SDL_Surface *bitmap;
	SGU_Sprite spr;
	bitmap=SDL_CreateRGBSurface(SDL_SWSURFACE,48,8,24,0x0000FF,0x00FF00,0xFF0000,0x000000);
	SGU_FillRect(bitmap,0,0,48,8,SGU_RGB(255,255,255,0));
	spr.Load(filename,bitmap->format);
	spr.Display(bitmap,0,0,14);
	for (int m=1;m<5;m++)
	{
		spr.Display(bitmap,(m<<3),0,2);
	}
	spr.Display(bitmap,40,0,18);
	char string[256];
	strcpy( string, filename);
	strcat( string, ".bmp");
	SDL_SaveBMP(bitmap,string);
	SDL_FreeSurface(bitmap);
}



int main( int argc, char* argv[] )
{
	// process command line
	if (argc!=3)
	{
		printf("Wrong numer of argument, use : makeprev {t,l,s} filename\n");
		printf("\nt for theme, l for level, s for snake\n\n");
		exit(1);
	}

	// SDL init
    if ( SDL_Init(0) < 0 )
	{
		fprintf(stderr,"SDL : Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(2);
    }
	atexit(SDL_Quit);

	switch (argv[1][0])
	{
	case 'l':
	case 'L':
		DoLevel(argv[2]);
		break;
	case's':
	case'S':
		DoSnake(argv[2]);
		break;
	case 't':
	case 'T':
		DoTheme(argv[2]);
		break;
	}
	return 0;
}