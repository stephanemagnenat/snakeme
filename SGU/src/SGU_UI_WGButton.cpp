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

#include "SGU_UI_WGButton.h"

// C standard
#include <string.h>

// Normal Button
CUINormalButton::CUINormalButton(int ix,int iy,int iw,int ih,CUITheme *ithm,SGU_Sprite *ifnt, const char *s,bool cnt)
{
	x=ix;
	y=iy;
	w=iw;
	h=ih;
	theme=ithm;
	font=ifnt;
	caption=s;
	centered=cnt;
	if (centered)
	{
		int l=0;
		for (int i=0;i<(int)strlen(s);i++)
		{
			l+=font->GetW(s[i]-32);
		}
		dx=(w-l)>>1;
	}
	else
	{
		dx=2;
	}

}

void CUINormalButton::SetCaption(char *s)
{
	caption=s;
	if (centered)
	{
		dx=(w-SGU_GetTextWidth(font,s))>>1;
	}
	else
	{
		dx=2;
	}
}


bool CUINormalButton::SendMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si)
{
	switch (message)
	{
	case UIEV_DRAW:
		{
			// Save for focus
			DefaultSendMessage(UIEV_DRAW,0,0,si);
			SGU_FillRectAlpha(si,x,y,w,h,theme->WgtBack);
			// we have no mouse over
			SGU_RectShade(si,x,y,w,h,theme->BHiCol,theme->BMidCol,theme->BLowCol);
			// draw text
			SGU_DisplayText(si,font,x+dx,y+2,caption);
			return true;
		}
	case UIEV_LBUTTONDOWN:
		{
			SGU_RectShade(si,x,y,w,h,theme->FLowCol,theme->FMidCol,theme->FHiCol);
			return true;
		}
	case UIEV_LBUTTONUP:
		{
			int mx,my;
			GETMOUSEPOS(p1,&mx,&my);
			if (IsPtOnObject(mx,my))
			{
				SGU_RectShade(si,x,y,w,h,theme->FHiCol,theme->FMidCol,theme->FLowCol);
			}
			else
			{
				SGU_RectShade(si,x,y,w,h,theme->BHiCol,theme->BMidCol,theme->BLowCol);
			}
			return true;
		}
	case UIEV_GETMOUSEOVER:
		{
			// if there is a button down, we have focus
			if (p2!=0)
				SGU_RectShade(si,x,y,w,h,theme->FLowCol,theme->FMidCol,theme->FHiCol);
			else
				SGU_RectShade(si,x,y,w,h,theme->FHiCol,theme->FMidCol,theme->FLowCol);
			return true;
		}
	case UIEV_LOSEMOUSEOVER:
		{
			SGU_RectShade(si,x,y,w,h,theme->BHiCol,theme->BMidCol,theme->BLowCol);
			return true;
		}
	default:
		return DefaultSendMessage(message,p1,p2,si);
	}
}

// Graphics buttons
CUIGfxButton::CUIGfxButton(int ix,int iy,int iw,int ih,CUITheme *ithm,SGU_Sprite *ispr, int f,bool ids)
{
	x=ix;
	y=iy;
	w=iw;
	h=ih;
	theme=ithm;
	sprite=ispr;
	frame=f;
	ds=ids;
}

bool CUIGfxButton::SendMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si)
{
	switch (message)
	{
	case UIEV_DRAW:
		{
			// Save for focus
			DefaultSendMessage(UIEV_DRAW,0,0,si);
			SGU_FillRectAlpha(si,x,y,w,h,theme->WgtBack);
			// we have no focus
			SGU_RectShade(si,x,y,w,h,theme->BHiCol,theme->BMidCol,theme->BLowCol);
			// draw sprite
			if (ds)
			{
				SGU_DoubleSizeBlit(sprite->GetFrameSurface(frame),si,x+1,y+1);
			}
			else
			{
				sprite->DisplayClipped(si,x+1,y+1,w-2,h-2,frame);
			}
			return true;
		}
	case UIEV_LBUTTONDOWN:
		{
			SGU_RectShade(si,x,y,w,h,theme->FLowCol,theme->FMidCol,theme->FHiCol);
			return true;
		}
	case UIEV_LBUTTONUP:
		{
			int mx,my;
			GETMOUSEPOS(p1,&mx,&my);
			if (IsPtOnObject(mx,my))
			{
				SGU_RectShade(si,x,y,w,h,theme->FHiCol,theme->FMidCol,theme->FLowCol);
			}
			else
			{
				SGU_RectShade(si,x,y,w,h,theme->BHiCol,theme->BMidCol,theme->BLowCol);
			}
			return true;
		}
	case UIEV_GETMOUSEOVER:
		{
			// if there is a button down, we have focus
			if (p2!=0)
				SGU_RectShade(si,x,y,w,h,theme->FLowCol,theme->FMidCol,theme->FHiCol);
			else
				SGU_RectShade(si,x,y,w,h,theme->FHiCol,theme->FMidCol,theme->FLowCol);
			return true;
		}
	case UIEV_LOSEMOUSEOVER:
		{
			SGU_RectShade(si,x,y,w,h,theme->BHiCol,theme->BMidCol,theme->BLowCol);
			return true;
		}
	default:
		return DefaultSendMessage(message,p1,p2,si);
	}
}

