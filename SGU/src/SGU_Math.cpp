/**
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

    Luc-Olivier de Charrière
*/
#include "SGU_Math.h"


#ifdef macintosh
#include <Quickdraw.h>
#endif

#include <math.h>


short SGU_Random(void)
{
#ifdef macintosh
	return Random();
#endif
	return 0;
}

//Floating system:

point3D setPoint3D(float x, float y, float z)
{
	point3D returnedPoint3D;
	returnedPoint3D.x=x;
	returnedPoint3D.y=y;
	returnedPoint3D.z=z;
	return(returnedPoint3D);
}
point3D addPoint3D(point3D A, point3D B)
{
	point3D returnedPoint3D;
	returnedPoint3D.x=A.x+B.x;
	returnedPoint3D.y=A.y+B.y;
	returnedPoint3D.z=A.z+B.z;
	return(returnedPoint3D);
}
point3D addPoints3D(point3D A, point3D B, point3D C)
{
	point3D returnedPoint3D;
	returnedPoint3D.x=A.x+B.x+C.x;
	returnedPoint3D.y=A.y+B.y+C.y;
	returnedPoint3D.z=A.z+B.z+C.z;
	return(returnedPoint3D);
}
point3D difPoint3D(point3D A, point3D B)
{
	point3D returnedPoint3D;
	returnedPoint3D.x=A.x-B.x;
	returnedPoint3D.y=A.y-B.y;
	returnedPoint3D.z=A.z-B.z;
	return(returnedPoint3D);
}
float scaPoint3D(point3D A, point3D B)
{
	return(A.x*B.x + A.y*B.y + A.z*B.z);
}
float sqaPoint3D(point3D A)
{
	return(A.x*A.x + A.y*A.y + A.z*A.z);
}
float lenPoint3D(point3D A)
{
	float retFloat;
	retFloat = sqrt(A.x*A.x + A.y*A.y + A.z*A.z);
	return(retFloat);
}
point3D mulPoint3D(point3D A, float fac)
{
	point3D returnedPoint3D;
	returnedPoint3D.x=A.x*fac;
	returnedPoint3D.y=A.y*fac;
	returnedPoint3D.z=A.z*fac;
	return(returnedPoint3D);
}
point3D mulPoint3D(float fac, point3D A)
{
	point3D returnedPoint3D;
	returnedPoint3D.x=A.x*fac;
	returnedPoint3D.y=A.y*fac;
	returnedPoint3D.z=A.z*fac;
	return(returnedPoint3D);
}
point3D mulPoint3D(point3D A, matrix3 M)
{
	point3D returnedPoint3D;
	returnedPoint3D.x = A.x*M[0][0] + A.y*M[0][1] + A.z*M[0][2];
	returnedPoint3D.y = A.x*M[1][0] + A.y*M[1][1] + A.z*M[1][2];
	returnedPoint3D.z = A.x*M[2][0] + A.y*M[2][1] + A.z*M[2][2];
	return(returnedPoint3D);
}
point3D mulPoint3D(matrix3 M, point3D A)
{
	point3D returnedPoint3D;
	returnedPoint3D.x = A.x*M[0][0] + A.y*M[1][0] + A.z*M[2][0];
	returnedPoint3D.y = A.x*M[0][1] + A.y*M[1][1] + A.z*M[2][1];
	returnedPoint3D.z = A.x*M[0][2] + A.y*M[1][2] + A.z*M[2][2];
	return(returnedPoint3D);
}
point3D mulPoint3D(point3D A, point3D B)
{
	point3D returnedPoint3D;
	returnedPoint3D.x = A.y*B.z - A.z*B.y;
	returnedPoint3D.y = A.z*B.x - A.x*B.z;
	returnedPoint3D.z = A.x*B.y - A.y*B.x;
	return(returnedPoint3D);
}
point3D normPoint3D(point3D A)
{
	point3D returnedPoint3D;
	float	norm;
	norm=lenPoint3D(A);
	returnedPoint3D.x = A.x/norm;
	returnedPoint3D.y = A.y/norm;
	returnedPoint3D.z = A.z/norm;
	return(returnedPoint3D);
}

//2D system:
point2D setPoint2D(float x, float y)
{
	point2D returnedPoint2D;
	returnedPoint2D.x=x;
	returnedPoint2D.y=y;
	return(returnedPoint2D);
}
float scaPoint2D(point2D A, point2D B)
{
	return(A.x*B.x + A.y*B.y);
}

//Integer system:

