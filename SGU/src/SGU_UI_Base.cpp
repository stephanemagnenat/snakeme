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

#include "SGU_UI_Base.h"
#include <string.h>
#include <stdlib.h>

// Light Button
/*
CUILightButton::CUILightButton(int ix,int iy,int iw,int ih,CUITheme *ithm,SGU_Sprite *ifnt, char *s,bool cnt)
{
	theme=ithm;
	x=ix;
	y=iy;
	w=iw;
	h=ih;
	font=ifnt;7
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

bool CUILightButton::SendMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si)
{
	switch (message)
	{
	case UIEV_DRAW:
		{
			SGU_DisplayText(si,font,x+dx,y+2,caption);
			topregion.SaveRegion(si,x,y,w+1,1);
			bottomregion.SaveRegion(si,x,y+h,w+1,1);
			return true;
		}
	case UIEV_GETMOUSEOVER:
		{
			SGU_HorzLineShade(si,x,y,x+w,theme->FHiCol,theme->FLowCol);
			SGU_HorzLineShade(si,x,y+h,x+w,theme->FHiCol,theme->FLowCol);
			return true;
		}
	case UIEV_LOOSEMOUSEOVER:
		{
			topregion.RestaurRegion(si);
			bottomregion.RestaurRegion(si);
			return true;
		}
	default:
		return false;
	}
}


void CUILightButton::SetCaption(char *s)
{
	caption=s;
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
*/

// Process messages
bool CUIObject::DefaultSendMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si)
{
	switch (message)
	{
	case UIEV_DRAW:
		{
			topregion.SaveRegion(si,x-1,y-1,w+2,1);
			bottomregion.SaveRegion(si,x-1,y+h,w+2,1);
			leftregion.SaveRegion(si,x-1,y,1,h);
			rightregion.SaveRegion(si,x+w,y,1,h);
			return true;
		}
	case UIEV_GETFOCUS:
		{
			SGU_RectShade(si,x-1,y-1,w+2,h+2,theme->FHiCol,theme->FMidCol,theme->FLowCol);
			//SGU_Rect(si,x-1,y-1,w+2,h+2,SGU_RGB(128,128,128,0));
			hasfocus=true;
			return true;
		}
	case UIEV_LOSEFOCUS:
		{
			topregion.RestaurRegion(si);
			bottomregion.RestaurRegion(si);
			leftregion.RestaurRegion(si);
			rightregion.RestaurRegion(si);
			hasfocus=false;
			return true;
		}
	default:
		return false;
	}	

}


