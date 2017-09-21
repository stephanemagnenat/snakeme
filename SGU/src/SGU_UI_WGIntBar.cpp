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

#include "SGU_UI_WGIntBar.h"

// Horizontal

CUIGenericHorzIntBar::CUIGenericHorzIntBar(int ix,int iy, int iw,int ih,CUITheme *ithm,int imin,int imax,int ival)
{
	x=ix;
	y=iy;
	w=iw;
	h=ih;
	arrowlng=h;
	theme=ithm;
	min=imin;
	max=imax;
	warp=false;
	SetVal(ival);
	left=new CUISignButton(x,y,arrowlng,h,theme,SB_ARRAY_LEFT);
	right=new CUISignButton(x+w-arrowlng,y,arrowlng,h,theme,SB_ARRAY_RIGHT);
}


CUIGenericHorzIntBar::CUIGenericHorzIntBar(int ix,int iy, int iw,int ih,CUITheme *ithm,int imin,int imax,int ival,int ial)
{
	x=ix;
	y=iy;
	w=iw;
	h=ih;
	arrowlng=ial;
	theme=ithm;
	min=imin;
	max=imax;
	warp=false;
	SetVal(ival);
	left=new CUISignButton(x,y,arrowlng,h,theme,SB_ARRAY_LEFT);
	right=new CUISignButton(x+w-arrowlng,y,arrowlng,h,theme,SB_ARRAY_RIGHT);
}


void CUIGenericHorzIntBar::ResetInterval(int imin,int imax)
{
	min=imin;
	max=imax;
	SetVal(imin);
}


bool CUIGenericHorzIntBar::SendMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si)
{
	switch (message)
	{
	case UIEV_KEYDOWN:
		{
			if (p1==SDLK_LEFT)
			{
				Dec();
				SendChildMessage(INTERNEV_REDRAW,0,0,si);
				return true;
			}
			else if (p1==SDLK_RIGHT)
			{
				Inc();
				SendChildMessage(INTERNEV_REDRAW,0,0,si);
				return true;
			}
			else
			{
				return SendChildMessage(UIEV_KEYDOWN,p1,p2,si);
				//return false;
			}
		}
	case UIEV_DRAW:
		{
			// Save for focus
			DefaultSendMessage(UIEV_DRAW,0,0,si);
			// Pass message
			left->SendMessage(UIEV_DRAW,0,0,si);
			right->SendMessage(UIEV_DRAW,0,0,si);
			SendChildMessage(UIEV_DRAW,0,0,si);
			return true;
		}
	case UIEV_LBUTTONDOWN:
		{
			int mx,my;
			GETMOUSEPOS(p1,&mx,&my);
			if (left->IsPtOnObject(mx,my))
			{
				Dec();
				SendChildMessage(INTERNEV_REDRAW,0,0,si);
				left->SendMessage(UIEV_LBUTTONDOWN,p1,p2,si);
				return true;
			}
			else if (right->IsPtOnObject(mx,my))
			{
				Inc();
				SendChildMessage(INTERNEV_REDRAW,0,0,si);
				right->SendMessage(UIEV_LBUTTONDOWN,p1,p2,si);
				return true;
			}	
			else
			{
				return SendChildMessage(UIEV_LBUTTONDOWN,p1,p2,si);
				//return false;
			}
		}
	case UIEV_LBUTTONUP:
		{
			int mx,my;
			GETMOUSEPOS(p1,&mx,&my);
			if (left->IsPtOnObject(mx,my))
			{
				left->SendMessage(UIEV_LBUTTONUP,p1,p2,si);
				SendChildMessage(UIEV_LBUTTONUP,p1,p2,si);
				return true;
			}
			else if (right->IsPtOnObject(mx,my))
			{
				right->SendMessage(UIEV_LBUTTONUP,p1,p2,si);
				SendChildMessage(UIEV_LBUTTONUP,p1,p2,si);
				return true;
			}
			else
			{
				return SendChildMessage(UIEV_LBUTTONUP,p1,p2,si);
			}
		}
	case UIEV_MOUSEMOVE:
		{
			int mx,my;
			GETMOUSEPOS(p1,&mx,&my);
			if (this->IsPtOnObject(mx,my))
			{
				if (left->IsPtOnObject(mx,my))
				{
					left->SendMessage(UIEV_GETMOUSEOVER,p1,p2,si);
					right->SendMessage(UIEV_GETMOUSEOVER,p1,0,si);
					return true;
				}
				else if (right->IsPtOnObject(mx,my))
				{
					right->SendMessage(UIEV_GETMOUSEOVER,p1,p2,si);
					left->SendMessage(UIEV_GETMOUSEOVER,p1,0,si);
					return true;
				}
				else
				{
					left->SendMessage(UIEV_GETMOUSEOVER,p1,0,si);
					right->SendMessage(UIEV_GETMOUSEOVER,p1,0,si);
					SendChildMessage(UIEV_MOUSEMOVE,p1,p2,si);
					// note : fix this, we shoudl know if we have changed internal mouseover
					return true;
				}
			}
			else
			{
				return SendChildMessage(UIEV_MOUSEMOVE,p1,p2,si);
			}
		}
	case UIEV_GETMOUSEOVER:
		{
			int mx,my;
			GETMOUSEPOS(p1,&mx,&my);
			if (left->IsPtOnObject(mx,my))
			{
				left->SendMessage(UIEV_GETMOUSEOVER,p1,p2,si);
				right->SendMessage(UIEV_GETMOUSEOVER,p1,0,si);
			}
			else if (right->IsPtOnObject(mx,my))
			{
				right->SendMessage(UIEV_GETMOUSEOVER,p1,p2,si);
				left->SendMessage(UIEV_GETMOUSEOVER,p1,0,si);
			}
			else
			{
				left->SendMessage(UIEV_GETMOUSEOVER,p1,0,si);
				right->SendMessage(UIEV_GETMOUSEOVER,p1,0,si);	
			}
			SendChildMessage(UIEV_GETMOUSEOVER,p1,p2,si);
			return true;
		}
	case UIEV_LOSEMOUSEOVER:
		{
			right->SendMessage(UIEV_LOSEMOUSEOVER,p1,p2,si);
			left->SendMessage(UIEV_LOSEMOUSEOVER,p1,p2,si);
			SendChildMessage(UIEV_LOSEMOUSEOVER,p1,p2,si);
			return true;
		}
	case UIEV_LOSEFOCUS:
		{
			SendChildMessage(UIEV_LOSEFOCUS,p1,p2,si);
			DefaultSendMessage(message,p1,p2,si);
			return true;
		}
	default:
		return DefaultSendMessage(message,p1,p2,si);
	}
}

