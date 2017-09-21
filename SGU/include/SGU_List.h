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

#ifndef __SGU_LIST_H
#define __SGU_LIST_H

#include <stdlib.h>

struct ListNode
{
	void *Element;
	ListNode *Prev, *Next;
};

typedef ListNode *lpListNode;

class List;
typedef List *lpList;

class List
{
private:
	ListNode *Head, *Tail;
	int eln;
public:
	List() { Head=NULL; Tail=NULL; eln=0;}
	~List();

	void AddElement(void *element);
	void DeleteElement(void *element);
	bool IsElementInList(void *element);
	bool IsEmpty(void);
	void Clear(void);
	void Duplicate(lpList dest);
	lpListNode GetHead(void) { return Head; }
	lpListNode GetTail(void) { return Tail; }
	void SetEmpty(void) { Head=NULL; Tail=NULL; }
	int GetNbElement(void) { return eln; }
	void *GetElementN(int n);
};

class StringList:public List
{
public:
	void AddStringElement(char *s);
	~StringList();
};

#endif