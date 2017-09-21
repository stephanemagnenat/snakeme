/*
    DirLister, a little plateforme independent directory lister
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

#ifndef __DIRLISTER_H
#define __DIRLISTER_H

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


	/// Directory Listing
class SGU_DirLister
{
public:
	SGU_DirLister(char *ext);
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