// Vertical

CUIGenericVertIntBar::CUIGenericVertIntBar(int ix,int iy, int iw,int ih,CUITheme *ithm,int imin,int imax,int ival)
{
	x=ix;
	y=iy;
	w=iw;
	h=ih;
	arrowlng=w;
	theme=ithm;
	min=imin;
	max=imax;
	SetVal(ival);
	top=new CUISignButton(x,y,w,arrowlng,theme,SB_ARRAY_UP);
	bottom=new CUISignButton(x,y+h-arrowlng,w,arrowlng,theme,SB_ARRAY_DOWN);
}

CUIGenericVertIntBar::CUIGenericVertIntBar(int ix,int iy, int iw,int ih,CUITheme *ithm,int imin,int imax,int ival,int ial)
{
	x=ix;
	y=iy;
	w=iw;
	h=ih;
	arrowlng=ial;
	theme=ithm;
	min=imin;
	max=imax;
	SetVal(ival);
	top=new CUISignButton(x,y,w,arrowlng,theme,SB_ARRAY_UP);
	bottom=new CUISignButton(x,y+h-arrowlng,w,arrowlng,theme,SB_ARRAY_DOWN);
}


void CUIGenericVertIntBar::ResetInterval(int imin,int imax)
{
	min=imin;
	max=imax;
	SetVal(imin);
}


