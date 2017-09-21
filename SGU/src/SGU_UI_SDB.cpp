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

#include "SGU_UI_SDB.h"
#include <string.h>

#ifdef WIN32
	#include <io.h>
#endif

CUISDBTemplate::CUISDBTemplate(int ix,int iy, int iw,int ih,CUITheme *ithm,SGU_Sprite *ifnt)
{
	x=ix;
	y=iy;
	w=iw;
	h=ih;
	theme=ithm;
	font=ifnt;
}

CUISDBOpenFile::CUISDBOpenFile(int ix,int iy, int iw,int ih,CUITheme *ithm,SGU_Sprite *ifnt,char *title, char *ok, char *cancel):CUISDBTemplate(ix,iy,iw,ih,ithm,ifnt)
{
	elclick=-1;
	filenames=new List;
	titletext=title;
	oktext=ok;
	canceltext=cancel;
	screen=new CUIScreen(3);
	screen->objects[0]=NULL;
	screen->objects[1]=new CUINormalButton(x+4,y+h-32,(w-16)>>1,28,theme,font,ok,true);
	screen->objects[2]=new CUINormalButton(x+12+((w-16)>>1),y+h-32,(w-16)>>1,28,theme,font,cancel,true);
	UIRedraw.lastitem=-1;
	UIRedraw.newitem=-1;
}

CUISDBOpenFile::~CUISDBOpenFile()
{
	lpListNode mynode;
	for (mynode=filenames->GetHead();mynode!=NULL;mynode=mynode->Next)
	{
		free ((char *)(mynode->Element));
	}
	delete filenames;
	for (int n=0;n<3;n++)
		delete screen->objects[n];
}

void CUISDBOpenFile::FullDraw(SDL_Surface *si)
{
	SGU_RectShade(si,x,y,w,h,theme->FHiCol,theme->FMidCol,theme->FLowCol);
	SGU_DisplayText(si,font,((w-SGU_GetTextWidth(font,titletext))>>1)+x+4,y+4,titletext);
	// draw items on screen
	screen->DrawScreen(si);
}

bool CUISDBOpenFile::SendMessage(SDL_Event ev,SDL_Surface *si)
{
	screen->ProcessDefaultEvent(&ev,si);
	if (screen->GetClicked()>=0)
	{
		switch (screen->GetClicked())
		{
		case 1:
			{
				elclick=((CUIStringList *)(screen->objects[0]))->GetSelectedElement();
				return true;
			}
			break;
		case 2:
			{
				elclick=-1;
				return true;
			}
			break;
		}
	}
	return false;
}

void CUISDBOpenFile::ScanDir(char *filespec)
{
	char *tempname;
	char *stemp;
	SGU_DirLister dir(filespec);
	
	while ((tempname=dir.GetNextName())!=NULL)
	{
		if ( (strcmp(tempname,".")!=0) && (strcmp(tempname,"..")!=0) )
		{
			stemp=(char *)malloc((strlen(tempname)+1)*sizeof(char));
			strcpy(stemp,tempname);
			filenames->AddElement(stemp);
		}
	}
	screen->objects[0]=new CUIStringList(x+4,y+30,w-8,h-70,theme,filenames->GetNbElement(),16,21);
	((CUIStringList *)(screen->objects[0]))->SetFont(font);
	((CUIStringList *)(screen->objects[0]))->SetStringList(filenames);
}

char *CUISDBOpenFile::GetFileName(void)
{
	if ((elclick>=0) && (elclick<filenames->GetNbElement()))
	{
		return (char *)filenames->GetElementN(elclick);
	}
	else
	{
		return NULL;
	}
}

CUISDBSaveFile::CUISDBSaveFile(int ix,int iy, int iw,int ih,CUITheme *ithm,SGU_Sprite *ifnt,char *title, char *ok, char *cancel):CUISDBTemplate(ix,iy,iw,ih,ithm,ifnt)
{
	filenames=new List;
	titletext=title;
	oktext=ok;
	canceltext=cancel;
	screen=new CUIScreen(4);
	screen->objects[0]=NULL;
	screen->objects[1]=new CUITextEdit(x+4,y+h-70, w-8,28,theme,font, "",false);
	screen->objects[2]=new CUINormalButton(x+4,y+h-32,(w-16)>>1,28,theme,font,ok,true);
	screen->objects[3]=new CUINormalButton(x+12+((w-16)>>1),y+h-32,(w-16)>>1,28,theme,font,cancel,true);
	UIRedraw.lastitem=-1;
	UIRedraw.newitem=-1;
}

CUISDBSaveFile::~CUISDBSaveFile()
{
	lpListNode mynode;
	for (mynode=filenames->GetHead();mynode!=NULL;mynode=mynode->Next)
	{
		free ((char *)(mynode->Element));
	}
	delete filenames;
	for (int n=0;n<4;n++)
		delete screen->objects[n];
}

void CUISDBSaveFile::FullDraw(SDL_Surface *si)
{
	SGU_RectShade(si,x,y,w,h,theme->FHiCol,theme->FMidCol,theme->FLowCol);
	SGU_DisplayText(si,font,((w-SGU_GetTextWidth(font,titletext))>>1)+x+4,y+4,titletext);
	// draw items on screen
	screen->DrawScreen(si);
}

