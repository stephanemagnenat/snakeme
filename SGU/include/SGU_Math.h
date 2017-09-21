/**
    SGU - SDL Graphic Utility
    Copyright (C) 2000 Luc-Olivier de Charrière

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

    
    Luc-Olivier de Charrière
*/

#ifndef __SGU_MATH_H
#define __SGU_MATH_H

#include "SGU.h"

#ifdef macintosh
#include <Quickdraw.h>
#endif

typedef struct point3D		point3D;
struct point3D{
	float	x,y,z;
};

typedef float	matrix3[3][3];

typedef struct point2D		point2D;
struct point2D{
	float	x,y;
};

struct environement3D{
	point3D		S;//eye position
	point3D		R;//eye direction vector
	point3D		Vh, Vw; //projection plans vectors
	
	point3D		E;//parallel light vector
	SGU_RGBA	L;
	int			sios;//Size of the frame, needed to center view during numeric error correction.
	
	matrix3		T;//Transormation for plan vectors.
	point3D		Vho, Vwo; //projection plans vectors, without transformation.
};

struct sphere3D{
	point3D		C;
	float		a,b,c;
	float		alpha, beta, delta;
	SGU_RGBA	A;
	matrix3		T;
};

struct spheres3D{
	sphere3D	s[32];
	int			n;
};

typedef struct point3Di		point3Di;
struct point3Di{
	int		x,y,z;
};

typedef struct point2Di		point2Di;
struct point2Di{
	int		x,y;
};
struct environement3Di{
	point3Di	S;//eye position
	point3Di	R;//eye direction vector
	point3Di	Vh, Vw; //projection plans vectors
	
	point3Di	E;//parallel light vector
	SGU_RGBA	L;
};

struct sphere3Di{
	point3Di	C;
	int			a,b,c;
	int			alpha, beta, delta;
	SGU_RGBA	A;
	matrix3		T;
};
typedef int		matrix3i[3][3];

//3D
 short SGU_Random(void);
 point3D setPoint3D(float x, float y, float z);
 point3D addPoint3D(point3D A, point3D B);
 point3D addPoints3D(point3D A, point3D B, point3D C);
 point3D difPoint3D(point3D A, point3D B);
 float scaPoint3D(point3D A, point3D B);
 float sqaPoint3D(point3D A);
 float lenPoint3D(point3D A);
 point3D mulPoint3D(float fac, point3D A);
 point3D mulPoint3D(point3D A, float fac);

point3D mulPoint3D(point3D A, matrix3 M);
point3D mulPoint3D(matrix3 M, point3D A);

 point3D mulPoint3D(point3D A, point3D B);
 point3D normPoint3D(point3D A);
//2D

point2D setPoint2D(float x, float y);
float scaPoint2D(point2D A, point2D B);

//Integer system:
point3Di setPoint3Di(point3D A);
point3Di setPoint3Di(point3D A, float fact);
point3Di setPoint3Di(float fact, point3D A);
point3Di setPoint3Di(int x, int y, int z);
point3Di addPoint3D(point3Di A, point3Di B);
point3Di addPoints3D(point3Di A, point3Di B, point3Di C);
point3Di difPoint3D(point3Di A, point3Di B);
int scaPoint3D(point3Di A, point3Di B);
int sqaPoint3D(point3Di A);
int lenPoint3D(point3Di A);
point3Di mulPoint3D(point3Di A, int fac);
point3Di mulPoint3D(int fac, point3Di A);
point3Di mulPoint3D(point3Di A, matrix3i M);
point3Di mulPoint3D(matrix3i M, point3Di A);
point2Di setPoint2Di(int x, int y);


//Matrixes
void setMatrix3(matrix3 M, float a,float b,float c,float d,float e,float f,float g,float h,float i);
void setMatrix3(matrix3 N, matrix3 R);
void setMatrix3(matrix3 R, point3D x, point3D y, point3D z);
void mulMatrix3(matrix3 M, matrix3 N, matrix3 R);
bool invMatrix3(matrix3 M, matrix3 R);
point3D columnOf(matrix3 M, int i);
point3D lineOf(matrix3 M, int i);
void sharpMatrix3(matrix3 M);
void normMatrix3(matrix3 M);

//integer matrixes:
void setMatrix3i(matrix3i M, int a,int b,int c,int d,int e,int f,int g,int h,int i);


//Others
float limitAt(float x, float lim);
int limitAti(int x, int lim);

#endif