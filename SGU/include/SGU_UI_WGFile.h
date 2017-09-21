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

#ifndef __SGU_UI_WGFILE_H
#define __SGU_UI_WGFILE_H

#include "SGU_UI_Base.h"
#include "SGU_UI_WGList.h"
#include "SGU_UI_WGScrollBar.h"
#include "SGU_UI_WGText.h"

// File List
class CUIFileLister:public CUIGenericList
{
public:
	CUIFileLister(int ix,int iy, int iw,int ih,CUITheme *ithm,SGU_Sprite *ifnt,int nbel,int elw,int slidew):CUIGenericList(ix, iy, iw, ih, ithm, nbel, elw, slidew) { font=ifnt; }
	~CUIFileLister() { DeleteListContent(); }
private:
	void DeleteListContent();
	void DisplayListElement(int eln,int x,int y,SDL_Surface *si);
public:
	char *GetSelectedFile() { return (char *)strings.GetElementN(GetSelectedElement()); }
	void Scan(char *iext);
	void Add(char *fn) { strings.AddElement(fn); ResetNbEl(strings.GetNbElement()); }
private:
	char ext[16];
	List strings;
	SGU_Sprite *font;
};


// File Chooser
class CUIFileChooser:public CUIGenericHorzIntBar
{
public:
	CUIFileChooser(int ix,int iy, int iw,int ih,CUITheme *ithm,SGU_Sprite *ifnt,bool ihideext):CUIGenericHorzIntBar(ix,iy,iw,ih,ithm,0,0,0) 
	{
		font=ifnt;
		textzone=new CUITextEdit(ix+arrowlng,iy,iw-(arrowlng<<1),ih,ithm,ifnt,"no file",true);
		hideext=ihideext;
		warp=true;
	}
	~CUIFileChooser() { DeleteListContent(); }
private:
	void DeleteListContent();
	bool SendChildMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si);
public:
	char *GetSelectedFile() { return (char *)strings.GetElementN(GetVal()); }
	void Scan(char *iext);
	void Add(char *fn) { strings.AddElement(fn); ResetInterval(0,strings.GetNbElement()-1); }
private:
	bool hideext;
	char ext[16];
	List strings;
	CUITextEdit *textzone;
	SGU_Sprite *font;
};


#endif