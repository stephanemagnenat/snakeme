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

#ifndef __SGU_UI_WGSLIST_H
#define __SGU_UI_WGSLIST_H

#include "SGU_UI_Base.h"
#include "SGU_UI_WGScrollBar.h"

// Generic List
class CUIGenericList:public CUIObject
{
public:
	CUIGenericList(int ix,int iy, int iw,int ih,CUITheme *ithm,int nbel,int elw,int slidew);
	~CUIGenericList(void) { delete scrollbar; }
	bool SendMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si);
	int GetSelectedElement(void) { return scrollbar->GetVal(); }
protected:
	void ResetNbEl(int nbel);
private:
	virtual void DisplayListElement(int eln,int x,int y,SDL_Surface *si)=0;
private:
	CUIVertScrollBar *scrollbar;
	int ew; // element width
	int nve; // number of viewable element
	int sw; // scroll bar width
	bool ismouseover;
	SGU_RegionSaver listregion;
};

// Frame List
/*class CUIFrameList:public CUIGenericList
{
public:
	CUIFrameList(int ix,int iy, int iw,int ih,CUITheme *ithm,int nbel,int elw,bool vert,int slidew):CUIGenericList( ix, iy,  iw, ih, ithm, nbel, elw, vert,slidew) {}
private:
	void DisplayListElement(int eln,int x,int y,SDL_Surface *si);
public:
	void SetSprite(SGU_Sprite *spr) {sprite=spr; }
private:
	SGU_Sprite *sprite;
};
*/

// String List
class CUIStringList:public CUIGenericList
{
public:
	CUIStringList(int ix,int iy, int iw,int ih,CUITheme *ithm,int nbel,int elw,int slidew):CUIGenericList(ix, iy, iw, ih, ithm, nbel, elw, slidew) {}
private:
	void DisplayListElement(int eln,int x,int y,SDL_Surface *si);
public:
	void SetStringList(List *s) {strings=s; }
	void SetFont(SGU_Sprite *fnt) { font=fnt; }
private:
	List *strings;
	SGU_Sprite *font;
};

#endif
