/*  === Snake Level Editor === *\
    (c) 1999 Stephane Magnenat
	(c) 2000 Stephane Magnenat
\*  ========================== */

#include "CSnakeLevelEditor.h"
#include "SGU.h"
#include <string.h>

CSnakeLevelEditor::CSnakeLevelEditor()
{
	array=new CSnakeArray;
	array->SetYStartPos(32);
	editorsprite=NULL;
	leveltheme=NULL;
	UIRedraw.lastitem=-1;
	UIRedraw.newitem=-1;
	IsButDwn=false;
	drawmode=POINTMODE;
	brush=1;
	IsSaved=false;
	omx=0;
	omy=0;
}

CSnakeLevelEditor::~CSnakeLevelEditor()
{
	delete array;
	delete editorsprite;
	delete screen;
	if (leveltheme)
		delete leveltheme;
}

// newtile indicate the type of wall
/*
0 : free	: -1
1 : wall1	: 10
2 : wall2	: 11
3 : wall1c	: 12
4 : wall2c	: 13
*/

void CSnakeLevelEditor::Point(int x,int y,int newtile)
{
	if (newtile==0)
	{
		array->obstaclearray[x][y]=-1;
	}
	else
	{
		array->obstaclearray[x][y]=newtile+9;
	}
}

void CSnakeLevelEditor::Fill(int x,int y,int newtile)
{
	if (newtile==0)
	{
		newtile=-1;
	}
	else
	{
		newtile+=9;
	}
	FillRec(x,y,newtile);
}

void CSnakeLevelEditor::FillRec(int x,int y,int newtile)
{
	int oldtile;
	oldtile=array->obstaclearray[x][y];
	if (oldtile==newtile)
		return;
	array->obstaclearray[x][y]=newtile;
//	if ((unsigned)newtile==array->bgspritearray[x][y])
//		return;
	if ((x>1) && (array->obstaclearray[x-1][y]==oldtile))
	{
		FillRec(x-1,y,newtile);
	}
//	if ((unsigned)newtile==array->bgspritearray[x][y])
//		return;
	if ((x<SNAKEARRAYW-2) && (array->obstaclearray[x+1][y]==oldtile))
	{
		FillRec(x+1,y,newtile);
	}
//	if ((unsigned)newtile==array->bgspritearray[x][y])
//		return;
	if ((y>1) && (array->obstaclearray[x][y-1]==oldtile))
	{
		FillRec(x,y-1,newtile);
	}
//	if ((unsigned)newtile==array->bgspritearray[x][y])
//		return;
	if ((y<SNAKEARRAYH-2) && (array->obstaclearray[x][y+1]==oldtile))
	{
		FillRec(x,y+1,newtile);
	}
}

// SWAP macro, usefull for sorting coordinates
#define SWAP(a,b,d) d=a; a=b; b=d;

inline int fitoi(float f)
{
	int i=(int)f;
	if (i>0)
	{
		if ((f-i)>0.5)
			return i+1;
		else
			return i;
	}
	else
	{
		if ((f-i)<-0.5)
			return i-1;
		else
			return i;
	}
}

void CSnakeLevelEditor::Line(int x1,int y1,int x2,int y2,int newtile)
{
	if (newtile==0)
	{
		newtile=-1;
	}
	else
	{
		newtile+=9;
	}
	
	int d,dx,dy,cx,cy;


	dx=x2-x1; // can be positiv or negative
	dy=y2-y1; // is positive or zero
	if (dx==0)
	{
		if (dy<0)
		{
			SWAP(y1,y2,d);
		}
		for (cy=y1;cy<=y2;cy++)
			array->obstaclearray[x1][cy]=newtile;
		return;
	}

	if (dy==0)
	{
		if (dx<0)
		{
			SWAP(x1,x2,d);
		}
		for (cx=x1;cx<=x2;cx++)
			array->obstaclearray[cx][y1]=newtile;
		return;
	}

	if (ABS(dx)>ABS(dy))
	{
		// we sort on x
		if (dx<0)
		{
			SWAP(y1,y2,d);
			SWAP(x1,x2,d);
			dx=-dx;
		}
		for (cx=x1;cx<=x2;cx++)
		{
			cy=fitoi( ( (float)( (cx-x1)*(y2-y1) )/(x2-x1) )+y1);
			array->obstaclearray[cx][cy]=newtile;
		}
	}
	else
	{
		// we sort on y
		if (dy<0)
		{
			SWAP(y1,y2,d);
			SWAP(x1,x2,d);
			dy=-dy;
		}
		for (cy=y1;cy<=y2;cy++)
		{
			cx=fitoi( ( (float)( (cy-y1)*(x2-x1) )/(y2-y1) )+x1);
			array->obstaclearray[cx][cy]=newtile;
		}

	}

}

