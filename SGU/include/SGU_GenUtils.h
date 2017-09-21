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

#ifndef __SGU_GENUTILS_H
#define __SGU_GENUTILS_H

#ifdef macintosh
	#include <Types.h>
	#include <Memory.h>
	#include <Quickdraw.h>
	#include <Fonts.h>
	#include <Events.h>
	#include <Menus.h>
	#include <Windows.h>
	#include <TextEdit.h>
	#include <Dialogs.h>
	#include <OSUtils.h>
	#include <ToolUtils.h>
	#include <SegLoad.h>
	#include <Sound.h>
	#include <SegLoad.h>
	#include <TextUtils.h>
	#include "ownutsText.h"
#endif

// include for directory scanning
#ifdef WIN32
//	#include <windows.h>
	#include <io.h>
#endif
#ifdef __GNUC__
	#include <stddef.h>
	#include <stdio.h>
	#include <sys/types.h>
	#include <dirent.h>
#endif

#include "SDL.h"
#include "SGU_types.h"
#include "SGU_Sprite.h"


	/// Draw an horizontal line with a given colour
void SGU_HorzLine(SDL_Surface *surface,Sint16 x,Sint16 y,Sint16 l,SGU_RGBA c);

	/// Draw a vertical line with a given colour
void SGU_VertLine(SDL_Surface *surface,Sint16 x,Sint16 y,Sint16 l,SGU_RGBA c);

	/// Display a string using an Sprite as font
void SGU_DisplayText(SDL_Surface *surface,SGU_Sprite *font,int x,int y,const char *s);

	/// Display a string using an Sprite as font and centered
void SGU_DisplayTextCenter(SDL_Surface *surface,SGU_Sprite *font,int x,int y,int w,const char *s);

	/// Give the length of a given string using font
int SGU_GetTextWidth(SGU_Sprite *font,const char *s);

	/// Draw a line
void SGU_Line(SDL_Surface *surface,int x1,int y1,int x2,int y2,SGU_RGBA c);

	/// Draw a rectangle
void SGU_Rect(SDL_Surface *surface,int x1,int y1,int w,int h,SGU_RGBA c);

	/// Draw a filled rectangle
void SGU_FillRect(SDL_Surface *surface,int x,int y,int w,int h,SGU_RGBA c);

	/// Fill a rect with alpha
void SGU_FillRectAlpha(SDL_Surface *surface,int x,int y,int w,int h,SGU_RGBA c);

	/// Draw a horizontal line with shade
void SGU_HorzLineShade(SDL_Surface *surface,Sint16 x,Sint16 y,Sint16 l,SGU_RGBA c1,SGU_RGBA c2);

	/// Draw a vertical line with shade
void SGU_VertLineShade(SDL_Surface *surface,Sint16 x,Sint16 y,Sint16 l,SGU_RGBA c1,SGU_RGBA c2);

	/// Draw shaded rectangle
void SGU_RectShade(SDL_Surface *surface,int x,int y,int w,int h,SGU_RGBA c1,SGU_RGBA ct,SGU_RGBA c2);

	/// Blit in double size using bilinear filtering
void SGU_DoubleSizeBlit(SDL_Surface *src,SDL_Surface *dst,int x,int y);

	/// Region saver, usefull for mouse and other purprose
class SGU_RegionSaver
{
public:
	SGU_RegionSaver() { data=NULL; }
	~SGU_RegionSaver() { if (data) SDL_FreeSurface(data); }
	bool IsEmpty() { return (data==NULL); }
	bool SaveRegion(SDL_Surface *surface,int rx,int ry,int rw,int rh);
	bool RestaurRegion(SDL_Surface *surface);
private:
	SDL_Rect rect;
	SDL_Surface *data;
};

	/// Directory Listing
class SGU_DirLister
{
public:
	SGU_DirLister(const char *ext);
	~SGU_DirLister();
	/// Get Next name, return NULL if ended
	char *GetNextName();
private:
	char *extension;
#ifdef WIN32
	bool first;
	struct _finddata_t c_file;
	long hFile;
#endif
#ifdef __GNUC__
	DIR *dp;
	struct dirent *ep;
#endif
#ifdef macintosh
	/* Globals */
	Rect	windRect;
	
	short	volumeStandard;
	long	dossierStandard, dossierParent;
	char** finalListe;
	int		nbFiles;
	int		indiceListe, indiceFullListe;
	
	#define kFolderBit 16
	
	/* Prototypes */
	void Initialize(void);
	Boolean getIncludedFiles( char* folderName , char*** fileList , int* gimmiNbFiles);
	Boolean InitDriverMacFiles(void);
	void filterList( char*** fileList , int* gimmiNbFiles , char* ext);

#endif
};


#endif
