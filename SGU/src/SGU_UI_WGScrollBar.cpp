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

#include "SGU_UI_WGScrollBar.h"
#include "SGU_UI_Base.h"

bool CUIVertScrollBar::SendChildMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si)
{
	switch (message)
	{
	case UIEV_LBUTTONDOWN:
		{
			int mx,my;
			GETMOUSEPOS(p1,&mx,&my);
			if (my<y+arrowlng+slidepos)
			{
				baseval=MAX(min,baseval-nve);
				SendChildMessage(INTERNEV_REDRAW,0,0,si);
			}
			else if (my>y+arrowlng+slidepos+lslidebar)
			{
				baseval=MIN(baseval+nve,max-nve+1);
				SendChildMessage(INTERNEV_REDRAW,0,0,si);
			}
			else
			{
				barclickpos=my-slidepos-arrowlng-y;
				SendChildMessage(INTERNEV_REDRAW,0,0,si);
			}
			return true;
		}
	case UIEV_LBUTTONUP:
		{
			if (barclickpos>=0)
			{
				barclickpos=-1;
				return true;
			}
			return false;
		}
	case UIEV_MOUSEMOVE:
		{
			if (barclickpos>=0)
			{
				int mx,my;
				GETMOUSEPOS(p1,&mx,&my);
				if (my<=y+arrowlng)
				{
					my=y+arrowlng+1;
				}
				if (my>=y+h-arrowlng)
				{
					my=y+h-arrowlng-1;
				}				
				int newstartpos=my-barclickpos-y-arrowlng; // new starting position
				int nbaseval=min+(newstartpos*(valdomain-nve+1))/(lslidezone-lslidebar);
				SetBaseVal(nbaseval);
				SendChildMessage(INTERNEV_REDRAW,0,0,si);
				return true;
			}
			return false;
		}
	case UIEV_DRAW:
		{
			lslidezone=h-(2*arrowlng);
			valdomain=max-min;
			slideregion.SaveRegion(si,x,y+arrowlng,w,lslidezone);
			if (valdomain>nve)
			{
				lslidebar=(lslidezone*nve)/(valdomain+1);	
			}
			else
			{
				lslidebar=-1;
			}
			SendChildMessage(INTERNEV_REDRAW,0,0,si);
			return true;
		}
	case INTERNEV_REDRAW:
		{
			if (lslidebar>-1)
			{
				// redraw background
				slideregion.RestaurRegion(si);
				// compute slidepos
				slidepos=((baseval-min)*(lslidezone-lslidebar))/(valdomain-nve+1);
				// draw slide
				SGU_FillRectAlpha(si,x,y+arrowlng+slidepos,w,lslidebar,theme->WgtBack);				
				if (ismouseover)
				{
					SGU_RectShade(si,x,y+arrowlng+slidepos,w,lslidebar,theme->FHiCol,theme->FMidCol,theme->FLowCol);
				}
				else
				{
					SGU_RectShade(si,x,y+arrowlng+slidepos,w,lslidebar,theme->BHiCol,theme->BMidCol,theme->BLowCol);
				}
				return true;
			}
			return false;
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
			barclickpos=-1;
			return true;
		}
	default:
		{
			return false;
		}
	}
}
