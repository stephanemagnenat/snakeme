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

#ifndef __SGU_UI_WGVALUEBAR_H
#define __SGU_UI_WGVALUEBAR_H

#include "SGU_UI_Base.h"
#include "SGU_UI_WGIntBar.h"

#define CLICKTHRESHOLD 4

class CUIHorzValueBar:public CUIGenericHorzIntBar
{
public:
	CUIHorzValueBar(int ix,int iy, int iw,int ih,CUITheme *ithm,int imin,int imax,int ival):CUIGenericHorzIntBar(ix,iy,iw,ih,ithm,imin,imax,ival)
	{
		ismouseover=false;
		dragged=false;
		val=min;
	}
	CUIHorzValueBar(int ix,int iy, int iw,int ih,CUITheme *ithm,int imin,int imax,int ival,int iaw):CUIGenericHorzIntBar(ix,iy,iw,ih,ithm,imin,imax,ival,iaw)
	{
		ismouseover=false;
		dragged=false;
		val=min;
	}
	bool SendChildMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si);
	inline void Inc(void) { if (val<max) val++; }
	inline void Dec(void) { if (val>min) val--; }
private:
	int lbarzone;
	int barlng;
	bool dragged;
	bool ismouseover;
	SGU_RegionSaver barregion;
};

#endif