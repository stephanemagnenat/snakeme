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

#include "DirLister.h"

	/// Directory Listing
SGU_DirLister::SGU_DirLister(char *ext)
{
#ifdef WIN32
	first=true;
	extension=(char *)malloc(strlen(ext)+3);
	strcpy((extension+2),ext);
	extension[0]='*';
	extension[1]='.';
#endif
#ifdef __GNUC__
	extension=(char *)malloc(strlen(ext)+1);
	strcpy(extension,ext);
	dp=opendir("./");
#endif
#ifdef macintosh
	if (InitDriverMacFiles() && getIncludedFiles( ":" , &finalListe , &indiceFullListe)	)
		filterList(&finalListe , &indiceFullListe , ext);
	else
		indiceFullListe=0;
		indiceListe=0;
#endif
}

SGU_DirLister::~SGU_DirLister()
{
#ifdef WIN32
	_findclose( hFile );
	free(extension);
#endif
#ifdef __GNUC__
	(void)closedir(dp);
	free(extension);
#endif
#ifdef macintosh
	int i;
	for (i=0; i<indiceFullListe; i++)
		if (finalListe[i])
			DisposePtr(finalListe[i]);
	if (finalListe)
			DisposePtr((char *)finalListe);
#endif
}

	/// Get Next name, return NULL if ended
