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

#include "SGU_UI_WGValueBar.h"
#include "SGU_UI_Base.h"

bool CUIHorzValueBar::SendChildMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si)
{
	switch (message)
	{
	case UIEV_LBUTTONDOWN:
		{
			int mx,my;
			GETMOUSEPOS(p1,&mx,&my);
			if (abs(mx-(barlng+x+arrowlng))<CLICKTHRESHOLD)
			{
				dragged=true;
			}
			return true;
		}
	case UIEV_LBUTTONUP:
		{
			dragged=false;
			return false;
		}
	case UIEV_MOUSEMOVE:
		{
			if (dragged)
			{
				int mx,my;
				GETMOUSEPOS(p1,&mx,&my);
				if (mx<=x+arrowlng)
				{
					mx=x+arrowlng+1;
				}
				if (mx>=x+w-arrowlng)
				{
					mx=x+h-arrowlng-1;
				}				
				int newstartpos=mx-x-arrowlng; // new starting position
				int nbaseval=min+(newstartpos*(max-min))/(lbarzone);
				SetVal(nbaseval);
				SendChildMessage(INTERNEV_REDRAW,0,0,si);
				return true;
			}
			return false;
		}
	case UIEV_DRAW:
		{
			lbarzone=w-(2*arrowlng);
			barregion.SaveRegion(si,x+arrowlng,y,lbarzone,h);
			SendChildMessage(INTERNEV_REDRAW,0,0,si);
			return true;
		}
	case INTERNEV_REDRAW:
		{
			// redraw background
			barregion.RestaurRegion(si);
			// compute slidepos
			barlng=((val-min)*(lbarzone))/(max-min);
			// draw slide
			SGU_FillRectAlpha(si,x+arrowlng,y,barlng,h,theme->WgtBack);				
			if (ismouseover)
			{
				SGU_RectShade(si,x+arrowlng,y,barlng,h,theme->FHiCol,theme->FMidCol,theme->FLowCol);
			}
			else
			{
				SGU_RectShade(si,x+arrowlng,y,barlng,h,theme->BHiCol,theme->BMidCol,theme->BLowCol);
			}
			return true;
		}
	case UIEV_GETMOUSEOVER:
		{
			ismouseover=true;
			SendChildMessage(INTERNEV_REDRAW,0,0,si);
			return true;
		}
	case UIEV_LOSEMOUSEOVER:
		{
			ismouseover=false;
			SendChildMessage(INTERNEV_REDRAW,0,0,si);
			return true;
		}
	case UIEV_LOSEFOCUS:
		{
			dragged=false;
			return true;
		}
	default:
		{
			return false;
		}
	}
}
