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

#include "SGU_Sprite.h"
#include "SGU_GenUtils.h"
#include "SGU_PixelUtils.h"
#include "SDL.h"
#include "SDL_endian.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

	/**
	We load a sprite from filename.
	The sprite format is the one used with MUANG, Urbanit and MGA
	*/
bool SGU_Sprite::Load(char *filename,SDL_PixelFormat *pxlformat)
{
	FILE *fp;
	Uint32 *ptr;
	int p;
	tOrigFrame tmp;
	tOrigFrame *origf;
	int pos,l,m,dx;

	fp=fopen(filename,"rb");
	if (fp==NULL)
		return false;
	// we read the number of frame
	fread(&NbFrames,sizeof(Uint32),1,fp);
	
	NbFrames = SDL_SwapLE32( NbFrames );
	
	// we allocate memory for the frame pointer array
	frames=(tFrame *)malloc(NbFrames*sizeof(tFrame));
	for (Uint32 n=0;n<NbFrames;n++)
	{
		// we read this frame information
		fread(&tmp,sizeof(tOrigFrame),1,fp);
		
		tmp.x = SDL_SwapLE32( tmp.x );
		tmp.y = SDL_SwapLE32( tmp.y );
		tmp.z = SDL_SwapLE32( tmp.z );
		tmp.h = SDL_SwapLE32( tmp.h );
		tmp.w = SDL_SwapLE32( tmp.w );
		tmp.totalsize = SDL_SwapLE32( tmp.totalsize );
		
		
		// we allocate the memory for the given number of line
		origf=(tOrigFrame *)malloc(tmp.totalsize*2);
		///The *2 for debug only ! Luc-O 08.01.2000
		///Because the ""fread(ptr,frames[n]->lines[p].l, sizeof(DWORD) ,fp);""
		///looks like to "overread" and "overwrite" in RAM after frames[n].
		///Result after *2 => more stable.
		
		// we copy info from tmp to frame
		memcpy(origf,&tmp,sizeof(tOrigFrame));
		// we get a pointer to line array
		ptr=(Uint32 *)(((Uint8 *)origf)+sizeof(tOrigFrame)+tmp.h*sizeof(tLine));
		for (p=0;p<tmp.h;p++)
		{
			// we read this Line information
			fread(&origf->lines[p].l,sizeof(Sint32),1,fp);
			
			origf->lines[p].l = SDL_SwapLE32( origf->lines[p].l );
			
			// we write data at correct location
			fread(ptr,origf->lines[p].l, sizeof(Sint32) ,fp);
			
			// we restaur pointer
			origf->lines[p].data=ptr;
			// we calculate next line data address
			ptr += (origf->lines[p].l);
		}

		// Ok, create the surface and set the color key
		frames[n].surface=SDL_CreateRGBSurface(SDL_SRCALPHA|SDL_SRCCOLORKEY|SDL_SWSURFACE,
												tmp.w,
												tmp.h,
												pxlformat->BitsPerPixel,
												pxlformat->Rmask,
												pxlformat->Gmask,
												pxlformat->Bmask,
												pxlformat->Amask);
		SDL_SetColorKey(frames[n].surface,SDL_SRCCOLORKEY,SDL_MapRGB(pxlformat,255,0,255));
		// set palette for surface
		if (pxlformat->BitsPerPixel==8)
		{
			for (p=0;p<255;p++)
			{
				memcpy(frames[n].surface->format->palette->colors,pxlformat->palette->colors,256*sizeof(SDL_Color));
			}
		}
			
		frames[n].x=(Sint16)tmp.x;
		frames[n].y=(Sint16)tmp.y;
		frames[n].z=(Sint16)tmp.z;

		for (p=0;p<tmp.h;p++)
		{
			ptr=(Uint32 *)origf->lines[p].data;
			pos=0;
			while (pos<tmp.w)
			{
				l=*ptr;
				l = SDL_SwapLE32( l );
				ptr++;
				if (l<0) // we have transparence lines
				{
					for (dx=0;dx<-l;dx++)
					{
						SGU_SetPixel(frames[n].surface,pos+dx,p,255,0,255);
					}
					pos+=-l;
				}
				else
				{
					for (m=0;m<l;m++)
					{
						MGAPal32 myRGB;
						// get RGB Values
						myRGB=*(MGAPal32 *)ptr;
						SGU_SetPixel(frames[n].surface,pos,p,myRGB.R,myRGB.G,myRGB.B);
						ptr++;
						pos++;
					}					
				}
			}

		}
		// delete the temporary old-fashioned buffer	
		free(origf);
	}
	fclose(fp);
	return true;
}

bool SGU_Sprite::Display(SDL_Surface *dest,Sint16  x,Sint16  y,Uint32 f)
{
	if (f<NbFrames)
	{
		SDL_Rect destrect;
		destrect.x=x-frames[f].x;
		destrect.y=y-frames[f].y;
		destrect.w=frames[f].surface->w;
		destrect.h=frames[f].surface->h;
		SDL_SetAlpha(frames[f].surface,0,0);
		if(SDL_BlitSurface(frames[f].surface,NULL,dest,&destrect)!=0)
			return false;
		else
			return true;
	}
	else
	{
		return false;
	}
}

bool SGU_Sprite::DisplayClipped(SDL_Surface *dest,Sint16  x,Sint16  y,Uint16 w,Uint16 h, Uint32 f)
{
	if (f<NbFrames)
	{
		SDL_Rect destrect;
		SDL_Rect srcrect;
		destrect.x=x-frames[f].x;
		destrect.y=y-frames[f].y;
		destrect.w=w;
		destrect.h=h;
		srcrect.x=0;
		srcrect.y=0;
		srcrect.w=w;
		srcrect.h=h;
		SDL_SetAlpha(frames[f].surface,0,0);
		if(SDL_BlitSurface(frames[f].surface,&srcrect,dest,&destrect)!=0)
			return false;
		else
			return true;
	}
	else
	{
		return false;
	}
}

bool SGU_Sprite::DisplayAlpha(SDL_Surface *dest,Sint16  x,Sint16  y,Uint32 f,Uint8 p255alpha)
{
	if (f<NbFrames)
	{
		SDL_Rect destrect;
		destrect.x=x-frames[f].x;
		destrect.y=y-frames[f].y;
		destrect.w=frames[f].surface->w;
		destrect.h=frames[f].surface->h;
		SDL_SetAlpha(frames[f].surface,SDL_SRCALPHA,p255alpha);
		if(SDL_BlitSurface(frames[f].surface,NULL,dest,&destrect)!=0)
			return false;
		else
			return true;
	}
	else
	{
		return false;
	}
}

void SGU_Sprite::FreeFrames(void)
{
	if (NbFrames>0)
	{
		for (Uint32 n=0;n<NbFrames;n++)
		{
			SDL_FreeSurface(frames[n].surface);
		}
		free (frames);
	}
}
