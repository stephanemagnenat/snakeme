/*  ====== SnakeDirUtils ===== *\
    (c) 1999 Stephane Magnenat
\*  ========================== */

#ifndef __SNAKEDIRUTILS_H
#define __SNAKEDIRUTILS_H

inline int DxytoDir(int dx,int dy)
{
	if ((dx<-1) || (dx>1) || (dy<-1) || (dy>1))
	{
		return -1;
	}
	static int dtodir[3][3] = { {2,2,2},{1,0,3},{0,0,0}};
	return dtodir[dx+1][dy+1];
}

inline int DirtoDx(int dir)
{
	if (dir==0)
		return 1;
	else if (dir==2)
		return -1;
	else return 0;
}

inline int DirtoDy(int dir)
{
	if (dir==1)
		return -1;
	else if (dir==3)
		return 1;
	else
		return 0;
}

inline int NLtoI(int next,int last)
{
	if ((next<0) || (next>3) ||(last<0) || (last>3))
	{
		return -1;
	}
	static int dirmid[4][4] = { {0,8,0,4}, {5,1,9,1} ,{2,6,2,10} ,{11,3,7,3}};
	return dirmid[next][last];
	// always next-actual, previous-actual
}

#endif