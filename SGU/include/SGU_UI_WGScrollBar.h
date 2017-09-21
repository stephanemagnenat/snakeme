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

#ifndef __SGU_UI_WGSCROLLBAR_H
#define __SGU_UI_WGSCROLLBAR_H

#include "SGU_UI_Base.h"
#include "SGU_UI_WGIntBar.h"

class CUIVertScrollBar:public CUIGenericVertIntBar
{
public:
	CUIVertScrollBar(int ix,int iy, int iw,int ih,CUITheme *ithm,int imin,int imax,int ival,int inve):CUIGenericVertIntBar(ix,iy,iw,ih,ithm,imin,imax,ival)
	{
		nve=inve;
		ismouseover=false;
		baseval=min;
		val=min;
		barclickpos=-1;
	}
	CUIVertScrollBar(int ix,int iy, int iw,int ih,CUITheme *ithm,int imin,int imax,int ival,int inve,int iaw):CUIGenericVertIntBar(ix,iy,iw,ih,ithm,imin,imax,ival,iaw)
	{
		nve=inve;
		ismouseover=false;
		baseval=min;
		val=min;
		barclickpos=-1;
	}
	bool SendChildMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si);
	inline void Inc(void) { if (baseval<(max-nve+1)) baseval++; }
	inline void Dec(void) { if (baseval>min) baseval--; }
	inline int GetBaseVal(void) { return baseval; }
	inline void SetBaseVal(int bv) { if ((bv<=(max-nve+1))&&(bv>=min)) baseval=bv; }
private:
	int lslidebar; // length of the scroll bar
	int slidepos; // y pos of bar
	int lslidezone; // length of the slide zone
	int nve; // number of viewvabels elements
	int valdomain; // max-min
	int baseval; // first val on screen (bar start)
	int barclickpos; // pos of click on bar
	bool ismouseover;
	SGU_RegionSaver slideregion;
};

#endif