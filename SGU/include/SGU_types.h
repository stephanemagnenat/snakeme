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

#ifndef __SGU_TYPES_H
#define __SGU_TYPES_H

typedef struct
{
	Uint8 R,G,B,A;
} SGU_RGBA;

inline SGU_RGBA SGU_RGB(Uint8 r,Uint8 g,Uint8 b,Uint8 a)
{
	SGU_RGBA ts;
	ts.R=r;
	ts.G=g;
	ts.B=b;
	ts.A=a;
	return ts;
}

#ifndef ABS
#define ABS(x) ((x>=0) ? x : -x)
#endif

#ifndef SGN
#define SGN(x) ((x>0) ? 1 : -1)
#endif

#ifndef MIN
#define MIN(a,b) ((a<b) ? a : b)
#endif

#ifndef MAX
#define MAX(a,b) ((a<b) ? b : a)
#endif

#endif