// Graphics buttons
CUIMultiGfxButton::CUIMultiGfxButton(int ix,int iy,int iw,int ih,CUITheme *ithm,SGU_Sprite *ispr, bool ids)
{
	x=ix;
	y=iy;
	w=iw;
	h=ih;
	theme=ithm;
	sprite=ispr;
	frame=0;
	ds=ids;
}

bool CUIMultiGfxButton::SendMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si)
{
	switch (message)
	{
	case UIEV_DRAW:
		{
			// Save for focus
			DefaultSendMessage(UIEV_DRAW,0,0,si);
			SGU_FillRectAlpha(si,x,y,w,h,theme->WgtBack);
			frameregion.SaveRegion(si,x+1,y+1,w-2,h-2);
			// we have no focus
			SGU_RectShade(si,x,y,w,h,theme->BHiCol,theme->BMidCol,theme->BLowCol);
			SendMessage(INTERNEV_REDRAW,0,0,si);
			return true;
		}
	case INTERNEV_REDRAW:
		{
			frameregion.RestaurRegion(si);
			// draw sprite
			if (ds)
			{
				SGU_DoubleSizeBlit(sprite->GetFrameSurface(frame),si,x+1,y+1);
			}
			else
			{
				sprite->DisplayClipped(si,x+1,y+1,w-2,h-2,frame);
			}
			return true;
		}
	case UIEV_LBUTTONDOWN:
		{
			frame=(frame+1)%sprite->GetNbFrames();
			SGU_RectShade(si,x,y,w,h,theme->FLowCol,theme->FMidCol,theme->FHiCol);
			SendMessage(INTERNEV_REDRAW,0,0,si);
			return true;
		}
	case UIEV_LBUTTONUP:
		{
			int mx,my;
			GETMOUSEPOS(p1,&mx,&my);
			if (IsPtOnObject(mx,my))
			{
				SGU_RectShade(si,x,y,w,h,theme->FHiCol,theme->FMidCol,theme->FLowCol);
			}
			else
			{
				SGU_RectShade(si,x,y,w,h,theme->BHiCol,theme->BMidCol,theme->BLowCol);
			}
			return true;
		}
	case UIEV_GETMOUSEOVER:
		{
			// if there is a button down, we have focus
			if (p2!=0)
				SGU_RectShade(si,x,y,w,h,theme->FLowCol,theme->FMidCol,theme->FHiCol);
			else
				SGU_RectShade(si,x,y,w,h,theme->FHiCol,theme->FMidCol,theme->FLowCol);
			return true;
		}
	case UIEV_LOSEMOUSEOVER:
		{
			SGU_RectShade(si,x,y,w,h,theme->BHiCol,theme->BMidCol,theme->BLowCol);
			return true;
		}
	case UIEV_KEYDOWN:
		{
			if (p1==SDLK_RETURN)
			{
				frame=(frame+1)%sprite->GetNbFrames();
				this->SendMessage(INTERNEV_REDRAW,0,0,si);
				return true;
			}
		}
	default:
		return DefaultSendMessage(message,p1,p2,si);
	}
}

// Sign Button
CUISignButton::CUISignButton(int ix,int iy,int iw,int ih,CUITheme *ithm,int isn)
{
	x=ix;
	y=iy;
	w=iw;
	h=ih;
	theme=ithm;
	sn=isn;
}

