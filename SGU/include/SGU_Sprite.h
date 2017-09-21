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

#ifndef __SGU_SPRITE_H
#define __SGU_SPRITE_H

#include "SDL.h"

	// Frame structures
typedef struct
{
	Sint16 z;
	Sint16 x;
	Sint16 y;
	SDL_Surface *surface;
} tFrame;

	// Line structure, used in original sprite
typedef struct
{
	int l;
	void *data;
} tLine;

	// MGAPal32 structure, used for loading sprite
typedef struct
{
	unsigned char B,G,R,A;
} MGAPal32;

	// Original Sprite structures
typedef struct
{
	int totalsize;		// =sizeof(tFrame)+h*sizeof(tLine)+sizeof(all the data);
	int w;
	int h;
	int z;
	int x;
	int y;
	tLine lines[0];
} tOrigFrame;

	/** SGU Sprite Object for SDL */
class SGU_Sprite
{
public:
	SGU_Sprite() { NbFrames=0; frames=NULL; }
	~SGU_Sprite() { FreeFrames(); }
		/// This methode load a sprite (*.spr) from the disk
	bool Load(char *filename,SDL_PixelFormat *pxlformat);
		/// This methode display a sprite on a given SDL_Surface
	bool Display(SDL_Surface *dest,Sint16  x,Sint16  y,Uint32 f);
		/// This methode display a sprite on a given SDL_Surface with alpha
	bool DisplayAlpha(SDL_Surface *dest,Sint16  x,Sint16  y,Uint32 f,Uint8 p255alpha);
		/// This methode display a sprite on a given SDL_Surface, but clipped
	bool DisplayClipped(SDL_Surface *dest,Sint16  x,Sint16  y,Uint16 w,Uint16 h, Uint32 f);
	/*void SetZ(int f,int z) { frames[f].z=z; }
	void SetX(int f,int x) { frames[f].x=x; }
	void SetY(int f,int y) { frames[f].y=y; }*/
		/// This methode give the z value for the given frame f
	int GetZ(int f) { return frames[f].z; }
		/// This methode give the x offset for the given frame f
	int GetX(int f) { return frames[f].x; }
		/// This methode give the y offset for the given frame f
	int GetY(int f) { return frames[f].y; }
		/// This methode give the width for the given frame f
	int GetW(int f) { return frames[f].surface->w; }
		/// This methode give the height for the given frame f
	int GetH(int f) { return frames[f].surface->h; }
		/// This methode give the number of frame
	int GetNbFrames(void) {  return NbFrames; }
		/// Return The SDL Surface associated to a frame
	SDL_Surface *GetFrameSurface(int f) { return frames[f].surface; }
private:
		/// This free the memory for this sprite, internal use only
	void FreeFrames(void);
private:
	tFrame *frames;
	Uint32 NbFrames;
};

#endif