void CSnakeLevelEditor::Rectangle(int x1,int y1,int x2,int y2,int newtile)
{
	int xb=MIN(x1,x2);
	int xf=MAX(x1,x2);
	int yb=MIN(y1,y2);
	int yf=MAX(y1,y2);
	xb=MAX(xb,0);
	yb=MAX(yb,0);
	xf=MIN(xf,SNAKEARRAYW);
	yf=MIN(yf,SNAKEARRAYH);

	int xn,yn;
	if (newtile==0)
	{
		newtile=-1;
	}
	else
	{
		newtile+=9;
	}

	for (xn=xb;xn<=xf;xn++)
	{
		for (yn=yb;yn<=yf;yn++)
		{
			array->obstaclearray[xn][yn]=newtile;
		}
	}
}

void CSnakeLevelEditor::LineSel(SDL_Surface *si,int x1,int y1,int x2,int y2)
{
	x1>>=3;
	y1>>=3;
	x2>>=3;
	y2>>=3;
	y1-=4;
	y2-=4;
	if (y1<0)
		y1=0;
	if (y2<0)
		y2=0;
	int d,dx,dy,cx,cy;


	dx=x2-x1; // can be positiv or negative
	dy=y2-y1; // is positive or zero
	if (dx==0)
	{
		if (dy<0)
		{
			SWAP(y1,y2,d);
		}
		for (cy=y1;cy<=y2;cy++)
			SGU_Rect(si,x1<<3,(cy<<3)+32,7,7,SGU_RGB(255,255,255,0));
		return;
	}

	if (dy==0)
	{
		if (dx<0)
		{
			SWAP(x1,x2,d);
		}
		for (cx=x1;cx<=x2;cx++)
			SGU_Rect(si,cx<<3,(y1<<3)+32,7,7,SGU_RGB(255,255,255,0));
		return;
	}

	if (ABS(dx)>ABS(dy))
	{
		// we sort on x
		if (dx<0)
		{
			SWAP(y1,y2,d);
			SWAP(x1,x2,d);
			dx=-dx;
		}
		for (cx=x1;cx<=x2;cx++)
		{
			cy=fitoi( ( (float)( (cx-x1)*(y2-y1) )/(x2-x1) )+y1);
			SGU_Rect(si,cx<<3,(cy<<3)+32,7,7,SGU_RGB(255,255,255,0));
		}
	}
	else
	{
		// we sort on y
		if (dy<0)
		{
			SWAP(y1,y2,d);
			SWAP(x1,x2,d);
			dy=-dy;
		}
		for (cy=y1;cy<=y2;cy++)
		{
			cx=fitoi( ( (float)( (cy-y1)*(x2-x1) )/(y2-y1) )+x1);
			SGU_Rect(si,cx<<3,(cy<<3)+32,7,7,SGU_RGB(255,255,255,0));
		}

	}
}	