char *SGU_DirLister::GetNextName()
{
#ifdef WIN32
	if (first)
	{
		if ( !((hFile = _findfirst( extension, &c_file )) == -1L) )
		{
			first=false;
			return c_file.name;
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		if (_findnext( hFile, &c_file )==0)
		{
			return c_file.name;
		}
		else
		{
			return NULL;
		}
	}
#endif
#ifdef __GNUC__
	if (dp)
	{
		int l,nl;
		while (ep=readdir(dp))
		{
			l=strlen(extension);
			nl=strlen(ep->d_name);
			if (strcmp(extension,ep->d_name+(nl-l))==0)
			{
				return ep->d_name;
			}
		}
		return NULL;
	}
	else
	{
		return NULL;
	}
#endif
#ifdef macintosh
if (indiceListe<indiceFullListe)
	{
		indiceListe++;
		return finalListe[indiceListe-1];
	}
	else
		return NULL;
#endif
}


#ifdef macintosh
Boolean SGU_DirLister::getIncludedFiles( char* folderName , char*** fileList , int* gimmiNbFiles)
{
	Boolean myErr;
	//Str255 nomCourrant;
	char * nomDossierAplication;
	CInfoPBRec myCBP;
	short index, i;
	
	char **theListA, **theListT;
	int nbFiles ,nbAlocatedFiles;
	
	short	vRefNum; 
	long	dirID;
	ConstStr255Param	folderNamePstr;
	FSSpec *	spec;
	char *currentName;
	
	*fileList = nil;
	
	//first: find folder from name description.
	vRefNum = 0;
	dirID = 0;
	spec = (FSSpec*)NewPtr(sizeof(FSSpec));
	
	
	copyCToPascalString(folderName, (char **)&folderNamePstr);
	myErr = FSMakeFSSpec(vRefNum, dirID, folderNamePstr, spec);
	if (myErr == noErr)
	{
		volumeStandard = (*spec).vRefNum;
		dossierParent = (*spec).parID;
	}
	else
		return false;//Not found: we must escape.
	
	//if (BitAnd(myCBP.dirInfo.ioFlAttrib, kFolderBit) != 0 )
	//	return false;//This is a file! We escape.
	
	
	nomDossierAplication =  (char*)NewPtr(sizeof(Str63));
	BlockMove( &((*spec).name) , nomDossierAplication , ((*spec).name[0]+1) );
	
	
	myCBP.dirInfo.ioCompletion = nil;
	myCBP.dirInfo.ioNamePtr = (unsigned char*)nomDossierAplication;
	myCBP.dirInfo.ioVRefNum = volumeStandard;
	myCBP.dirInfo.ioFDirIndex = 0;//We want folder info.	
	myCBP.dirInfo.ioDrDirID = dossierParent;
		
	myErr = PBGetCatInfoSync(&myCBP);
		
	if (myErr == noErr )
	{
		dossierStandard = myCBP.dirInfo.ioDrDirID;//ioDrParID/ioDrDirID
	}
	else
		return false;//Not found: we must escape.
	
	
	//Now list included files.
	index = 1;
	nbFiles = 0;
	nbAlocatedFiles = 16;
	theListA = (char **)NewPtr(nbAlocatedFiles*sizeof(char*));
	
	currentName =  (char *)NewPtr(256*sizeof(char));
	
	while (true)
	{
		myCBP.dirInfo.ioNamePtr = (unsigned char*)currentName;
		myCBP.dirInfo.ioFDirIndex = index;
		myCBP.dirInfo.ioVRefNum = volumeStandard;
		myCBP.dirInfo.ioDrDirID = dossierStandard;
		myCBP.dirInfo.ioACUser = 0;
		
		myErr = PBGetCatInfo(&myCBP, false);
		
		if (myErr == noErr )
		{
			if (BitAnd(myCBP.dirInfo.ioFlAttrib, kFolderBit) == 0 )
			{
				theListA[nbFiles] = p2cstr( (unsigned char*)currentName );
				
				currentName =  (char *)NewPtr(256*sizeof(char));
				
				nbFiles++;
				
				if (nbFiles>=nbAlocatedFiles)
				{
					
					nbAlocatedFiles+=16;
					theListT = (char **)NewPtr(nbAlocatedFiles*sizeof(char *));
					for (i=0;i<nbFiles;i++)
					{
						theListT[i]=theListA[i];
					}
					DisposePtr((char *)theListA);
					theListA = theListT;
				}
			}
		}
		else
			break;
		index++;
	}
	
	if ((nbFiles+1)<nbAlocatedFiles)
	{
		theListT = (char **)NewPtr((nbFiles+1)*sizeof(char *));
		for (i=0;i<nbFiles;i++)
			{
				theListT[i]=theListA[i];
			}
		DisposePtr((char *)theListA);
		theListA = theListT;
	}
	*fileList = theListA;
	*gimmiNbFiles = nbFiles;
	
	return true;
}
void SGU_DirLister::filterList( char*** fileList , int* gimmiNbFiles , char* ext)
{
	int i, j, le, lex;
	int nbFiles, indiceFullListe;
	char **theListA, **theListT;
	Boolean good;
	
	nbFiles=0;
	indiceFullListe=*gimmiNbFiles;
	theListA=*fileList;
	
	
	lex=strlen(ext);
	
	if ((ext!=NULL)&&(lex>0))
	{
		
		for (i=0; i<lex; i++)
			if (ext[i]<91)
				ext[i]=ext[i]+32;//all Lower case
		
		for (i=0; i<indiceFullListe; i++)
		{
			le=strlen(theListA[i]);
			
			good=true;
			for (j=1; j<=lex; j++)
				if ((theListA[i][le-j]!=ext[lex-j])&&(theListA[i][le-j]!=ext[lex-j]-32))
				{
					good=false;
					break;
				}
			
			if (good)
			{
				theListA[nbFiles]=theListA[i];
				if (i>nbFiles)
					theListA[i]=NULL;
				nbFiles++;
			}else
			{
				DisposePtr(theListA[i]);
				theListA[i]=NULL;
			}
		}
	
		theListT = (char **)NewPtr((nbFiles+1)*sizeof(char *));
		for (i=0;i<nbFiles;i++)
			{
				theListT[i]=theListA[i];
			}
		theListT[nbFiles]="\0";
		
		DisposePtr((char *)theListA);
		*fileList = theListT;
		*gimmiNbFiles=nbFiles;
	}
}
Boolean SGU_DirLister::InitDriverMacFiles()
{
	short	vRefNum; 
	long	dirID;
	ConstStr255Param	fileName;
	FSSpec *	spec;
	Boolean myErr;
	
	vRefNum = 0;
	dirID = 0;
	fileName ="\p:";
	spec = (FSSpec*)NewPtr(sizeof(FSSpec));
	
	myErr = FSMakeFSSpec(vRefNum, dirID, fileName, spec);
	
	if (myErr == noErr)
	{
		volumeStandard = (*spec).vRefNum;
		dossierStandard = (*spec).parID;
		return true;
	}
	else
		return false;
}
#endif