point3Di setPoint3Di(point3D A)
{
	point3Di returnedPoint3D;
	returnedPoint3D.x=A.x;
	returnedPoint3D.y=A.y;
	returnedPoint3D.z=A.z;
	return(returnedPoint3D);
}
point3Di setPoint3Di(point3D A, float fact)
{
	point3Di returnedPoint3D;
	returnedPoint3D.x=(fact*A.x);
	returnedPoint3D.y=(fact*A.y);
	returnedPoint3D.z=(fact*A.z);
	return(returnedPoint3D);
}
point3Di setPoint3Di(float fact, point3D A)
{
	point3Di returnedPoint3D;
	returnedPoint3D.x=(fact*A.x);
	returnedPoint3D.y=(fact*A.y);
	returnedPoint3D.z=(fact*A.z);
	return(returnedPoint3D);
}
point3Di setPoint3Di(int x, int y, int z)
{
	point3Di returnedPoint3D;
	returnedPoint3D.x=x;
	returnedPoint3D.y=y;
	returnedPoint3D.z=z;
	return(returnedPoint3D);
}
point3Di addPoint3D(point3Di A, point3Di B)
{
	point3Di returnedPoint3D;
	returnedPoint3D.x=A.x+B.x;
	returnedPoint3D.y=A.y+B.y;
	returnedPoint3D.z=A.z+B.z;
	return(returnedPoint3D);
}
point3Di addPoints3D(point3Di A, point3Di B, point3Di C)
{
	point3Di returnedPoint3D;
	returnedPoint3D.x=A.x+B.x+C.x;
	returnedPoint3D.y=A.y+B.y+C.y;
	returnedPoint3D.z=A.z+B.z+C.z;
	return(returnedPoint3D);
}
point3Di difPoint3D(point3Di A, point3Di B)
{
	point3Di returnedPoint3D;
	returnedPoint3D.x=A.x-B.x;
	returnedPoint3D.y=A.y-B.y;
	returnedPoint3D.z=A.z-B.z;
	return(returnedPoint3D);
}
int scaPoint3D(point3Di A, point3Di B)
{
	return(A.x*B.x + A.y*B.y + A.z*B.z);
}
int sqaPoint3D(point3Di A)
{
	return(A.x*A.x + A.y*A.y + A.z*A.z);
}
int lenPoint3D(point3Di A)
{
	int retint;
	retint = sqrt(A.x*A.x + A.y*A.y + A.z*A.z);
	return(retint);
}
point3Di mulPoint3D(point3Di A, int fac)
{
	point3Di returnedPoint3D;
	returnedPoint3D.x=A.x*fac;
	returnedPoint3D.y=A.y*fac;
	returnedPoint3D.z=A.z*fac;
	return(returnedPoint3D);
}
point3Di mulPoint3D(int fac, point3Di A)
{
	point3Di returnedPoint3D;
	returnedPoint3D.x=A.x*fac;
	returnedPoint3D.y=A.y*fac;
	returnedPoint3D.z=A.z*fac;
	return(returnedPoint3D);
}
point3Di mulPoint3D(point3Di A, matrix3i M)
{
	point3Di returnedPoint3D;
	returnedPoint3D.x = A.x*M[0][0] + A.y*M[0][1] + A.z*M[0][2];
	returnedPoint3D.y = A.x*M[1][0] + A.y*M[1][1] + A.z*M[1][2];
	returnedPoint3D.z = A.x*M[2][0] + A.y*M[2][1] + A.z*M[2][2];
	return(returnedPoint3D);
}
point3Di mulPoint3D(matrix3i M, point3Di A)
{
	point3Di returnedPoint3D;
	returnedPoint3D.x = A.x*M[0][0] + A.y*M[1][0] + A.z*M[2][0];
	returnedPoint3D.y = A.x*M[0][1] + A.y*M[1][1] + A.z*M[2][1];
	returnedPoint3D.z = A.x*M[0][2] + A.y*M[1][2] + A.z*M[2][2];
	return(returnedPoint3D);
}

point2Di setPoint2Di(int x, int y)
{
	point2Di returnedPoint2D;
	returnedPoint2D.x=x;
	returnedPoint2D.y=y;
	return(returnedPoint2D);
}

//Matrix functions:

void setMatrix3(matrix3 M, float a,float b,float c,float d,float e,float f,float g,float h,float i)
{
	M[0][0] = a;
	M[0][1] = b;
	M[0][2] = c;
	M[1][0] = d;
	M[1][1] = e;
	M[1][2] = f;
	M[2][0] = g;
	M[2][1] = h;
	M[2][2] = i;
}

