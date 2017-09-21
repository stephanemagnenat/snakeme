/* String Table services                */
/*                                      */
/* Definition file                      */
/*                                      */
/* Copyright (c) Stephane Magnenat 2000 */
/*                                      */

#ifndef __STRINGTABLE_H
#define __STRINGTABLE_H

typedef struct OneStringToken
{
	char *name;
	char **data;
} OneStringToken;

class StringTable
{
public:
	StringTable();
	~StringTable();
	void SetLang(int l) { actlang=l; }
	bool Load(const char *filename);
	const char *GetString(const char *stringname);
private:
	int actlang;
	int numberoflanguages;
	int numberofstrings;
	OneStringToken *strings;
};

#endif