/*

CUISlideBar::CUISlideBar(int ix,int iy, int iw,int ih,CUITheme *ithm,int totval,int nbvalons,int aw,bool vert)
{
	x=ix;
	y=iy;
	w=iw;
	h=ih;
	theme=ithm;
	totalval=totval;
	vertical=vert;
	nbvalonscreen=nbvalons;
	arrowwidth=aw;
	basevalue=0;
	if (nbvalons<totval)
	{
		if (vertical)
		{
			slidewidth=((h-2*arrowwidth-4)*nbvalonscreen)/totalval;
		}
		else
		{
			slidewidth=((w-2*arrowwidth-4)*nbvalonscreen)/totalval;
		}
	}
	else
	{
		slidewidth=-1;
	}
}

bool CUISlideBar::SendMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si)
{
switch (message)
	{
	// Note : to do
	case UIEV_KEYDOWN:
		{
			if ((p1==SDLK_LEFT) && (!vertical))
			{

			}
		}
	case UIEV_DRAW:
		{
			// Save for focus
			DefaultSendMessage(UIEV_DRAW,0,0,si);
			// save background
			SGU_FillRectAlpha(si,x+1,y+1,w-1,h-1,theme->WgtBack);
			// draw rectangle
			SGU_RectShade(si,x,y,w,h,theme->BHiCol,theme->BMidCol,theme->BLowCol);
			if (vertical)
			{
				// save region
				slideregion.SaveRegion(si,x+1,y+arrowwidth+1,w-2,h-2*arrowwidth-2);
				// draw bar
				if (slidewidth>-1)
				{
					SGU_FillRectAlpha(si,x,y+arrowwidth,w,slidewidth,theme->WgtBack);
				}
				SGU_HorzLineShade(si,x+1,y+arrowwidth,w-2,theme->BHiCol,theme->BMidCol);
				SGU_HorzLineShade(si,x+1,y+h-arrowwidth,w-2,theme->BMidCol,theme->BLowCol);
				// draw arrow
				int dw=w-4;
				int dh=arrowwidth-4;
				SGU_Line(si,x+2+(dw>>1),y+2,x+2+dw,y+2+dh,theme->FHiCol);
				SGU_Line(si,x+2+(dw>>1),y+2,x+2,y+2+dh,theme->FHiCol);
				SGU_Line(si,x+2,y+2+dh,x+2+dw,y+2+dh,theme->FHiCol);
				SGU_Line(si,x+2,y+h-2-dh,x+2+dw,y+h-2-dh,theme->FHiCol);
				SGU_Line(si,x+2+(dw>>1),y+h-2,x+2+dw,y+h-2-dh,theme->FHiCol);
				SGU_Line(si,x+2+(dw>>1),y+h-2,x+2,y+h-2-dh,theme->FHiCol);
			}
			else
			{
				// save region
				slideregion.SaveRegion(si,x+arrowwidth+1,y+1,w-2*arrowwidth-2,h-2);
				// draw bar
				if (slidewidth>-1)
				{
					SGU_FillRectAlpha(si,x+arrowwidth,y,slidewidth,h,theme->WgtBack);
				}
				SGU_VertLineShade(si,x+arrowwidth,y+1,h-2,theme->BHiCol,theme->BMidCol);
				SGU_VertLineShade(si,x+w-arrowwidth,y+1,h-2,theme->BMidCol,theme->BLowCol);
				// draw arrow
				int dh=h-4;
				int dw=arrowwidth-4;
				SGU_Line(si,x+2,y+2+(dh>>1),x+2+dw,y+2,theme->FHiCol);
				SGU_Line(si,x+2,y+2+(dh>>1),x+2+dw,y+2+dh,theme->FHiCol);
				SGU_Line(si,x+2+dw,y+2,x+2+dw,y+2+dh,theme->FHiCol);
				SGU_Line(si,x+w-2,y+2+(dh>>1),x+w-2-dw,y+2,theme->FHiCol);
				SGU_Line(si,x+w-2,y+2+(dh>>1),x+w-2-dw,y+2+dh,theme->FHiCol);
				SGU_Line(si,x+w-2-dw,y+2,x+w-2-dw,y+2+dh,theme->FHiCol);
			}
			return true;
		}
	case UIEV_GETMOUSEOVER:
		{
			SGU_RectShade(si,x,y,w,h,theme->FHiCol,theme->BMidCol,theme->FLowCol);
			return true;
		}
	case UIEV_LOSEMOUSEOVER:
		{
			SGU_RectShade(si,x,y,w,h,theme->BHiCol,theme->BMidCol,theme->BLowCol);
			return true;
		}
	case UIEV_LBUTTONDOWN:
		{
			if (vertical)
			{
				// do scroll			
				if (((p2-y)<arrowwidth) && (basevalue>0))
					basevalue--;
				if (((y+h-p2)<arrowwidth) && (basevalue<totalval-nbvalonscreen))
					basevalue++;
				if (slidewidth>-1)
				{
					// redraw background
					slideregion.RestaurRegion(si);
					// draw slide
					//SGU_RectShade(si,x+1,y+arrowwidth+1+(basevalue*(h-(2*arrowwidth+2)-slidewidth))/(totalval-nbvalonscreen),w-2,slidewidth,theme->BHiCol,theme->BMidCol,theme->BLowCol);
				}
			}
			else
			{
				// do scroll			
				if (((p1-x)<arrowwidth) && (basevalue>0))
					basevalue--;
				if (((x+w-p1)<arrowwidth) && (basevalue<totalval-nbvalonscreen))
					basevalue++;
				if (slidewidth>-1)
				{
					// redraw background
					slideregion.RestaurRegion(si);
					// draw slide
					SGU_RectShade(si,x+arrowwidth+1+(basevalue*(w-(2*arrowwidth+2)-slidewidth))/(totalval-nbvalonscreen),y+1,slidewidth,h-2,theme->BHiCol,theme->BMidCol,theme->BLowCol);
				}
			}
			return true;
		}
	default:
		return DefaultSendMessage(message,p1,p2,si);
	}
}

CUIGenericList::CUIGenericList(int ix,int iy, int iw,int ih,CUITheme *ithm,int nbel,int elw,bool vert,int slidew)
{
	x=ix;
	y=iy;
	w=iw;
	h=ih;
	theme=ithm;
	vertical=vert;
	nbelement=nbel;
	elwidth=elw+2;
	sw=slidew;
	selel=-1;
	if (vertical)
	{
		elfit=h/elwidth;
		slidebar = new CUISlideBar(x,y,slidew,h,theme,nbelement,elfit,sw,true);
	}
	else
	{
		elfit=w/elwidth;
		slidebar = new CUISlideBar(x,y,w,slidew,theme,nbelement,elfit,sw,false);
	}
}

bool CUIGenericList::SendMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si)
{
	switch (message)
	{
	case UIEV_DRAW:
		{
			// Save for focus
			DefaultSendMessage(UIEV_DRAW,0,0,si);
			slidebar->SendMessage(UIEV_DRAW,0,0,si);
			if (vertical)
			{
				// save background
				SGU_FillRectAlpha(si,x+2+sw,y,w-sw-2,h,theme->WgtBack);
				listregion.SaveRegion(si,x+3+sw,y+1,w-sw-4,h-2);
				// draw rectangle
				SGU_RectShade(si,x+2+sw,y,w-sw-2,h,theme->BHiCol,theme->BMidCol,theme->BLowCol);
				for (int n=0;n<MIN(elfit,nbelement);n++)
				{
					DisplayListElement(slidebar->GetBaseValue()+n,x+sw+4,y+1+(n*elwidth),si);
				}
				// redraw selrect
				if ((selel-slidebar->GetBaseValue()>=0) && (selel-slidebar->GetBaseValue()<elfit))
				{
					SGU_RectShade(si,x+sw+4,y+1+(selel-slidebar->GetBaseValue())*elwidth,w-sw-6,elwidth-2,theme->FHiCol,theme->FMidCol,theme->FLowCol);
				}
			}
			else
			{
				// save background
				SGU_FillRectAlpha(si,x,y+2+sw,w,h-sw-2,theme->WgtBack);
				listregion.SaveRegion(si,x+1,y+3+sw,w-2,h-sw-4);
				// draw rectangle
				SGU_RectShade(si,x,y+2+sw,w,h-sw-2,theme->BHiCol,theme->BMidCol,theme->BLowCol);
				for (int n=0;n<MAX(elfit,nbelement);n++)
				{
					DisplayListElement(slidebar->GetBaseValue()+n,x+1+(n*elwidth),y+4+sw,si);
				}
				// redraw selrect
				if ((selel-slidebar->GetBaseValue()>=0) && (selel-slidebar->GetBaseValue()<elfit))
				{
					SGU_RectShade(si,x+(selel-slidebar->GetBaseValue())*elwidth,y+sw+3,elwidth-2,h-sw-4,theme->FHiCol,theme->FMidCol,theme->FLowCol);
				}
			}
			return true;
		}
	case UIEV_GETMOUSEOVER:
		{
			if (vertical)
				SGU_RectShade(si,x+2+sw,y,w-sw-2,h,theme->FHiCol,theme->FMidCol,theme->FLowCol);
			else
				SGU_RectShade(si,x,y+2+sw,w,h-sw-2,theme->FHiCol,theme->FMidCol,theme->FLowCol);
			slidebar->SendMessage(UIEV_GETMOUSEOVER,0,0,si);
			return true;
		}
	case UIEV_LOSEMOUSEOVER:
		{
			if (vertical)
				SGU_RectShade(si,x+2+sw,y,w-sw-2,h,theme->BHiCol,theme->BMidCol,theme->BLowCol);
			else
				SGU_RectShade(si,x,y+2+sw,w,h-sw-2,theme->BHiCol,theme->BMidCol,theme->BLowCol);
			slidebar->SendMessage(UIEV_LOSEMOUSEOVER,0,0,si);
			return true;
		}
	case UIEV_LBUTTONDOWN:
		{
			if (vertical)
			{
				// if left
				if ((p1-x)<sw)
				{
					slidebar->SendMessage(UIEV_LBUTTONDOWN,p1,p2,si);
				}
				else
				{
					selel=((p2-y)/elwidth)+slidebar->GetBaseValue();
					if (selel>=nbelement)
						selel=-1;
				}
				// redraw background
				listregion.RestaurRegion(si);
				// redraw elements
				for (int n=0;n<MIN(elfit,nbelement);n++)
				{
					DisplayListElement(slidebar->GetBaseValue()+n,x+sw+4,y+1+(n*elwidth),si);
				}
				// redraw selrect
				if ((selel-slidebar->GetBaseValue()>=0) && (selel-slidebar->GetBaseValue()<elfit))
				{
					SGU_RectShade(si,x+sw+4,y+1+(selel-slidebar->GetBaseValue())*elwidth,w-sw-6,elwidth-2,theme->FHiCol,theme->FMidCol,theme->FLowCol);
				}
			}
			else
			{
				if ((p2-y)<sw)
				{
					slidebar->SendMessage(UIEV_LBUTTONDOWN,p1,p2,si);
				}
				else
				{
					selel=((p1-x)/elwidth)+slidebar->GetBaseValue();
					if (selel>=nbelement)
						selel=-1;
				}
				// redraw background
				listregion.RestaurRegion(si);
				// redraw elements
				for (int n=0;n<MIN(elfit,nbelement);n++)
				{
					DisplayListElement(slidebar->GetBaseValue()+n,x+1+(n*elwidth),y+4+sw,si);
				}
				// redraw selrect
				if ((selel-slidebar->GetBaseValue()>=0) && (selel-slidebar->GetBaseValue()<elfit))
				{
					SGU_RectShade(si,x+(selel-slidebar->GetBaseValue())*elwidth,y+sw+3,elwidth-2,h-sw-4,theme->FHiCol,theme->FMidCol,theme->FLowCol);
				}
			}
			return true;
		}
	default:
		return DefaultSendMessage(message,p1,p2,si);
	}
}


void CUIFrameList::DisplayListElement(int eln,int x,int y,SDL_Surface *si)
{
	sprite->Display(si,x,y-1,eln);
};

void CUIStringList::DisplayListElement(int eln,int x,int y,SDL_Surface *si)
{
	SGU_DisplayText(si,font,x,y,(char *)(strings->GetElementN(eln)));
};
*/

