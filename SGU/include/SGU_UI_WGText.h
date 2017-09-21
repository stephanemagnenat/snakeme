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

#ifndef __SGU_UI_WGTEXT_H
#define __SGU_UI_WGTEXT_H

#include "SGU_UI_Base.h"

// Text Edit control
class CUITextEdit:public CUIObject
{
public:
	CUITextEdit(int ix,int iy, int iw,int ih,CUITheme *ithm,SGU_Sprite *ifnt, char *s,bool readonly);
	bool SendMessage(int message,ssize_t p1,ssize_t p2,SDL_Surface *si);
	char *GetText(void) { text[textcount]=0; return text; }
	void SetText(char *s);
public:
	char text[256];
	bool needredraw;
	bool ro;
	SGU_RegionSaver textregion;
	SGU_Sprite *font;
	unsigned char cursor;
	unsigned char textcount;
};

#endif
