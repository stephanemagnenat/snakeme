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

#ifndef __CUITheme_H
#define __CUITheme_H

#include "SGU.h"

class CUITheme
{
public:
	CUITheme();
	~CUITheme();
	bool Load(char *filename,SDL_Surface *si);
	bool Save(char *filename,SDL_Surface *si);
	void RepackColor(SDL_Surface *si);
	void DrawBackground(SDL_Surface *si,SDL_Rect *rect);
public:
	SGU_RGBA FHiCol;
	SGU_RGBA FMidCol;
	SGU_RGBA FLowCol;
	SGU_RGBA BHiCol;
	SGU_RGBA BMidCol;
	SGU_RGBA BLowCol;
	SGU_RGBA WgtBack;
	SDL_Surface *bgpict;
};

#endif