void CSnakeLevelEditor::UnLineSel(SDL_Surface *si,int x1,int y1,int x2,int y2)
{
	x1>>=3;
	y1>>=3;
	x2>>=3;
	y2>>=3;
	y1-=4;
	y2-=4;
	if (y1<0)
		y1=0;
	if (y2<0)
		y2=0;
	int d,dx,dy,cx,cy;


	dx=x2-x1; // can be positiv or negative
	dy=y2-y1; // is positive or zero
	if (dx==0)
	{
		if (dy<0)
		{
			SWAP(y1,y2,d);
		}
		for (cy=y1;cy<=y2;cy++)
			array->SingleBackgroundPosDraw(x1,cy,si);
		return;
	}

	if (dy==0)
	{
		if (dx<0)
		{
			SWAP(x1,x2,d);
		}
		for (cx=x1;cx<=x2;cx++)
			array->SingleBackgroundPosDraw(cx,y1,si);
		return;
	}

	if (ABS(dx)>ABS(dy))
	{
		// we sort on x
		if (dx<0)
		{
			SWAP(y1,y2,d);
			SWAP(x1,x2,d);
			dx=-dx;
		}
		for (cx=x1;cx<=x2;cx++)
		{
			cy=fitoi( ( (float)( (cx-x1)*(y2-y1) )/(x2-x1) )+y1);
			array->SingleBackgroundPosDraw(cx,cy,si);
		}
	}
	else
	{
		// we sort on y
		if (dy<0)
		{
			SWAP(y1,y2,d);
			SWAP(x1,x2,d);
			dy=-dy;
		}
		for (cy=y1;cy<=y2;cy++)
		{
			cx=fitoi( ( (float)( (cy-y1)*(x2-x1) )/(y2-y1) )+x1);
			array->SingleBackgroundPosDraw(cx,cy,si);
		}

	}
}

void CSnakeLevelEditor::RectSel(SDL_Surface *si,int x1,int y1,int x2,int y2)
{
	int xb=MIN(x1,x2);
	int xf=MAX(x1,x2);
	int yb=MIN(y1,y2);
	int yf=MAX(y1,y2);
	yb=MAX(32,yb);
	yf=MAX(32,yf);
	xb=xb&~0x7;
	yb=yb&~0x7;
	xf=xf|0x7;
	yf=yf|0x7;
	SGU_Rect(si,xb,yb,xf-xb+1,yf-yb+1,SGU_RGB(255,255,255,0));
}

void CSnakeLevelEditor::UnRectSel(SDL_Surface *si,int x1,int y1,int x2,int y2)
{
	int xb=MIN(x1,x2);
	int xf=MAX(x1,x2);
	int yb=MIN(y1,y2);
	int yf=MAX(y1,y2);
	int n;
	yb=MAX(32,yb);
	yf=MAX(32,yf);
	xb>>=3;
	yb>>=3;
	xf>>=3;
	yf>>=3;
	yb-=4;
	yf-=4;
	
	for (n=xb;n<=xf;n++)
	{
		array->SingleBackgroundPosDraw(n,yb,si);
		array->SingleBackgroundPosDraw(n,yf,si);
	}
	for (n=yb;n<=yf;n++)
	{
		array->SingleBackgroundPosDraw(xb,n,si);
		array->SingleBackgroundPosDraw(xf,n,si);
	}

}

void CSnakeLevelEditor::PointSel(SDL_Surface *si,int x1,int y1)
{
	int xb=x1&~0x7;
	int yb=y1&~0x7;
	int xf=x1|0x7;
	int yf=y1|0x7;
	if (yb<32)
		return;
	SGU_Rect(si,xb,yb,8,8,SGU_RGB(255,255,255,0));
}