bool CUIBoolChooser::SendMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si)
{
	switch (message)
	{
	case UIEV_DRAW:
		{
			// Save for focus
			DefaultSendMessage(UIEV_DRAW,0,0,si);
			this->SendMessage(INTERNEV_REDRAW,0,0,si);
			this->SendMessage(UIEV_LOSEMOUSEOVER,0,0,si);
			SGU_DisplayText(si,font,x+20,y+2,caption);
			return true;
		}
	case INTERNEV_REDRAW:
		{
			if (state)
			{
				SGU_FillRect(si,x+1,y+1,18,18,SGU_RGB(0,255,0,0));
			}
			else
			{
				SGU_FillRect(si,x+1,y+1,18,18,SGU_RGB(255,0,0,0));
			}
			return true;
		}
	case UIEV_LBUTTONDOWN:
		{
			state=!state;
			this->SendMessage(INTERNEV_REDRAW,0,0,si);
			this->SendMessage(UIEV_GETMOUSEOVER,0,0,si);
			return true;
		}
	case UIEV_KEYDOWN:
		{
			if (p1==SDLK_RETURN)
			{
				state=!state;
				this->SendMessage(INTERNEV_REDRAW,0,0,si);
				return true;
			}
			else
				return false;
		}
	case UIEV_GETMOUSEOVER:
		{
			if (state)
			{
				SGU_RectShade(si,x,y,20,20,theme->FLowCol,theme->FMidCol,theme->FHiCol);
			}
			else
			{
				SGU_RectShade(si,x,y,20,20,theme->FHiCol,theme->FMidCol,theme->FLowCol);
			}
			return true;
		}
	case UIEV_LOSEMOUSEOVER:
		{
			if (state)
			{
				SGU_RectShade(si,x,y,20,20,theme->BLowCol,theme->BMidCol,theme->BHiCol);
			}
			else
			{
				SGU_RectShade(si,x,y,20,20,theme->BHiCol,theme->BMidCol,theme->BLowCol);
			}
			return true;
		}
	default:
		return DefaultSendMessage(message,p1,p2,si);
	}
}

