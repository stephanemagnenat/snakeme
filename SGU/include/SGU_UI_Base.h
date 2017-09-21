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

#ifndef __UIBASE_H
#define __UIBASE_H

#include "SGU.h"
#include "SGU_List.h"
#include "SGU_UI_Theme.h"
#include <stdio.h>

// User interface events
// UI derivated widget should react to proper messages

// LOWORD(p1)=mouse x,
// HIWORD(p2)=mouse y,
// p2=state

#define GETMOUSEPOS(p,x,y) { *x=(p)&0xFFFF; *y=(p)>>16; }
#define RETMOUSEPOS(x,y) (((x)&0xFFFF)+((y)<<16))

#define UIEV_LBUTTONDOWN 1
#define UIEV_LBUTTONUP 2
#define UIEV_LBUTTONDBLCLK 3

#define UIEV_RBUTTONDOWN 4
#define UIEV_RBUTTONUP 5
#define UIEV_RBUTTONDBLCLK 6

#define UIEV_MBUTTONDOWN 7
#define UIEV_MBUTTONUP 8
#define UIEV_MBUTTONDBLCLK 9

#define UIEV_MSCROLLDOWN 10
#define UIEV_MSCROLLUP 11

#define UIEV_MOUSEMOVE 12

// p1=virtual key code
#define UIEV_KEYDOWN 13
#define UIEV_KEYUP 14

// p1=ASCII char
#define UIEV_CHAR 15

//
#define UIEV_DRAW 16
#define UIEV_GETFOCUS 17
#define UIEV_LOSEFOCUS 18

// p1=Mouse pos, p2=Mouse state
#define UIEV_GETMOUSEOVER 19
#define UIEV_LOSEMOUSEOVER 20

// messages for textedit
#define INTERNEV_REDRAW 100
#define INTERNEV_SETTEXT 101

typedef struct
{
	int lastitem;
	int newitem;
} SUIRedraw;

class CUIObject
{
public:
	CUIObject() { theme=NULL; hasfocus=false; }
	CUIObject(int ix,int iy,int iw,int ih,CUITheme *ithm) {x=ix; y=iy; w=iw; h=ih; theme=ithm; hasfocus=false; }
	virtual ~CUIObject() { }
	bool DefaultSendMessage(int message,ssize_t p1,ssize_t p2,SDL_Surface *si);
	virtual bool SendMessage(int message,ssize_t p1,ssize_t p2,SDL_Surface *si)=0;
	bool IsPtOnObject(int px,int py) { return ((px>x)&&(px<x+w) &&(py>y) &&(py<y+h)); }
private:
	SGU_RegionSaver topregion;
	SGU_RegionSaver bottomregion;
	SGU_RegionSaver leftregion;
	SGU_RegionSaver rightregion;
protected:
	bool hasfocus;
public:
	int x,y,w,h;
	CUITheme *theme;
};

