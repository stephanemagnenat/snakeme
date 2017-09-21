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

#include "SGU_UI_WGText.h"

// C Standard
#include <string.h>

// Text Edit
CUITextEdit::CUITextEdit(int ix,int iy,int iw,int ih,CUITheme *ithm,SGU_Sprite *ifnt, const char *s,bool readonly)
{
	x=ix;
	y=iy;
	w=iw;
	h=ih;
	theme=ithm;
	font=ifnt;
	ro=readonly;
	strcpy(text,s);
	needredraw=true;
	cursor=0;
	textcount=strlen(text);
}


bool CUITextEdit::SendMessage(int message, ssize_t p1, ssize_t p2, SDL_Surface *si)
{
	switch (message)
	{
	case UIEV_DRAW:
		{
			// Save for focus
			DefaultSendMessage(UIEV_DRAW,0,0,si);
			SGU_FillRectAlpha(si,x,y,w,h,theme->WgtBack);
			SGU_RectShade(si,x,y,w,h,theme->BHiCol,theme->BMidCol,theme->BLowCol);
			textregion.SaveRegion(si,x+1,y+1,w-2,h-2);
			//SGU_DisplayText(si,font,x+2,y+2,text);
			SendMessage(INTERNEV_REDRAW,0,0,si);
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
	case UIEV_GETFOCUS:
		{
			DefaultSendMessage(UIEV_GETFOCUS,0,0,si);
			SendMessage(INTERNEV_REDRAW,0,0,si);
			return true;
		}
	case UIEV_LOSEFOCUS:
		{
			DefaultSendMessage(UIEV_LOSEFOCUS,0,0,si);
			SendMessage(INTERNEV_REDRAW,0,0,si);
			return true;
		}
	case UIEV_KEYDOWN:
		{
			switch (p1)
			{
			case SDLK_BACKSPACE:
				{
					if (cursor>0)
					{
						for (int n=cursor-1;n<textcount;n++)
						{
							text[n]=text[n+1];
						}
						text[textcount]=0;
						cursor--;
						textcount--;
						this->SendMessage(INTERNEV_REDRAW,0,0,si);
					}
					return true;
				}
				break;
			case SDLK_DELETE:
				{
					if (textcount>0)
					{
						for (int n=cursor;n<textcount;n++)
						{
							text[n]=text[n+1];
						}
						text[--textcount]=0;
						//textcount--;
						if (cursor>textcount)
							cursor--;
						this->SendMessage(INTERNEV_REDRAW,0,0,si);
					}
					return true;
				}
				break;
			case SDLK_LEFT:
				{
					if (cursor>0)
					{
						cursor--;
						this->SendMessage(INTERNEV_REDRAW,0,0,si);
					}
					return true;
				}
				break;
			case SDLK_RIGHT:
				{
					if (cursor<textcount)
					{
						cursor++;
						this->SendMessage(INTERNEV_REDRAW,0,0,si);
					}
					return true;
				}
				break;
			}
			return false;
		}
	case UIEV_CHAR:
		{
			if (!ro)
			{
				char c=(char)p1;
				// add char
				if ((textcount<256) && (c>31))
				{
					for (int n=textcount;n>cursor;n--)
					{
						text[n]=text[n-1];
					}
					text[cursor]=c;
					textcount++;
					cursor++;
					this->SendMessage(INTERNEV_REDRAW,0,0,si);
				}
				return true;
			}
			return false;
		}
		break;
	case INTERNEV_REDRAW:
		{
			textregion.RestaurRegion(si);
			text[textcount]=0;
			char *tp=text;
			int dep=0;
			// prevent overriding
			int deltalen;
			if (ro)
				deltalen=0;
			else
				deltalen=SGU_GetTextWidth(font,"_");
			while (SGU_GetTextWidth(font,tp)+deltalen>w-4)
			{
				tp++;
				dep++;
			}
			SGU_DisplayText(si,font,x+2,y+2,tp);
			if ((!ro) && (hasfocus))
			{
				// compute input displacement
				char temp[255];
				strcpy(temp,tp);
				if (dep<=cursor)
				{
					temp[cursor-dep]=0;
					SGU_DisplayText(si,font,x+2+SGU_GetTextWidth(font,temp),y+2,"_");
				}
			}
			needredraw=true;
			return true;
		}
		break;
	case INTERNEV_SETTEXT:
		{
			strcpy(text,(char *)p1);
			textcount=strlen(text);
			cursor=textcount;
			this->SendMessage(INTERNEV_REDRAW,0,0,si);
			return true;
		}
		break;
	default:
		return DefaultSendMessage(message,p1,p2,si);
	}
}


void CUITextEdit::SetText(const char *s)
{
	strcpy(text,s);
	textcount=strlen(text);
	cursor=textcount;
}