bool CUISignButton::SendMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si)
{
	switch (message)
	{
	case UIEV_DRAW:
		{
			// Save for focus
			DefaultSendMessage(UIEV_DRAW,0,0,si);
			SGU_FillRectAlpha(si,x,y,w,h,theme->WgtBack);
			// we have no mouse over
			SGU_RectShade(si,x,y,w,h,theme->BHiCol,theme->BMidCol,theme->BLowCol);
			// draw sign
			switch (sn)
			{
			case SB_ARRAY_RIGHT:
				{
					int dlarg=h-(SB_BORDER_SPACE<<1);
					int dlng=(dlarg>>1)+1;
					// we always fit
					if (dlng>w-(SB_BORDER_SPACE<<1))
					{
						dlng=w-(SB_BORDER_SPACE<<1);
						dlarg=(dlng<<1)-1;
					}
					int xm=x+(w>>1);
					int ym=y+(h>>1);
					int startx=x+((w-dlng)>>1);
					int dl=0;
					for (int dx=startx+dlng-1;dx>=startx;dx--)
					{
						SGU_VertLine(si,dx,ym-dl,(2*dl)+1,theme->FHiCol);
						dl++;
					}
				}
				break;
			case SB_ARRAY_LEFT:
				{
					int dlarg=h-(SB_BORDER_SPACE<<1);
					int dlng=(dlarg>>1)+1;
					// we always fit
					if (dlng>w-(SB_BORDER_SPACE<<1))
					{
						dlng=w-(SB_BORDER_SPACE<<1);
						dlarg=(dlng<<1)-1;
					}
					int xm=x+(w>>1);
					int ym=y+(h>>1);
					int startx=x+((w-dlng)>>1);
					int dl=0;
					for (int dx=startx;dx<startx+dlng;dx++)
					{
						SGU_VertLine(si,dx,ym-dl,(2*dl)+1,theme->FHiCol);
						dl++;
					}				
				}
				break;
			case SB_ARRAY_UP:
				{
					int dlarg=w-(SB_BORDER_SPACE<<1);
					int dlng=(dlarg>>1)+1;
					// we always fit
					if (dlng>h-(SB_BORDER_SPACE<<1))
					{
						dlng=h-(SB_BORDER_SPACE<<1);
						dlarg=(dlng<<1)-1;
					}
					int xm=x+(w>>1);
					int ym=y+(h>>1);
					int starty=y+((h-dlng)>>1);
					int dl=0;
					for (int dy=starty;dy<starty+dlng;dy++)
					{
						SGU_HorzLine(si,xm-dl,dy,(2*dl)+1,theme->FHiCol);
						dl++;
					}
				}
				break;
			case SB_ARRAY_DOWN:
				{
					int dlarg=w-(SB_BORDER_SPACE<<1);
					int dlng=(dlarg>>1)+1;
					// we always fit
					if (dlng>h-(SB_BORDER_SPACE<<1))
					{
						dlng=h-(SB_BORDER_SPACE<<1);
						dlarg=(dlng<<1)-1;
					}
					int xm=x+(w>>1);
					int ym=y+(h>>1);
					int starty=y+((h-dlng)>>1);
					int dl=0;
					for (int dy=starty+dlng-1;dy>=starty;dy--)
					{
						SGU_HorzLine(si,xm-dl,dy,(2*dl)+1,theme->FHiCol);
						dl++;
					}
				}
				break;
			}
			return true;
		}
	case UIEV_LBUTTONDOWN:
		{
			SGU_RectShade(si,x,y,w,h,theme->FLowCol,theme->FMidCol,theme->FHiCol);
			return true;
		}
	case UIEV_LBUTTONUP:
		{
			int mx,my;
			GETMOUSEPOS(p1,&mx,&my);
			if (IsPtOnObject(mx,my))
			{
				SGU_RectShade(si,x,y,w,h,theme->FHiCol,theme->FMidCol,theme->FLowCol);
			}
			else
			{
				SGU_RectShade(si,x,y,w,h,theme->BHiCol,theme->BMidCol,theme->BLowCol);
			}
			return true;
		}
	case UIEV_GETMOUSEOVER:
		{
			// if there is a button down, we have focus
			if (p2!=0)
				SGU_RectShade(si,x,y,w,h,theme->FLowCol,theme->FMidCol,theme->FHiCol);
			else
				SGU_RectShade(si,x,y,w,h,theme->FHiCol,theme->FMidCol,theme->FLowCol);
			return true;
		}
	case UIEV_LOSEMOUSEOVER:
		{
			SGU_RectShade(si,x,y,w,h,theme->BHiCol,theme->BMidCol,theme->BLowCol);
			return true;
		}
	default:
		return DefaultSendMessage(message,p1,p2,si);
	}
}