// WARNING, depreciated
/*
class CUILightButton:public CUIObject
{
public:
	CUILightButton(int ix,int iy,int iw,int ih,CUITheme *ithm,SGU_Sprite *ifnt, char *s,bool cnt);
	void SetCaption(char *s);
	bool SendMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si);
public:
	SGU_Sprite *font;
	char *caption;
	SGU_RegionSaver topregion;
	SGU_RegionSaver bottomregion;
	bool centered;
	int dx;
};



class CUISlideBar:public CUIObject
{
public:
	CUISlideBar(int ix,int iy, int iw,int ih,CUITheme *ithm,int totval,int nbvalons,int aw, bool vert);
	bool SendMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si);
	int GetBaseValue(void) { return basevalue; }
private:
	int totalval;
	int nbvalonscreen;
	int basevalue;
	int arrowwidth;
	int slidewidth;
	bool vertical;
	SGU_RegionSaver slideregion;
};

class CUIGenericList:public CUIObject
{
public:
	CUIGenericList(int ix,int iy, int iw,int ih,CUITheme *ithm,int nbel,int elw,bool vert,int slidew);
	~CUIGenericList(void) { delete slidebar; }
	bool SendMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si);
	int GetSelectedElement(void) { return selel; }
	bool IsVertical(void) { return vertical; }
private:
	virtual void DisplayListElement(int eln,int x,int y,SDL_Surface *si)=0;
private:
	CUISlideBar *slidebar;
	int nbelement;
	int elfit;
	int elwidth;
	int selel;
	bool vertical;
	int sw;
	SGU_RegionSaver listregion;
};

class CUIFrameList:public CUIGenericList
{
public:
	CUIFrameList(int ix,int iy, int iw,int ih,CUITheme *ithm,int nbel,int elw,bool vert,int slidew):CUIGenericList( ix, iy,  iw, ih, ithm, nbel, elw, vert,slidew) {}
private:
	void DisplayListElement(int eln,int x,int y,SDL_Surface *si);
public:
	void SetSprite(SGU_Sprite *spr) {sprite=spr; }
private:
	SGU_Sprite *sprite;
};

class CUIStringList:public CUIGenericList
{
public:
	CUIStringList(int ix,int iy, int iw,int ih,CUITheme *ithm,int nbel,int elw,bool vert,int slidew):CUIGenericList( ix, iy,  iw, ih, ithm, nbel, elw, vert,slidew) {}
private:
	void DisplayListElement(int eln,int x,int y,SDL_Surface *si);
public:
	void SetStringList(List *s) {strings=s; }
	void SetFont(SGU_Sprite *fnt) { font=fnt; }
private:
	List *strings;
	SGU_Sprite *font;
};
*/

class CUIBoolChooser:public CUIObject
{
public:
	CUIBoolChooser(int ix,int iy,int iw,int ih,CUITheme *ithm,SGU_Sprite *ifnt, char *s) {x=ix; y=iy; w=iw; h=ih; theme=ithm; font=ifnt; caption=s; state=false; }
	bool SendMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si);
	bool GetState(void) { return state; }
	void SetState(bool ns) {  state=ns; }
private:
	bool state;
	SGU_Sprite *font;
	char *caption;
};
/*
class CUIIntChooser:public CUIObject
{
public:
	CUIIntChooser(int ix,int iy,int iw,int ih,CUITheme *ithm,SGU_RGBA icb,SGU_Sprite *ifnt,int minv,int maxv,int actv,int aw,bool vert);
	bool SendMessage(int message,ssize_t p1, ssize_t p2, SDL_Surface *si);
	int GetValue(void) { return value; }
	void SetValue(int val) { value=val; }
	int GetMaxValue(void) { return maxval; }
	void SetMaxValue(int mv) { maxval=mv; }
	int GetMinValue(void) { return minval; }
	void SetMinValue(int mv) { minval=mv; }
private:
	int value,maxval,minval,arrowwidth;
	SGU_RGBA cbar;
	SGU_RegionSaver slideregion;
	bool vertical;
};
*/
class CUIScreen
{
public:
	CUIScreen() { nobject=0; objects=NULL; UIRedraw.lastitem=-1; UIRedraw.newitem=-1;  focuswgt=0; firstdraw=true; }
	CUIScreen(int n) {nobject=n; objects=(CUIObject **)malloc(n*sizeof(CUIObject *)); UIRedraw.lastitem=-1; UIRedraw.newitem=-1; focuswgt=0; firstdraw=true; }
	virtual ~CUIScreen() {FreeAll(); }
	void ProcessDefaultEvent(SDL_Event *ev,SDL_Surface *ScrInfo);
	void SetNumberOfObjects(int n) { FreeAll(); objects=(CUIObject **)malloc(n*sizeof(CUIObject *));}
	int GetMouseOverWidget(int px, int py);
	void DrawScreen(SDL_Surface *ScrInfo);
	int LastMouseOverWidget(void) { return mouseoverwgt; }
	int GetClicked(void) { return clicked; }
	int GetKeyHit(void) { return keyhit; }
private:
	void FreeAll(void) { if (nobject>0) for (int n=0;n<nobject;n++) delete objects[n]; free(objects); }
private:
	SUIRedraw UIRedraw;
	int mouseoverwgt;
	int focuswgt;
	int clicked;
	int keyhit;
	bool firstdraw;
public:
	CUIObject **objects;
	int nobject;
};

#endif
