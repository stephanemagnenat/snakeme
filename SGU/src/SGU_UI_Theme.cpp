/*
    SGU - SDL Graphic Utility
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

#include "SGU_UI_Theme.h"
#include <stdio.h>

CUITheme::CUITheme()
{
	bgpict=NULL;
}

CUITheme::~CUITheme()
{

}

bool CUITheme::Load(char *filename,SDL_Surface *si)
{
	FILE *fp;
	if ((fp=fopen(filename,"rb"))==NULL)
	{
		return false;
	}
	else
	{
		fread(&FHiCol,sizeof(SGU_RGBA),1,fp);
		fread(&FLowCol,sizeof(SGU_RGBA),1,fp);
		fread(&BHiCol,sizeof(SGU_RGBA),1,fp);
		fread(&BLowCol,sizeof(SGU_RGBA),1,fp);
		fread(&WgtBack,sizeof(SGU_RGBA),1,fp);
		fclose(fp);
		RepackColor(si);
		return true;
	}
}

bool CUITheme::Save(char *filename,SDL_Surface *si)
{
	FILE *fp;
	if ((fp=fopen(filename,"wb"))==NULL)
	{
		return false;
	}
	else
	{
		fwrite(&FHiCol,sizeof(SGU_RGBA),1,fp);
		fwrite(&FLowCol,sizeof(SGU_RGBA),1,fp);
		fwrite(&BHiCol,sizeof(SGU_RGBA),1,fp);
		fwrite(&BLowCol,sizeof(SGU_RGBA),1,fp);
		fwrite(&WgtBack,sizeof(SGU_RGBA),1,fp);
		fclose(fp);
		RepackColor(si);
		return true;
	}
}

void CUITheme::RepackColor(SDL_Surface *si)
{
	// Recalculate mid color
	FMidCol.R=(FHiCol.R+FLowCol.R)>>1;
	FMidCol.G=(FHiCol.G+FLowCol.G)>>1;
	FMidCol.B=(FHiCol.B+FLowCol.B)>>1;
	BMidCol.R=(BHiCol.R+BLowCol.R)>>1;
	BMidCol.G=(BHiCol.G+BLowCol.G)>>1;
	BMidCol.B=(BHiCol.B+BLowCol.B)>>1;
}

void CUITheme::DrawBackground(SDL_Surface *si,SDL_Rect *rect)
{
	int posx,posy;
	int x1,y1,x2,y2,nx,ny;
	SDL_Rect srect,drect;
	x1=rect->x;
	y1=rect->y;
	x2=rect->x+rect->w;
	y2=rect->y+rect->h;
	posx=(x1/bgpict->w)*bgpict->w;
	nx=x1/bgpict->w;	
	while (posx<x2)
	{
		posy=(y1/bgpict->h)*bgpict->h;
		ny=y1/bgpict->h;
		while (posy<y2)
		{
			srect.x=MAX(x1-posx,0);
			srect.y=MAX(y1-posy,0);
			srect.w=MIN(x2-posx,bgpict->w)-srect.x;
			srect.h=MIN(y2-posy,bgpict->h)-srect.y;
			drect.x=srect.x+nx*bgpict->w;
			drect.y=srect.y+ny*bgpict->h;
			drect.w=srect.w;
			drect.h=srect.h;
			posy+=bgpict->h;
			SDL_BlitSurface(bgpict,&srect,si,&drect);
			ny++;
		}
		posx+=bgpict->w;
		nx++;
	}
}