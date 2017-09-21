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

#include "SGU_UI_WGList.h"
#include "SGU_UI_Base.h"

#include <string.h>


CUIGenericList::CUIGenericList(int ix,int iy, int iw,int ih,CUITheme *ithm,int nbel,int elw,int slidew)
{
	x=ix;
	y=iy;
	w=iw;
	h=ih;
	theme=ithm;
	ew=elw;
	nve=h/ew;
	sw=slidew;
	ismouseover=false;
	scrollbar = new CUIVertScrollBar(x,y,sw,h,theme,0,nbel-1,0,nve,(sw*4)/5);
}

void CUIGenericList::ResetNbEl(int nbel)
{
	delete scrollbar;
	scrollbar = new CUIVertScrollBar(x,y,sw,h,theme,0,nbel-1,0,h/ew,(sw*4)/5);
}

bool CUIGenericList::SendMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si)
{
	switch (message)
	{
	case INTERNEV_REDRAW:
		{
			// redraw background
			listregion.RestaurRegion(si);
			for (int n=0;n<MIN(nve,scrollbar->GetInterval()+1);n++)
			{
				DisplayListElement(scrollbar->GetBaseVal()+n,x+sw+1,y+1+(n*ew),si);
			}
			// redraw selrect
			if ((scrollbar->GetVal()-scrollbar->GetBaseVal()>=0) && (scrollbar->GetVal()-scrollbar->GetBaseVal()<nve))
			{
				SGU_RectShade(si,x+sw+1,y+1+(scrollbar->GetVal()-scrollbar->GetBaseVal())*ew,w-sw-2,ew,theme->FHiCol,theme->FMidCol,theme->FLowCol);
			}
			return true;
		}
	case UIEV_DRAW:
		{
			// Save for focus
			DefaultSendMessage(UIEV_DRAW,0,0,si);
			scrollbar->SendMessage(UIEV_DRAW,0,0,si);
			// save background
			SGU_FillRectAlpha(si,x+sw+1,y+1,w-sw-2,h-2,theme->WgtBack);
			listregion.SaveRegion(si,x+sw+1,y+1,w-sw-2,h-2);
			// redraw
			SendMessage(INTERNEV_REDRAW,0,0,si);	
			return true;
		}
	case UIEV_GETMOUSEOVER:
		{
			SGU_RectShade(si,x+sw,y,w-sw,h,theme->FHiCol,theme->FMidCol,theme->FLowCol);
			scrollbar->SendMessage(UIEV_GETMOUSEOVER,0,0,si);
			return true;
		}
	case UIEV_LOSEMOUSEOVER:
		{
			SGU_RectShade(si,x+sw,y,w-sw,h,theme->BHiCol,theme->BMidCol,theme->BLowCol);
			scrollbar->SendMessage(UIEV_LOSEMOUSEOVER,0,0,si);
			return true;
		}
	case UIEV_LBUTTONDOWN:
		{
			int mx,my;
			GETMOUSEPOS(p1,&mx,&my);

			if ((mx-x)>=sw)
			{
				int selel=((my-y)/ew)+scrollbar->GetBaseVal();
				scrollbar->SetVal(selel);
			}
			else
			{
				if (scrollbar->GetInterval()>=nve)
					scrollbar->SendMessage(UIEV_LBUTTONDOWN,p1,p2,si);
			}
			
			SendMessage(INTERNEV_REDRAW,0,0,si);
			return true;
		}
	case UIEV_LBUTTONUP:
		{
			if (scrollbar->SendMessage(UIEV_LBUTTONUP,p1,p2,si))
			{
				SendMessage(INTERNEV_REDRAW,0,0,si);
				return true;
			}
			return false;
		}
	case UIEV_MOUSEMOVE:
		{
			if (scrollbar->SendMessage(UIEV_MOUSEMOVE,p1,p2,si))
			{
				SendMessage(INTERNEV_REDRAW,0,0,si);
				return true;
			}
			return false;
		}
	case UIEV_KEYDOWN:
		{
			if (p1==SDLK_UP)
			{
				//int actval=scrollbar->GetVal();
				//actval--;
				scrollbar->SetVal(scrollbar->GetVal()-1);
				if (scrollbar->GetVal()<scrollbar->GetBaseVal())
				{
					scrollbar->Dec();
					scrollbar->SendChildMessage(INTERNEV_REDRAW,0,0,si);
				}
				SendMessage(INTERNEV_REDRAW,0,0,si);
				return true;
			}
			if (p1==SDLK_DOWN)
			{
				scrollbar->SetVal(scrollbar->GetVal()+1);
				if (scrollbar->GetVal()>=scrollbar->GetBaseVal()+nve)
				{
					scrollbar->Inc();
					scrollbar->SendChildMessage(INTERNEV_REDRAW,0,0,si);
				}
				SendMessage(INTERNEV_REDRAW,0,0,si);
				return true;
			}
			return false;
		}
	default:
		return DefaultSendMessage(message,p1,p2,si);
	}
}
/*
void CUIFrameList::DisplayListElement(int eln,int x,int y,SDL_Surface *si)
{
	sprite->Display(si,x,y-1,eln);
};
*/
void CUIStringList::DisplayListElement(int eln,int x,int y,SDL_Surface *si)
{
	SGU_DisplayText(si,font,x,y,(char *)(strings->GetElementN(eln)));
};

