/*  === Snake Level Editor === *\
    (c) 1999 Stephane Magnenat
\*  ========================== */

#ifndef __CSNAKELEVELEDITOR_H
#define __CSNAKELEVELEDITOR_H

#include "SGU.h"
#include "SGU_UI.h"
#include "CSnakeArray.h"
#include "StringTable.h"

class CSnakeLevelEditor
{
public:
	CSnakeLevelEditor();
	~CSnakeLevelEditor();
	void InitDefaultValues(char *defbgd,SGU_Sprite *ltlfnt,SGU_Sprite *mdlfnt,SDL_Surface *si,StringTable *ilang); 
	void FullDraw(SDL_Surface *si);
	// return true if ended, false otherwise
	bool SendMessage(SDL_Event ev,SDL_Surface *si);
public:
	CUITheme *theme;
private:
	void Point(int x,int y,int newtile);
	void Line(int x1,int y1,int x2,int y2,int newtile);
	void Rectangle(int x1,int y1,int x2,int y2,int newtile);
	void Fill(int x,int y,int newtile);
	void FillRec(int x,int y,int newtile);

	void RectSel(SDL_Surface *si,int x1,int y1,int x2,int y2);
	void UnRectSel(SDL_Surface *si,int x1,int y1,int x2,int y2);
	void PointSel(SDL_Surface *si,int x1,int y1);
	void UnPointSel(SDL_Surface *si,int x1,int y1);
	void LineSel(SDL_Surface *si,int x1,int y1,int x2,int y2);
	void UnLineSel(SDL_Surface *si,int x1,int y1,int x2,int y2);

	void DrawBar(SDL_Surface *si);
	void UpdateHelpText(SDL_Surface *si);
	void UpdateActTools(SDL_Surface *si);

	void Save4Undo(void);
	void Undo(void);
private:
	StringTable *lang;
	CSnakeArray *array;
	SGU_Sprite *leveltheme;
	SGU_Sprite *editorsprite;
	SGU_Sprite *littlefont;
	SGU_Sprite *middlefont;
	CUIScreen *screen;
	SUIRedraw UIRedraw;	
	bool IsButDwn;
	bool IsSaved;
	int omx,omy;
	int cmx,cmy;
	enum { POINTMODE=0, LINEMODE, RECTMODE, FILLMODE } drawmode;
	int brush;
	signed long int undoarray[SNAKEARRAYW][SNAKEARRAYH];
};


#endif