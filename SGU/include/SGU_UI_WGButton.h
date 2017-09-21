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

#ifndef __SGU_UI_WGBUTTON_H
#define __SGU_UI_WGBUTTON_H

#include "SGU_UI_Base.h"

// Normal Button
class CUINormalButton:public CUIObject
{
public:
	CUINormalButton(int ix,int iy,int iw,int ih,CUITheme *ithm,SGU_Sprite *ifnt, char *s,bool cnt);
	void SetCaption(char *s);
	bool SendMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si);
public:
	SGU_Sprite *font;
	char *caption;
	int dx;
	bool centered;
};

// Graphic Button (contain a frame)
class CUIGfxButton:public CUIObject
{
public:
	CUIGfxButton(int ix,int iy,int iw,int ih,CUITheme *ithm,SGU_Sprite *ispr, int f,bool ids);
	bool SendMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si);
	void SetSprite(SGU_Sprite *ispr) { sprite=ispr; }
	void SetFrame(int f) { frame=f; }
	void SetDoubleSize(bool ids) { ds=ids; }
public:
	SGU_Sprite *sprite;
	int frame;
	bool ds;
};

// Multi Graphic Button (contain a frame)
class CUIMultiGfxButton:public CUIObject
{
public:
	CUIMultiGfxButton(int ix,int iy,int iw,int ih,CUITheme *ithm,SGU_Sprite *ispr, bool ids);
	bool SendMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si);
	void SetSprite(SGU_Sprite *ispr) { sprite=ispr; }
	int GetFrame(void) { return frame; }
	int GetNbFrames(void) { return sprite->GetNbFrames(); }
	void SetFrame(int f) { frame=f; }
	void SetDoubleSize(bool ids) { ds=ids; }
public:
	SGU_RegionSaver frameregion;
	SGU_Sprite *sprite;
	int frame;
	bool ds;
};

// Sign Button
#define SB_BORDER_SPACE 4

#define SB_ARRAY_RIGHT 1
#define SB_ARRAY_UP 2
#define SB_ARRAY_LEFT 3
#define SB_ARRAY_DOWN 4

class CUISignButton:public CUIObject
{
public:
	CUISignButton(int ix,int iy,int iw,int ih,CUITheme *ithm,int isn);
	bool SendMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si);
public:
	int sn;
};


#endif