void setMatrix3(matrix3 R, matrix3 N)
{
	//Oh, I could have used a for(i=0...)
	R[0][0] = N[0][0];
	R[0][1] = N[0][1];
	R[0][2] = N[0][2];
	R[1][0] = N[1][0];
	R[1][1] = N[1][1];
	R[1][2] = N[1][2];
	R[2][0] = N[2][0];
	R[2][1] = N[2][1];
	R[2][2] = N[2][2];
}
void setMatrix3(matrix3 R, point3D x, point3D y, point3D z)
{
	R[0][0] = x.x;
	R[0][1] = x.y;
	R[0][2] = x.z;
	
	R[1][0] = y.x;
	R[1][1] = y.y;
	R[1][2] = y.z;
	
	R[2][0] = z.x;
	R[2][1] = z.y;
	R[2][2] = z.z;
}

void mulMatrix3(matrix3 M, matrix3 N, matrix3 R)
{
	//This calculate R = M * N
	int i, j, k;
	float sum;
	
	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
		{
			sum=0;
			for (k=0; k<3; k++)
				{
					sum+= M[i][k] * N[k][j];
				}
			R[i][j]=sum;
		}
}

inline void swapFloat(float *f1, float *f2)
{
	float f3;
	
	f3=*f1;
	*f1=*f2;
	*f2=f3;
}

bool invMatrix3(matrix3 M, matrix3 R)
{
	//This calculate R = 1/M
	int i, j, k, v;
	float ratio;
	bool good;
	
	setMatrix3(R, 1,0,0,  0,1,0,  0,0,1);
	
	
	for (i=0; i<3; i++)
	{
		good=false;
		for (j=i; j<3; j++)
			if (M[i][j]!=0)
			{
				v=j;
				good=true;
				break;
			}
		
		if (!good)
			return false;
		
		if (v!=i)//Then we swap, #i & #v
		{
			for (k=0; k<3; k++)
			{
				swapFloat(&(M[k][v]), &(M[k][i]));
				swapFloat(&(R[k][v]), &(R[k][i]));
			}
		}
		//Now we have a non null float @ #i,#i.
		
		ratio=1/M[i][i];
		
		for (k=0; k<3; k++)
		{
			M[k][v]*=ratio;
			R[k][v]*=ratio;
		}
		
		M[i][i]=1;//Let it only if debuged.
		
		for (j=0; j<3; j++)
			if (j!=i)
			{
				ratio=M[i][j];
				for (k=0; k<3; k++)
				{
					M[k][j]-=M[k][i]*ratio;
					R[k][j]-=R[k][i]*ratio;
				}
			}
	}
	
	return true;
}

point3D columnOf(matrix3 M, int i)
{
	point3D Rp;
	Rp.x=M[i][0];
	Rp.y=M[i][1];
	Rp.z=M[i][2];
	return Rp;
}

point3D lineOf(matrix3 M, int i)
{
	point3D Rp;
	Rp.x=M[0][i];
	Rp.y=M[1][i];
	Rp.z=M[2][i];
	return Rp;
}
void sharpMatrix3(matrix3 M)
{
	//this should prevent distortion.
	//It corrects matrix M so its vectors are exactly perpendicular.
	//The vector y is conservated.
	float		distortionSum;
	point3D		x,y,z;
	x=columnOf(M, 0);
	y=columnOf(M, 1);
	z=columnOf(M, 2);
	
	distortionSum=fabs(scaPoint3D(x, y))+fabs(scaPoint3D(x, z))+fabs(scaPoint3D(y, z));
	
	//1.0e-7 is very often, because errors are not guaranteed.
	//1.0e-6 is rare.
	if ( distortionSum > (1.0e-6) ) 
	{
		z=mulPoint3D(x, y);
		x=mulPoint3D(y, z);
	}
	setMatrix3(M, x,y,z);
	
}
void normMatrix3(matrix3 M)
{
	//this should prevent length distortion.
	//It corrects matrix M so its vectors are exactly unit vectors,
	//the direction is conservated
	float		distortionSum;
	point3D		x,y,z;
	x=columnOf(M, 0);
	y=columnOf(M, 1);
	z=columnOf(M, 2);
	
	x=normPoint3D(x);
	y=normPoint3D(y);
	z=normPoint3D(z);
	
	setMatrix3(M, x,y,z);
}

//Integer matrices:

void setMatrix3i(matrix3i M, int a,int b,int c,int d,int e,int f,int g,int h,int i)
{
	M[0][0] = a;
	M[0][1] = b;
	M[0][2] = c;
	M[1][0] = d;
	M[1][1] = e;
	M[1][2] = f;
	M[2][0] = g;
	M[2][1] = h;
	M[2][2] = i;
}


//Others:
float limitAt(float x, float lim)
{
	if (x>lim)
		return lim;
	else
		return x;
}
int limitAti(int x, int lim)
{
	if (x>lim)
		return lim;
	else
		return x;
}