bool CUISDBSaveFile::SendMessage(SDL_Event ev,SDL_Surface *si)
{
	screen->ProcessDefaultEvent(&ev,si);
	if (screen->GetClicked()>=0)
	{
		switch (screen->GetClicked())
		{
		case 2:
			{
				isok=true;
				return true;
			}
			break;
		case 3:
			{
				isok=false;
				return true;
			}
			break;
		case 0:
			{
				int elclick;
				elclick=((CUIStringList *)(screen->objects[0]))->GetSelectedElement();
				if ((elclick>=0) && (elclick<filenames->GetNbElement()))
				{
					screen->objects[1]->SendMessage(INTERNEV_SETTEXT,(ssize_t)(char *)filenames->GetElementN(elclick),0,si);
				}
				SDL_UpdateRect(si,x,y,w,h);
				break;
			}
		}
	}
	return false;
}

void CUISDBSaveFile::ScanDir(char *filespec)
{
	char *tempname;
	char *stemp;
	SGU_DirLister dir(filespec);
	
	while ((tempname=dir.GetNextName())!=NULL)
	{
		if ( (strcmp(tempname,".")!=0) && (strcmp(tempname,"..")!=0) )
		{
			stemp=(char *)malloc((strlen(tempname)+1)*sizeof(char));
			strcpy(stemp,tempname);
			filenames->AddElement(stemp);
		}
	}
	screen->objects[0]=new CUIStringList(x+4,y+30,w-8,h-110,theme,filenames->GetNbElement(),16,21);
	((CUIStringList *)(screen->objects[0]))->SetFont(font);
	((CUIStringList *)(screen->objects[0]))->SetStringList(filenames);
}

char *CUISDBSaveFile::GetFileName(void)
{
	return ((CUITextEdit *)(screen->objects[1]))->GetText();
}

bool SGU_OpenFileDialog(SDL_Surface *si,int ix,int iy,int iw,int ih,CUITheme *ithm,SGU_Sprite *ifnt,char *title, char *ok, char *cancel,char *filename,char *filespec)
{
	SDL_Event ev;
	SDL_Rect rect;
	SGU_RegionSaver region;
	char *s;
	rect.x=ix;
	rect.y=iy;
	rect.w=iw;
	rect.h=ih;
	region.SaveRegion(si,ix,iy,iw,ih);
	ithm->DrawBackground(si,&rect);
	CUISDBOpenFile opendialog(ix,iy,iw,ih,ithm,ifnt,title,ok,cancel);
	opendialog.ScanDir(filespec);
	opendialog.FullDraw(si);
	SDL_UpdateRect(si,0,0,0,0);
	while ( SDL_WaitEvent(&ev) >= 0 )
	{
		if(opendialog.SendMessage(ev,si))
		{
			s=opendialog.GetFileName();
			if (s)
			{
				memcpy(filename,s,strlen(s)+1);
				region.RestaurRegion(si);
				SDL_UpdateRect(si,0,0,0,0);
				return true;
			}
			else
			{
				filename[0]=0;
				region.RestaurRegion(si);
				SDL_UpdateRect(si,0,0,0,0);
				return false;
			}
		}
	}
	region.RestaurRegion(si);
	SDL_UpdateRect(si,0,0,0,0);
	return false;
}

bool SGU_SaveFileDialog(SDL_Surface *si,int ix,int iy,int iw,int ih,CUITheme *ithm,SGU_Sprite *ifnt,char *title, char *ok, char *cancel,char *filename,char *filespec)
{
	SDL_Event ev;
	SDL_Rect rect;
	SGU_RegionSaver region;
	char *s;
	rect.x=ix;
	rect.y=iy;
	rect.w=iw;
	rect.h=ih;
	region.SaveRegion(si,ix,iy,iw,ih);
	ithm->DrawBackground(si,&rect);
	CUISDBSaveFile savedialog(ix,iy,iw,ih,ithm,ifnt,title,ok,cancel);
	savedialog.ScanDir(filespec);
	savedialog.FullDraw(si);
	SDL_UpdateRect(si,0,0,0,0);
	while ( SDL_WaitEvent(&ev) >= 0 )
	{
		if(savedialog.SendMessage(ev,si))
		{
			if (savedialog.GetState())
			{
				s=savedialog.GetFileName();
				if ((s) && (s[0]!=0))
				{
					strcpy(filename,s);
					//memcpy(filename,s,strlen(s));
					// compare filename with filespec
					char *end=strstr(filename,filespec);
					if ((unsigned)(end-filename)!=((unsigned)strlen(filename)-(unsigned)strlen(filespec)))
					{
						// add filespec to filename
						strcat(filename,".");
						strcat(filename,filespec);
					}
					region.RestaurRegion(si);
					SDL_UpdateRect(si,0,0,0,0);
					return true;
				}
				else
				{
					filename[0]=0;
					region.RestaurRegion(si);
					SDL_UpdateRect(si,0,0,0,0);
					return false;
				}
			}
			else
			{
				region.RestaurRegion(si);
				SDL_UpdateRect(si,0,0,0,0);
				return false;
			}
		}
	}
	region.RestaurRegion(si);
	SDL_UpdateRect(si,0,0,0,0);
	return false;
}
