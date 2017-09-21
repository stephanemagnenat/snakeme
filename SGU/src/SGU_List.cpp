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

#include "SGU_List.h"
#include <stdlib.h>

List::~List()
{ 
	lpListNode d;
	for(ListNode *c=Head;c;c=d)
	{
		d=c->Next;
		delete c;
	}
	Head=NULL;
	Tail=NULL;
}

bool List::IsEmpty(void)
{
	if ((Head==NULL) && (Tail==NULL))
		return true;
	else
		return false;
}

void List::AddElement(void *element)
{
	lpListNode n=new ListNode;

	n->Next=Head;
	n->Prev=NULL;
	n->Element=element;
	if(Head)
		Head->Prev=n;
	else
		Tail=n;
	Head=n;
	eln++;
}

void List::DeleteElement(void *element)
{
	for(lpListNode c=Head;c;c=c->Next)
	{
		if(c->Element==element)
		{
			if(c->Prev)
				c->Prev->Next=c->Next;
			else
				Head=c->Next;
			if(c->Next)
				c->Next->Prev=c->Prev;
			else
				Tail=c->Prev;

			delete c;
			eln--;
			return;
		}
	}
}

bool List::IsElementInList(void *element)
{
	for(lpListNode c=Head;c;c=c->Next)
	{
		if(c->Element==element)
			return true;
	}
	return false;
}

void List::Duplicate(lpList dest)
{
	dest->Clear();
	for (lpListNode c=Head;c;c=c->Next)
	{
		dest->AddElement(c->Element);
	}
}

void List::Clear(void)
{
	ListNode *d;
	for(lpListNode c=Head;c;c=d)
	{
		d=c->Next;
		delete c;
	}
	Head=NULL;
	Tail=NULL;
	eln=0;
}

void *List::GetElementN(int n)
{
	int m=0;
	void *el;
	for (lpListNode c=Head;(c&&m<=n);c=c->Next)
	{
		el=c->Element;
		m++;
	}
	return el;
}