void CSnakeLevelEditor::UnPointSel(SDL_Surface *si,int x1,int y1)
{
	if (y1>=32)
		array->SingleBackgroundPosDraw(x1>>3,(y1>>3)-4,si);
}

	
void CSnakeLevelEditor::InitDefaultValues(char *defbgd,SGU_Sprite *ltlfnt,SGU_Sprite *mdlfnt,SDL_Surface *si,StringTable *ilang)
{ 
	// set lang support
	lang=ilang;

	// load level theme sprite
	if (!leveltheme)
		leveltheme=new SGU_Sprite;
	leveltheme->Load(defbgd,si->format);
	array->SetThemeSprite(leveltheme);
	
	// load tools sprites
	editorsprite=new SGU_Sprite;
	editorsprite->Load("lvledit.spr",si->format);

	// load Interface
	littlefont=ltlfnt;
	middlefont=mdlfnt;
	screen = new CUIScreen(15);
	// level editor sprite :
	/*  0 = point tool
		1 = line tool
		2 = rectangle tool
		3 = fill tool
	*/
	screen->objects[0]= new CUINormalButton(2,1,46,13,theme,littlefont,lang->GetString("[EXIT]"),true);
	screen->objects[1]= new CUINormalButton(52,1,46,13,theme,littlefont,lang->GetString("[NEW]"),true);
	screen->objects[2]= new CUINormalButton(2,17,46,13,theme,littlefont,lang->GetString("[LOAD]"),true);
	screen->objects[3]= new CUINormalButton(52,17,46,13,theme,littlefont,lang->GetString("[SAVE]"),true);

	screen->objects[4]=new CUIGfxButton(160,2,28,28,theme,editorsprite,0,false);
	screen->objects[5]=new CUIGfxButton(195,2,28,28,theme,editorsprite,1,false);
	screen->objects[6]=new CUIGfxButton(230,2,28,28,theme,editorsprite,2,false);
	screen->objects[7]=new CUIGfxButton(265,2,28,28,theme,editorsprite,3,false);

	screen->objects[8]=new CUIGfxButton(310,7,17,17,theme,leveltheme,0,false);
	screen->objects[9]=new CUIGfxButton(335,7,17,17,theme,leveltheme,1,true);
	screen->objects[10]=new CUIGfxButton(360,7,17,17,theme,leveltheme,2,true);
	screen->objects[11]=new CUIGfxButton(385,7,17,17,theme,leveltheme,3,true);
	screen->objects[12]=new CUIGfxButton(410,7,17,17,theme,leveltheme,4,true);

	screen->objects[13]=new CUINormalButton(102,1,46,13,theme,littlefont,lang->GetString("[THEME]"),true);
	screen->objects[14]=new CUINormalButton(102,17,46,13,theme,littlefont,lang->GetString("[UNDO]"),true);
}

void CSnakeLevelEditor::UpdateHelpText(SDL_Surface *si)
{
	// rewrite a portion of screen
	SDL_Rect r;
	r.x=450;
	r.y=0;
	r.w=640;
	r.h=32;
	theme->DrawBackground(si,&r);
	// Draw help text
	switch (screen->LastMouseOverWidget())
	{
	case 0:
		{
			SGU_DisplayText(si,littlefont,450,11,lang->GetString("[LEEXIT]"));
		}
		break;
	case 1:
		{
			SGU_DisplayText(si,littlefont,450,11,lang->GetString("[LENEW]"));
		}
		break;
	case 2:
		{
			SGU_DisplayText(si,littlefont,450,11,lang->GetString("[LELOAD]"));
		}
		break;
	case 3:
		{
			SGU_DisplayText(si,littlefont,450,11,lang->GetString("[LESAVE]"));
		}
		break;
	case 4:
		{
			SGU_DisplayText(si,littlefont,450,11,lang->GetString("[LEPT]"));
		}
		break;
	case 5:
		{
			SGU_DisplayText(si,littlefont,450,11,lang->GetString("[LELN]"));
		}
		break;
	case 6:
		{
			SGU_DisplayText(si,littlefont,450,11,lang->GetString("[LERECT]"));
		}
		break;
	case 7:
		{
			SGU_DisplayText(si,littlefont,450,11,lang->GetString("[LEFILL]"));
		}
		break;
	case 8:
		{
			SGU_DisplayText(si,littlefont,450,11,lang->GetString("[LESELBB]"));
		}
		break;
	case 9:
		{
			SGU_DisplayText(si,littlefont,450,11,lang->GetString("[LESELUW]"));
		}
		break;
	case 10:
		{
			SGU_DisplayText(si,littlefont,450,11,lang->GetString("[LESELUW]"));
		}
		break;
	case 11:
		{
			SGU_DisplayText(si,littlefont,450,11,lang->GetString("[LESELDW]"));
		}
		break;
	case 12:
		{
			SGU_DisplayText(si,littlefont,450,11,lang->GetString("[LESELDW]"));
		}
		break;
	case 13:
		{
			SGU_DisplayText(si,littlefont,450,11,lang->GetString("[LELT]"));
		}
		break;
	case 14:
		{
			SGU_DisplayText(si,littlefont,450,11,lang->GetString("[LEUNDO]"));
		}
		break;
	default:
		{

		}
	}
}