/*
CUIIntChooser::CUIIntChooser(int ix,int iy,int iw,int ih,CUITheme *ithm,SGU_RGBA icb,SGU_Sprite *ifnt,int minv,int maxv,int actv,int aw,bool vert)
{
	x=ix;
	y=iy;
	w=iw;
	h=ih;
	theme=ithm;
	cbar=icb;
	minval=minv;
	maxval=maxv;
	value=actv;
	arrowwidth=aw;
	vertical=vert;
}

bool CUIIntChooser::SendMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si)
{
	switch (message)
	{
	case UIEV_DRAW:
		{
			// Save for focus
			DefaultSendMessage(UIEV_DRAW,0,0,si);
			// save background
			SGU_RGBA temp;
			temp=theme->WgtBack;
			temp.A=195;
			SGU_FillRectAlpha(si,x,y,w,h,temp);
			// draw rectangle
			SGU_RectShade(si,x,y,w,h,theme->BHiCol,theme->BMidCol,theme->BLowCol);
			if (vertical)
			{
				// save region
				slideregion.SaveRegion(si,x+1,y+arrowwidth+1,w-2,h-2*arrowwidth-2);
				// draw bar
				//SGU_FillRectAlpha(si,x+1,y+arrowwidth+2,w-2,((h-2*arrowwidth-1)*(value-minval))/(maxval-minval)-2,theme->WgtBack);
				//SGU_RectShade(si,x+1,y+arrowwidth+2,w-2,((h-2*arrowwidth-1)*(value-minval))/(maxval-minval)-2,theme->BHiCol,theme->BMidCol,theme->BLowCol);
				SGU_FillRectAlpha(si,x+1,y+arrowwidth+2,w-2,((h-2*arrowwidth-1)*(value-minval))/(maxval-minval)-2,theme->WgtBack);
				SGU_RectShade(si,x+1,y+arrowwidth+2,w-2,((h-2*arrowwidth-1)*(value-minval))/(maxval-minval)-2,theme->BHiCol,theme->BMidCol,theme->BLowCol);
				
				//DrawFillRect(si,x+1,y+arrowwidth+1,w-1,((h-2*arrowwidth-1)*(value-minval))/(maxval-minval),cbar);
				SGU_HorzLineShade(si,x+1,y+arrowwidth,w-2,theme->BHiCol,theme->BMidCol);
				SGU_HorzLineShade(si,x+1,y+h-arrowwidth,w-2,theme->BMidCol,theme->BLowCol);
				// draw arrow
				int dw=w-4;
				int dh=arrowwidth-4;
				SGU_Line(si,x+2+(dw>>1),y+2,x+2+dw,y+2+dh,theme->FHiCol);
				SGU_Line(si,x+2+(dw>>1),y+2,x+2,y+2+dh,theme->FHiCol);
				SGU_Line(si,x+2,y+2+dh,x+2+dw,y+2+dh,theme->FHiCol);
				SGU_Line(si,x+2,y+h-2-dh,x+2+dw,y+h-2-dh,theme->FHiCol);
				SGU_Line(si,x+2+(dw>>1),y+h-2,x+2+dw,y+h-2-dh,theme->FHiCol);
				SGU_Line(si,x+2+(dw>>1),y+h-2,x+2,y+h-2-dh,theme->FHiCol);
			}
			else
			{
				// save region
				slideregion.SaveRegion(si,x+arrowwidth+1,y+1,w-2*arrowwidth-2,h-2);
				// draw bar
				SGU_FillRectAlpha(si,x+arrowwidth+2,y+1,((w-2*arrowwidth-1)*(value-minval))/(maxval-minval)-2,h-3,theme->WgtBack);
				SGU_RectShade(si,x+arrowwidth+2,y+1,((w-2*arrowwidth-1)*(value-minval))/(maxval-minval)-2,h-2,theme->BHiCol,theme->BMidCol,theme->BLowCol);
				SGU_VertLineShade(si,x+arrowwidth,y+1,h-2,theme->BHiCol,theme->BMidCol);
				SGU_VertLineShade(si,x+w-arrowwidth,y+1,h-2,theme->BMidCol,theme->BLowCol);
				// draw arrow
				int dh=h-4;
				int dw=arrowwidth-4;
				SGU_Line(si,x+2,y+2+(dh>>1),x+2+dw,y+2,theme->FHiCol);
				SGU_Line(si,x+2,y+2+(dh>>1),x+2+dw,y+2+dh,theme->FHiCol);
				SGU_Line(si,x+2+dw,y+2,x+2+dw,y+2+dh,theme->FHiCol);
				SGU_Line(si,x+w-2,y+2+(dh>>1),x+w-2-dw,y+2,theme->FHiCol);
				SGU_Line(si,x+w-2,y+2+(dh>>1),x+w-2-dw,y+2+dh,theme->FHiCol);
				SGU_Line(si,x+w-2-dw,y+2,x+w-2-dw,y+2+dh,theme->FHiCol);
			}
			return true;
		}
	case UIEV_GETMOUSEOVER:
		{
			SGU_RectShade(si,x,y,w,h,theme->FHiCol,theme->FMidCol,theme->FLowCol);
			return true;
		}
	case UIEV_LOSEMOUSEOVER:
		{
			SGU_RectShade(si,x,y,w,h,theme->BHiCol,theme->BMidCol,theme->BLowCol);
			return true;
		}
	case UIEV_LBUTTONDOWN:
		{
			if (vertical)
			{
				// do scroll			
				if (((p2-y)<arrowwidth) && (value>minval))
					value--;
				if (((y+h-p2)<arrowwidth) && (value<maxval))
					value++;
				// redraw background
				slideregion.RestaurRegion(si);
				// draw slide
				SGU_FillRectAlpha(si,x+1,y+arrowwidth+2,w-2,((h-2*arrowwidth-1)*(value-minval))/(maxval-minval)-2,theme->WgtBack);
				SGU_RectShade(si,x+1,y+arrowwidth+2,w-2,((h-2*arrowwidth-1)*(value-minval))/(maxval-minval)-2,theme->BHiCol,theme->BMidCol,theme->BLowCol);
			}
			else
			{
				// do scroll			
				if (((p1-x)<arrowwidth) && (value>minval))
					value--;
				if (((x+w-p1)<arrowwidth) && (value<maxval))
					value++;
				// redraw background
				slideregion.RestaurRegion(si);
				// draw slide
				SGU_FillRectAlpha(si,x+arrowwidth+2,y+1,((w-2*arrowwidth-1)*(value-minval))/(maxval-minval)-2,h-3,theme->WgtBack);
				SGU_RectShade(si,x+arrowwidth+2,y+1,((w-2*arrowwidth-1)*(value-minval))/(maxval-minval)-2,h-2,theme->BHiCol,theme->BMidCol,theme->BLowCol);
			}
			return true;
		}
	default:
		return DefaultSendMessage(message,p1,p2,si);
	}
}
*/
// Screen
int CUIScreen::GetMouseOverWidget(int px, int py)
{
	for (int n=0;n<nobject;n++)
	{
		if (objects[n]->IsPtOnObject(px,py))
		{
			mouseoverwgt=n;
			return n;
		}
	}
	mouseoverwgt=-1;
	return -1;
}

