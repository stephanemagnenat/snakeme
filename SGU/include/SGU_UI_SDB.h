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

#ifndef __UISDB_H
#define __UISDB_H

#include "SGU_UI.h"

class CUISDBTemplate
{
public:
	CUISDBTemplate(int ix,int iy, int iw,int ih,CUITheme *ithm,SGU_Sprite *ifnt);
	virtual ~CUISDBTemplate() {}
	virtual void FullDraw(SDL_Surface *si)=0;
	// return true if ended, false otherwise
	virtual bool SendMessage(SDL_Event ev,SDL_Surface *si)=0;
protected:
	CUIScreen *screen;
	int x,y,w,h;
	CUITheme *theme;
	SUIRedraw UIRedraw;
	SGU_Sprite *font;
};

class CUISDBOpenFile:public CUISDBTemplate
{
public:
	CUISDBOpenFile(int ix,int iy, int iw,int ih,CUITheme *ithm,SGU_Sprite *ifnt,const char *title, const char *ok, const char *cancel);
	~CUISDBOpenFile();
	void FullDraw(SDL_Surface *si);
	bool SendMessage(SDL_Event ev,SDL_Surface *si);
	void ScanDir(const char *filespec);
	char *GetFileName(void);
public:
	const char *titletext;
	const char *oktext;
	const char *canceltext;
private:
	List *filenames;
	int elclick;
};

class CUISDBSaveFile:public CUISDBTemplate
{
public:
	CUISDBSaveFile(int ix,int iy, int iw,int ih,CUITheme *ithm,SGU_Sprite *ifnt,const char *title, const char *ok, const char *cancel);
	~CUISDBSaveFile();
	void FullDraw(SDL_Surface *si);
	bool SendMessage(SDL_Event ev,SDL_Surface *si);
	void ScanDir(const char *filespec);
	char *GetFileName(void);
	bool GetState(void) { return isok; }
public:
	const char *titletext;
	const char *oktext;
	const char *canceltext;
private:
	bool isok;
	List *filenames;
};

// Function for easy access
bool SGU_OpenFileDialog(SDL_Surface *si,int ix,int iy,int iw,int ih,CUITheme *ithm,SGU_Sprite *ifnt,const char *title, const char *ok, const char *cancel, char *filename, const char *filespec);
bool SGU_SaveFileDialog(SDL_Surface *si,int ix,int iy,int iw,int ih,CUITheme *ithm,SGU_Sprite *ifnt,const char *title, const char *ok, const char *cancel, char *filename, const char *filespec);

#endif
