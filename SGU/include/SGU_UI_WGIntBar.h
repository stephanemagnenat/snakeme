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

#ifndef __SGU_UI_WGINTBAR_H
#define __SGU_UI_WGINTBAR_H

#include "SGU_UI_Base.h"
#include "SGU_UI_WGButton.h"

// Generic Integer bar (scroll, sel, etc etc...)
class CUIGenericHorzIntBar:public CUIObject
{
public:
	CUIGenericHorzIntBar(int ix,int iy, int iw,int ih,CUITheme *ithm,int imin,int imax,int ival);
	CUIGenericHorzIntBar(int ix,int iy, int iw,int ih,CUITheme *ithm,int imin,int imax,int ival,int ial);
	~CUIGenericHorzIntBar() { delete left; delete right; }
	bool SendMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si);
	virtual bool SendChildMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si)=0;
	int GetVal(void) { return val; }
	int GetMin(void) { return min; }
	int GetMax(void) { return max; }
	int GetInterval(void) { return max-min; }
	void SetVal(int v) { if ((v>=min) && (v<=max)) val=v; }
protected:
	void ResetInterval(int imin,int imax);
	virtual void Inc(void) { if (val<max) val++; else if (warp) val=min; }
	virtual void Dec(void) { if (val>min) val--; else if (warp) val=max;}
	int min,max,val;
	int arrowlng;
	bool warp;
private:
	CUISignButton *left,*right;
	
};

class CUIGenericVertIntBar:public CUIObject
{
public:
	CUIGenericVertIntBar(int ix,int iy, int iw,int ih,CUITheme *ithm,int imin,int imax,int ival);
	CUIGenericVertIntBar(int ix,int iy, int iw,int ih,CUITheme *ithm,int imin,int imax,int ival,int ial);
	~CUIGenericVertIntBar() { delete top; delete bottom; }
	bool SendMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si);
	virtual bool SendChildMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si)=0;
	int GetVal(void) { return val; }
	int GetMin(void) { return min; }
	int GetMax(void) { return max; }
	int GetInterval(void) { return max-min; }
	void SetVal(int v) { if ((v>=min) && (v<=max)) val=v; }
protected:
	void ResetInterval(int imin,int imax);
	virtual void Inc(void) { if (val<max) val++; else if (warp) val=min;}
	virtual void Dec(void) { if (val>min) val--; else if (warp) val=max;}
	int min,max,val;
	int arrowlng;
	bool warp;
private:
	CUISignButton *top,*bottom;
};

#endif