bool CUIGenericVertIntBar::SendMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si)
{
	switch (message)
	{
	case UIEV_KEYDOWN:
		{
			if (p1==SDLK_UP)
			{
				Dec();
				SendChildMessage(INTERNEV_REDRAW,0,0,si);
				return true;
			}
			else if (p1==SDLK_DOWN)
			{
				Inc();
				SendChildMessage(INTERNEV_REDRAW,0,0,si);
				return true;
			}
			else
			{
				return SendChildMessage(UIEV_KEYDOWN,p1,p2,si);
				//return false;
			}
		}
	case UIEV_DRAW:
		{
			// Save for focus
			DefaultSendMessage(UIEV_DRAW,0,0,si);
			// Pass message
			top->SendMessage(UIEV_DRAW,0,0,si);
			bottom->SendMessage(UIEV_DRAW,0,0,si);
			SendChildMessage(UIEV_DRAW,0,0,si);
			return true;
		}
	case UIEV_LBUTTONDOWN:
		{
			int mx,my;
			GETMOUSEPOS(p1,&mx,&my);
			if (top->IsPtOnObject(mx,my))
			{
				Dec();
				SendChildMessage(INTERNEV_REDRAW,0,0,si);
				top->SendMessage(UIEV_LBUTTONDOWN,p1,p2,si);
				return true;
			}
			else if (bottom->IsPtOnObject(mx,my))
			{
				Inc();
				SendChildMessage(INTERNEV_REDRAW,0,0,si);
				bottom->SendMessage(UIEV_LBUTTONDOWN,p1,p2,si);
				return true;
			}	
			else
			{
				return SendChildMessage(UIEV_LBUTTONDOWN,p1,p2,si);
				//return false;
			}
		}
	case UIEV_LBUTTONUP:
		{
			int mx,my;
			GETMOUSEPOS(p1,&mx,&my);
			if (top->IsPtOnObject(mx,my))
			{
				top->SendMessage(UIEV_LBUTTONUP,p1,p2,si);
				SendChildMessage(UIEV_LBUTTONUP,p1,p2,si);
				return true;
			}
			else if (bottom->IsPtOnObject(mx,my))
			{
				bottom->SendMessage(UIEV_LBUTTONUP,p1,p2,si);
				SendChildMessage(UIEV_LBUTTONUP,p1,p2,si);
				return true;
			}
			else
			{
				return SendChildMessage(UIEV_LBUTTONUP,p1,p2,si);
			}
		}
	case UIEV_MOUSEMOVE:
		{
			int mx,my;
			GETMOUSEPOS(p1,&mx,&my);
			if (this->IsPtOnObject(mx,my))
			{
				if (top->IsPtOnObject(mx,my))
				{
					top->SendMessage(UIEV_GETMOUSEOVER,p1,p2,si);
					bottom->SendMessage(UIEV_GETMOUSEOVER,p1,0,si);
					return true;
				}
				else if (bottom->IsPtOnObject(mx,my))
				{
					bottom->SendMessage(UIEV_GETMOUSEOVER,p1,p2,si);
					top->SendMessage(UIEV_GETMOUSEOVER,p1,0,si);
					return true;
				}
				else
				{
					top->SendMessage(UIEV_GETMOUSEOVER,p1,0,si);
					bottom->SendMessage(UIEV_GETMOUSEOVER,p1,0,si);
					SendChildMessage(UIEV_MOUSEMOVE,p1,p2,si);
					// note : fix this, we shoudl know if we have changed internal mouseover
					return true;
				}
			}
			else
			{
				return SendChildMessage(UIEV_MOUSEMOVE,p1,p2,si);
			}
		}
	case UIEV_GETMOUSEOVER:
		{
			int mx,my;
			GETMOUSEPOS(p1,&mx,&my);
			if (top->IsPtOnObject(mx,my))
			{
				top->SendMessage(UIEV_GETMOUSEOVER,p1,p2,si);
				bottom->SendMessage(UIEV_GETMOUSEOVER,p1,0,si);
			}
			else if (bottom->IsPtOnObject(mx,my))
			{
				bottom->SendMessage(UIEV_GETMOUSEOVER,p1,p2,si);
				top->SendMessage(UIEV_GETMOUSEOVER,p1,0,si);
			}
			else
			{
				top->SendMessage(UIEV_GETMOUSEOVER,p1,0,si);
				bottom->SendMessage(UIEV_GETMOUSEOVER,p1,0,si);	
			}
			SendChildMessage(UIEV_GETMOUSEOVER,p1,p2,si);
			return true;
		}
	case UIEV_LOSEMOUSEOVER:
		{
			bottom->SendMessage(UIEV_LOSEMOUSEOVER,p1,p2,si);
			top->SendMessage(UIEV_LOSEMOUSEOVER,p1,p2,si);
			SendChildMessage(UIEV_LOSEMOUSEOVER,p1,p2,si);
			return true;
		}
	case UIEV_LOSEFOCUS:
		{
			SendChildMessage(UIEV_LOSEFOCUS,p1,p2,si);
			DefaultSendMessage(message,p1,p2,si);
			return true;
		}
	default:
		return DefaultSendMessage(message,p1,p2,si);
	}
}