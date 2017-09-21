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

#include "SGU_UI_WGFile.h"

#include <string.h>

// File Lister
void CUIFileLister::DisplayListElement(int eln,int x,int y,SDL_Surface *si)
{
	SGU_DisplayText(si,font,x,y,(char *)(strings.GetElementN(eln)));
};

void CUIFileLister::DeleteListContent(void)
{
	for(ListNode *c=strings.GetHead();c;c=c->Next)
	{
		free( (char *)(c->Element) );
	}
}

void CUIFileLister::Scan(char *iext)
{
	// delete previous content
	for(ListNode *c=strings.GetHead();c;c=c->Next)
	{
		free( (char *)(c->Element) );
	}
	strings.Clear();

	// reget files
	strcpy(ext,iext);
	{
		char *tempname;
		char *stemp;
		SGU_DirLister dir(ext);
		
		while ((tempname=dir.GetNextName())!=NULL)
		{
			if ( (strcmp(tempname,".")!=0) && (strcmp(tempname,"..")!=0) )
			{
				stemp=(char *)malloc((strlen(tempname)+1)*sizeof(char));
				strcpy(stemp,tempname);
				strings.AddElement(stemp);
			}
		}
	}
	ResetNbEl(strings.GetNbElement());
}


// File Chooser
bool CUIFileChooser::SendChildMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si)
{
	switch (message)
	{
	case UIEV_DRAW:
		{
			textzone->SendMessage(UIEV_DRAW,p1,p2,si);
		}
	case INTERNEV_REDRAW:
		{
			if (hideext)
			{		
				char temp[256];
				strcpy(temp,(char *)strings.GetElementN(GetVal()));
				for (int n=strlen(temp)-1;n>0;n--)
				{
					if (temp[n]=='.')
					{
						temp[n]=0;
						break;
					}
				}
				textzone->SendMessage(INTERNEV_SETTEXT,(ssize_t)temp,0,si);
			}
			else
			{
				textzone->SendMessage(INTERNEV_SETTEXT,(ssize_t)strings.GetElementN(GetVal()),0,si);
			}
			return true;
		}
	default :
		return textzone->SendMessage(message,p1,p2,si);
	}
}


void CUIFileChooser::DeleteListContent(void)
{
	for(ListNode *c=strings.GetHead();c;c=c->Next)
	{
		free( (char *)(c->Element) );
	}
}

void CUIFileChooser::Scan(char *iext)
{
	// delete previous content
	for(ListNode *c=strings.GetHead();c;c=c->Next)
	{
		free( (char *)(c->Element) );
	}
	strings.Clear();

	// reget files
	strcpy(ext,iext);
	{
		char *tempname;
		char *stemp;
		SGU_DirLister dir(ext);
		
		while ((tempname=dir.GetNextName())!=NULL)
		{
			if ( (strcmp(tempname,".")!=0) && (strcmp(tempname,"..")!=0) )
			{
				stemp=(char *)malloc((strlen(tempname)+1)*sizeof(char));
				strcpy(stemp,tempname);
				strings.AddElement(stemp);
			}
		}
	}
	ResetInterval(0,strings.GetNbElement()-1);
}