void CUIScreen::DrawScreen(SDL_Surface *ScrInfo)
{
	for (int n=0;n<nobject;n++)
	{
		objects[n]->SendMessage(UIEV_DRAW,0,0,ScrInfo);
	}
	if (firstdraw)
	{
		firstdraw=false;
		focuswgt=0;
	}
	objects[focuswgt]->SendMessage(UIEV_GETFOCUS,0,0,ScrInfo);
}


// Only the focused Window get the event, with an exception
// for the GETMOUSEOVER and LOSEMOUSEOVER
void CUIScreen::ProcessDefaultEvent(SDL_Event *ev,SDL_Surface *ScrInfo)
{
	int mx,my;

	clicked=-1;
	keyhit=-1;
	switch (ev->type)
	{
	case SDL_MOUSEMOTION:
		{
			mx=ev->motion.x;
			my=ev->motion.y;
			UIRedraw.newitem=GetMouseOverWidget(mx,my);
			if (UIRedraw.newitem!=UIRedraw.lastitem)
			{
				if (UIRedraw.lastitem>=0)
				{
					objects[UIRedraw.lastitem]->SendMessage(UIEV_LOSEMOUSEOVER,RETMOUSEPOS(mx,my),ev->motion.state,ScrInfo);
				}
				if (UIRedraw.newitem>=0)
				{
					// only send pushed button to selected
					if (UIRedraw.newitem==focuswgt)
						objects[UIRedraw.newitem]->SendMessage(UIEV_GETMOUSEOVER,RETMOUSEPOS(mx,my),ev->motion.state,ScrInfo);
					else
						objects[UIRedraw.newitem]->SendMessage(UIEV_GETMOUSEOVER,RETMOUSEPOS(mx,my),0,ScrInfo);
				}
				UIRedraw.lastitem=UIRedraw.newitem;
				objects[focuswgt]->SendMessage(UIEV_MOUSEMOVE,RETMOUSEPOS(mx,my),ev->motion.state,ScrInfo);
				SDL_UpdateRect(ScrInfo,0,0,0,0);
			}
			else
			{
				if (objects[focuswgt]->SendMessage(UIEV_MOUSEMOVE,RETMOUSEPOS(mx,my),ev->motion.state,ScrInfo))
					SDL_UpdateRect(ScrInfo,0,0,0,0);
			}
			break;
		}
	case SDL_KEYDOWN:
		{
			// test for tab, ignore if eaten
			if (!objects[focuswgt]->SendMessage(UIEV_KEYDOWN,ev->key.keysym.sym,0,ScrInfo))
			{
				if (ev->key.keysym.sym==SDLK_TAB)
				{
					Uint8 *keys;
					Uint8 mousestate=SDL_GetMouseState(&mx,&my);
					keys = SDL_GetKeyState(NULL);

					objects[focuswgt]->SendMessage(UIEV_LOSEFOCUS,0,0,ScrInfo);
					// SHIFT TAB pour -1, TAB pour +1
					if (keys[SDLK_LSHIFT] == SDL_PRESSED)
					{
						focuswgt=(focuswgt+nobject-1)%nobject;
					}
					else
					{
						focuswgt=(focuswgt+1)%nobject;
					}
					objects[focuswgt]->SendMessage(UIEV_GETFOCUS,RETMOUSEPOS(mx,my),mousestate,ScrInfo);
				}
				if (ev->key.keysym.sym==SDLK_RETURN)
				{
					clicked=focuswgt;
				}
			}
			char c=(char)ev->key.keysym.unicode;
			keyhit=focuswgt;
			objects[focuswgt]->SendMessage(UIEV_CHAR,c,0,ScrInfo);
			SDL_UpdateRect(ScrInfo,0,0,0,0);
			break;
		}
	case SDL_MOUSEBUTTONDOWN:
		{
			mx=ev->button.x;
			my=ev->button.y;
			int selunder=GetMouseOverWidget(mx,my);
			if (selunder>=0)
			{
				if (selunder!=focuswgt)
				{
					objects[focuswgt]->SendMessage(UIEV_LOSEFOCUS,RETMOUSEPOS(mx,my),ev->button.state,ScrInfo);
					focuswgt=selunder;
					objects[focuswgt]->SendMessage(UIEV_GETFOCUS,RETMOUSEPOS(mx,my),ev->button.state,ScrInfo);
				}
				objects[focuswgt]->SendMessage(UIEV_LBUTTONDOWN,RETMOUSEPOS(mx,my),ev->button.state,ScrInfo);
				SDL_UpdateRect(ScrInfo,0,0,0,0);
			}
			break;
		}
	case SDL_MOUSEBUTTONUP:
		{
			mx=ev->button.x;
			my=ev->button.y;
			if (GetMouseOverWidget(mx,my)==focuswgt)
				clicked=focuswgt;
			if (objects[focuswgt]->SendMessage(UIEV_LBUTTONUP,RETMOUSEPOS(mx,my),ev->button.state,ScrInfo))
				SDL_UpdateRect(ScrInfo,0,0,0,0);
		}
	}
}