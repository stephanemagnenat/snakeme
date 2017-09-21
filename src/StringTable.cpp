/* String Table services                */
/*                                      */
/* Code file                            */
/*                                      */
/* Copyright (c) Stephane Magnenat 2000 */
/*                                      */

#include "StringTable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef win32
#include <malloc.h>
#endif




StringTable::StringTable()
{
	numberofstrings=0;
	numberoflanguages=0;
}


StringTable::~StringTable()
{
	int n,m;
	for (n=0;n<numberofstrings;n++)
	{
		free (strings[n].name);
		for (m=0;m<numberoflanguages;m++)
		{
			free (strings[n].data[m]);
		}
		free (strings[n].data);
	}
	free (strings);
}


/* file format is :

	number of language
	number of string
	{
		nom du string
		string lang 1
		.
		.
		.
		string lang n
		}

*/

bool StringTable::Load(char *filename)
{
	FILE *fp;
	char temp[1024];
	char* tempp;
	int i,n;

	if ((fp=fopen(filename,"r"))==NULL)
	{
		return false;
	}
	else
	{
		actlang=0;
		// get length
		fgets(temp,1024,fp);
		numberoflanguages=atoi(strtok(temp," \r\n"));
		fgets(temp,1024,fp);
		numberofstrings=atoi(strtok(temp," \r\n"));
		strings=(OneStringToken *)malloc(numberofstrings*sizeof(OneStringToken));
		for (i=0;i<numberofstrings;i++)
		{
			// allocate name
			if (fgets(temp,1024,fp)==NULL)
			{
				numberofstrings=i;
				strings=(OneStringToken *)realloc(strings,numberofstrings*sizeof(OneStringToken));
				break;
			}
						
			tempp=strtok(temp,"\r\n");
			
			strings[i].name=(char *)malloc((strlen(tempp)+1)*sizeof(char));
			strcpy(strings[i].name,tempp);
			//_strupr(strings[i].name);
			// allocate languages
			strings[i].data=(char **)malloc(numberoflanguages*sizeof(char *));
			for (n=0;n<numberoflanguages;n++)
			{
				if (fgets(temp,1024,fp)==NULL)
				{
					numberofstrings=i;
					strings=(OneStringToken *)realloc(strings,numberofstrings*sizeof(OneStringToken));
					break;
				}
				tempp=strtok(temp,"\r\n");
				strings[i].data[n]=(char *)malloc((strlen(tempp)+1)*sizeof(char));
				strcpy(strings[i].data[n],tempp);
			}
		}
		fclose(fp);
		return true;
	}
}


char *StringTable::GetString(char *stringname)
{
	if (actlang<numberoflanguages)
	{
		char *temp;
		temp=(char *)malloc((strlen(stringname)+1)*sizeof(char));
		strcpy(temp,stringname);
		//_strupr(temp);
		for (int i=0;i<numberofstrings;i++)
		{
			if (strcmp(temp,strings[i].name)==0)
			{
				free(temp);
				return strings[i].data[actlang];
			}
		}
		free(temp);
		return "ERROR : NO STRING";
	}
	else
	{
		return "ERROR, BAD LANG";
	}
}