void CSnakeLevelEditor::DrawBar(SDL_Surface *si)
{
	//	DrawFillRect(si,0,0,639,15,toRGB16(0,0,0,si));
	SDL_Rect r;
	r.x=0;
	r.y=0;
	r.w=640;
	r.h=32;
	theme->DrawBackground(si,&r);
	// Update tools
	SGU_RectShade(si,159+35*drawmode,1,30,30,theme->FHiCol,theme->FMidCol,theme->FLowCol);
	// Update brush
	SGU_RectShade(si,309+25*brush,6,19,19,theme->FHiCol,theme->FMidCol,theme->FLowCol);
	// Send, everyone redraws itself :-)
	for (int n=0;n<screen->nobject;n++)
	{
		screen->objects[n]->SendMessage(UIEV_DRAW,0,0,si);
	}
}

void CSnakeLevelEditor::UpdateActTools(SDL_Surface *si)
{
	DrawBar(si);
	SDL_UpdateRect(si,0,0,0,0);
}

void CSnakeLevelEditor::FullDraw(SDL_Surface *si)
{
	DrawBar(si);
	array->FullBackgroundDraw(si);
}

bool CSnakeLevelEditor::SendMessage(SDL_Event ev,SDL_Surface *si)
{
	int mx,my,selwgt;

	if (ev.type==SDL_MOUSEBUTTONDOWN)
	{
		IsButDwn=true;
		mx=ev.button.x;
		my=ev.button.y;
		cmx=mx;
		cmy=my;
		selwgt=screen->GetMouseOverWidget(mx,my);
		switch (selwgt)
		{
		case 0:
			{
				// save if not saved
				if (!IsSaved)
				{
					char filename[256];
					strcpy(filename,"noname.sml");
					if (SGU_SaveFileDialog(si,100,100,440,200,theme,middlefont,lang->GetString("[LESAVEASK]"),lang->GetString("[LESAVEAY]"),lang->GetString("[LESAVEAN]"),filename,"sml"))
					{
						array->Save(filename);
					}
				}
				return true;
			}
			break;
		case 1:
			{
				array->ClearArray();
				
				FullDraw(si);
				SDL_UpdateRect(si,0,0,0,0);
			}
			break;
		case 2:
			{
				// Load
				char filename[256];
				if (SGU_OpenFileDialog(si,100,100,440,200,theme,middlefont,lang->GetString("[LELOAD]"),lang->GetString("[LOAD]"),lang->GetString("[CANCEL]"),filename,"sml"))
				{
					array->Load(filename,si);
					FullDraw(si);
					SDL_UpdateRect(si,0,0,0,0);
				}
				IsButDwn=false;
			}
			break;
		case 3:
			{
				// Save
				char filename[256];
				strcpy(filename,"noname.sml");
				if (SGU_SaveFileDialog(si,100,100,440,200,theme,middlefont,lang->GetString("[LESAVE]"),lang->GetString("[SAVE]"),lang->GetString("[CANCEL]"),filename,"sml"))
				{
					array->Save(filename);
					IsSaved=true;
				}
				IsButDwn=false;
			}
			break;
		case 4:
			{
				drawmode=POINTMODE;
				UpdateActTools(si);
			}
			break;
		case 5:
			{
				drawmode=LINEMODE;
				UpdateActTools(si);
			}
			break;
		case 6:
			{
				drawmode=RECTMODE;
				UpdateActTools(si);
			}
			break;
		case 7:
			{
				drawmode=FILLMODE;
				UpdateActTools(si);
			}
			break;
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
			{
				brush=selwgt-8;
				UpdateActTools(si);
			}	
			break;
		case 13:
			{
				// Load theme
				char filename[256];
				if (SGU_OpenFileDialog(si,100,100,440,200,theme,middlefont,lang->GetString("[LELT]"),lang->GetString("[LOAD]"),lang->GetString("[CANCEL]"),filename,"smt"))
				{
					leveltheme->Load(filename,si->format);	
					FullDraw(si);
					SDL_UpdateRect(si,0,0,0,0);
				}
				IsButDwn=false;
			}
			break;
		case 14:
			{
				// undo
				Undo();
				FullDraw(si);
				SDL_UpdateRect(si,0,0,0,0);
			}
			break;
		}
		if ((my>=40) && (my<472) && (mx>=8) && (mx<632))
		{
			IsSaved=false;
			if (drawmode==POINTMODE)
			{
				Save4Undo();
				Point(mx>>3,(my-32)>>3,brush);
				array->SingleBackgroundPosDraw(mx>>3,(my-32)>>3,si);
				SDL_UpdateRect(si,0,0,0,0);
			}
			else if  (drawmode==FILLMODE)
			{
				Save4Undo();
				Fill(mx>>3,(my-32)>>3,brush);
				array->FullBackgroundDraw(si);
				SDL_UpdateRect(si,0,0,0,0);
			}
		}
		omx=mx;
		omy=my;
	}
	if (ev.type==SDL_MOUSEMOTION)
	{
		// Note : problwem is HERE !!!
		
		mx=ev.motion.x;
		my=ev.motion.y;
		UIRedraw.newitem=screen->GetMouseOverWidget(mx,my);
		if (UIRedraw.newitem!=UIRedraw.lastitem)
		{
			if (UIRedraw.lastitem>=0)
			{
				screen->objects[UIRedraw.lastitem]->SendMessage(UIEV_LOSEMOUSEOVER,0,0,si);				
			}
			if (UIRedraw.newitem>=0)
			{
				screen->objects[UIRedraw.newitem]->SendMessage(UIEV_GETMOUSEOVER,0,0,si);
			}
			UIRedraw.lastitem=UIRedraw.newitem;
			UpdateHelpText(si);
			SDL_UpdateRect(si,0,0,0,0);
		}

		
		if ( IsButDwn && (cmy>=40) && (cmy<472) && (cmx>=8) && (cmx<632))
		{
			IsSaved=false;
			
			if (drawmode==POINTMODE)
			{
				if ((my>=40) && (my<472) && (mx>=8) && (mx<632))
				{					
					Point(mx>>3,(my-32)>>3,brush);
					array->SingleBackgroundPosDraw(mx>>3,(my-32)>>3,si);
					SDL_UpdateRect(si,0,0,0,0);
				}
			}
			else if (drawmode==RECTMODE)
			{
				if ((omy>=40) && (omy<472) && (omx>=8) && (omx<632))
					UnRectSel(si,cmx,cmy,omx,omy);
				if ((my>=40) && (my<472) && (mx>=8) && (mx<632))
					RectSel(si,cmx,cmy,mx,my);
				SDL_UpdateRect(si,0,0,0,0);
			}
			else if (drawmode==LINEMODE)
			{
				if ((omy>=40) && (omy<472) && (omx>=8) && (omx<632))			
					UnLineSel(si,cmx,cmy,omx,omy);
				if ((my>=40) && (my<472) && (mx>=8) && (mx<632))
					LineSel(si,cmx,cmy,mx,my);
				SDL_UpdateRect(si,0,0,0,0);
			}
		}
		else
		{	
			if ((omy>=40) && (omy<472) && (omx>=8) && (omx<632))
				UnPointSel(si,omx,omy);
			if ((my>=40) && (my<472) && (mx>=8) && (mx<632))
				PointSel(si,mx,my);
			SDL_UpdateRect(si,0,0,0,0);			
		}

		omx=mx;
		omy=my;
	}
	if (ev.type==SDL_MOUSEBUTTONUP)
	{
		IsButDwn=false;
		mx=ev.button.x;
		my=ev.button.y;
		if ((my>=40) && (my<472) && (mx>=8) && (mx<632) && (cmy>=40) && (cmy<472) && (cmx>=8) && (cmx<632))
		{
			IsSaved=false;
			if (drawmode==LINEMODE)
			{
				Save4Undo();
				Line(cmx>>3,(cmy-32)>>3,mx>>3,(my-32)>>3,brush);
				array->FullBackgroundDraw(si);
				SDL_UpdateRect(si,0,0,0,0);
			}
			else if  (drawmode==RECTMODE)
			{
				Save4Undo();
				Rectangle(cmx>>3,(cmy-32)>>3,mx>>3,(my-32)>>3,brush);
				array->FullBackgroundDraw(si);
				SDL_UpdateRect(si,0,0,0,0);
			}
		}
		omx=mx;
		omy=my;
	}
	return false;
}

void CSnakeLevelEditor::Save4Undo(void)
{
	memcpy(undoarray,array->obstaclearray,SNAKEARRAYW*SNAKEARRAYH*sizeof(signed long int));
}

void CSnakeLevelEditor::Undo(void)
{
	memcpy(array->obstaclearray,undoarray,SNAKEARRAYW*SNAKEARRAYH*sizeof(signed long int));
}