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

#include "SGU_GenUtils.h"
#include "SGU_PixelUtils.h"
#include "SDL.h"
#include <string.h>
#include <stdlib.h> 

// Leto/calodox
#define FIXED 8
#define I 255 /* nombre de degres different d'importance ligne/fond */
#define Ibits 8
#define Swap(a,b) swap=a; a=b; b=swap;

#ifdef macintosh
#include "ownutsText.c"
#endif


void SGU_HorzLine(SDL_Surface *surface,Sint16 x,Sint16 y,Sint16 l,SGU_RGBA c)
{
	Uint32 pixel;
    Uint8 *bits, bpp;
	Uint32 n;
	Uint8 r=c.R;
	Uint8 g=c.G;
	Uint8 b=c.B;
	
	// clip on y
	if ((y<0) || (y>=surface->h))
		return;
	
	// set l positiv
	if (l<0)
	{
		x+=l;
		l=-l;
	}

	// clip on x at left
	if (x<0)
	{
		l+=x;
		x=0;
	}

	// ignore wrong case
	if (((x+l)<0) || (x>=surface->w))
		return;

	// clip on x at right
	if ((x+l)>=surface->w)
	{
		l=surface->w-x;
	}
	
	if (l==0)
		return;

    pixel = SDL_MapRGB(surface->format, r, g, b);

    bpp = surface->format->BytesPerPixel;
    bits = ((Uint8 *)surface->pixels)+y*surface->pitch+x*bpp;

    /* Set the pixels */
    switch(bpp) 
	{
        case 1:
			for (n=0;n<(unsigned)l;n++)
			{
                *((Uint8 *)(bits)) = (Uint8)pixel;
				bits++;
			}
            break;
        case 2:
			for (n=0;n<(unsigned)l;n++)
			{
                *((Uint16 *)(bits)) = (Uint16)pixel;
				bits+=2;
			}
            break;
        case 3: 
			 /* Format/endian independent */
            Uint8 nr, ng, nb;
			for (n=0;n<(unsigned)l;n++)
			{
                nr = (pixel>>surface->format->Rshift)&0xFF;
                ng = (pixel>>surface->format->Gshift)&0xFF;
                nb = (pixel>>surface->format->Bshift)&0xFF;
                *((bits)+surface->format->Rshift/8) = nr; 
                *((bits)+surface->format->Gshift/8) = ng;
                *((bits)+surface->format->Bshift/8) = nb;
				bits+=3;
            }
            break;
        case 4:
			for (n=0;n<(unsigned)l;n++)
			{
                *((Uint32 *)(bits)) = (Uint32)pixel;
				bits+=4;
			}
            break;
    }
}


void SGU_VertLine(SDL_Surface *surface,Sint16 x,Sint16 y,Sint16 l,SGU_RGBA c)
{
	Uint32   pixel;
    Uint8   *bits, bpp;
	Uint32 n;
	Uint8 r=c.R;
	Uint8 g=c.G;
	Uint8 b=c.B;
	

	// clip on x
	if ((x<0) || (x>=surface->w))
		return;
	
	// set l positiv
	if (l<0)
	{
		y+=l;
		l=-l;
	}

	// clip on y at left
	if (y<0)
	{
		l+=y;
		y=0;
	}

	// ignore wrong case
	if (((y+l)<0) || (y>=surface->h))
		return;

	// clip on y at right
	if ((y+l)>=surface->h)
	{
		l=surface->h-y;
	}
	if (l==0)
		return;

    pixel = SDL_MapRGB(surface->format, r, g, b);

    bpp = surface->format->BytesPerPixel;
    bits = ((Uint8 *)surface->pixels)+y*surface->pitch+x*bpp;

    /* Set the pixels */
    switch(bpp) 
	{
        case 1:
			for (n=0;n<(unsigned)l;n++)
			{
                *((Uint8 *)(bits)) = (Uint8)pixel;
				bits+=surface->pitch;
			}
            break;
        case 2:
			for (n=0;n<(unsigned)l;n++)
			{
                *((Uint16 *)(bits)) = (Uint16)pixel;
				bits+=surface->pitch;
			}
            break;
        case 3: 
			 /* Format/endian independent */
            Uint8 nr, ng, nb;
			for (n=0;n<(unsigned)l;n++)
			{
                nr = (pixel>>surface->format->Rshift)&0xFF;
                ng = (pixel>>surface->format->Gshift)&0xFF;
                nb = (pixel>>surface->format->Bshift)&0xFF;
                *((bits)+surface->format->Rshift/8) = nr; 
                *((bits)+surface->format->Gshift/8) = ng;
                *((bits)+surface->format->Bshift/8) = nb;
				bits+=surface->pitch;
            }
            break;
        case 4:
			for (n=0;n<(unsigned)l;n++)
			{
                *((Uint32 *)(bits)) = (Uint32)pixel;
				bits+=surface->pitch;
			}
            break;
    }
}


void SGU_DisplayText(SDL_Surface *surface,SGU_Sprite *font,int x,int y,char *s)
{
	int n=0;
	int pos=x;
	while ((s[n]!=0)&&(pos<surface->w))
	{
		if ((s[n]<128) && (s[n]>31))
		{
			font->Display(surface,pos,y,s[n]-32);
		}
		pos+=font->GetW(n);
		n++;
	}
}

void SGU_DisplayTextCenter(SDL_Surface *surface,SGU_Sprite *font,int x,int y,int w,char *s)
{
	int dec=(w-SGU_GetTextWidth(font,s))>>1;
	SGU_DisplayText(surface,font,x+dec,y,s);
}

int SGU_GetTextWidth(SGU_Sprite *font,char *s)
{
	int l=0;
	for (int i=0;i<(int)strlen(s);i++)
	{
		l+=font->GetW(s[i]-32);
	}
	return l;
}

void SGU_Line(SDL_Surface *surface,int x1,int y1,int x2,int y2,SGU_RGBA c)
{
	Uint8 r=c.R;
	Uint8 g=c.G;
	Uint8 b=c.B;
	
	// from leto/calodox. 1999, Bresenham anti-aliased line code
    long dx, dy;
    long m,w;
    long e;
 	Sint32 littleincx;
	Sint32 littleincy;
	Sint32 bigincx;
	Sint32 bigincy;
	Sint32 alphadecx;
	Sint32 alphadecy;

    long swap;
    int test=1;
	int x;

	
    /* calcul des deltas */
    dx= x2 - x1;
    if ( dx==0)
	{
		SGU_VertLine(surface,x1,y1,y2-y1,c);
		return;
    }
    dy= y2 - y1;
    if ( dy==0) 
	{
		SGU_HorzLine(surface,x1,y1,x2-x1,c);
		return;
    }

    /* Y clipping */
    if (dy<0) 
	{
		test = -test;
		Swap(x1,x2);
		Swap(y1,y2);
		dx=-dx;
		dy=-dy;
	}
    /* the 2 points are Y-sorted. (y1<=y2) */
    if (y2 < 0) 
		return;
    if (y1 >= surface->h) 
		return;
    if (y1 < 0) 
	{
		x1 -= (y1 * dx) / dy;
		y1=0;
    }
    if (y2 >= surface->h) 
	{
		x2 -= ((y2 - surface->h)*dx) /dy;
		y2 = surface->h-1;
    }
    /* X clipping */
    if (dx<0) 
	{
		test = -test;
		Swap(x1,x2);
		Swap(y1,y2);
		dx=-dx;
		dy=-dy;
    }
    /* the 2 points are X-sorted. (x1<=x2) */
    if (x2 < 0) 
		return;
    if (x1 >= surface->w) 
		return;
    if (x1 < 0) 
	{
		y1 -= (x1 * dy) / dx;
		if (y1>=surface->h || y1<0) 
			return;
		x1=0;
    }
    if (x2 >= surface->w) 
	{
		y2 -= ((x2 - surface->w )*dy) / dx;
		x2 = surface->w-1;
    }
    dx = x2-x1;
    dy = y2-y1;

	/* prepare les variables pour dessiner la ligne
       dans la bonne direction */

    if (ABS(dx) > ABS(dy))
	{
		littleincx = 1;
		littleincy = 0;
		bigincx = 1;
		bigincy = SGN(dy);
		alphadecx = 0;
		alphadecy = SGN(dy);
    }
	else
	{
		test = -test;
		Swap(dx,dy);
		littleincx = 0;
		littleincy = 1;
		bigincx = SGN(dx);
		bigincy = 1;
		alphadecx = 1;
		alphadecy = 0;
    }

    if (dx<0)
	{
		dx= -dx;
		littleincx=0;
		littleincy=-littleincy;
		bigincx =-bigincx;
		bigincy =-bigincy;
		alphadecy=-alphadecy;
    }

    /* calcul de la position initiale */
    int px,py;
	px=x1;
	py=y1;
    
	/* initialisation des variables pour l'algo de bresenham */
    if (dx==0)
		return;
	if (dy==0)
		return;
    m = (ABS(dy)<< (Ibits+FIXED)) / ABS(dx);
    w = (I <<FIXED)-m;
    e = 1<<(FIXED-1);

    /* premier point */
	SGU_SetPixelAlpha(surface,px,py,r,g,b,(Uint8)(e>>FIXED));

    /* main loop */
    x=dx+1;
    if (x<=0) 
		return;

	while (--x) 
	{
		if (e < w) 
		{
			px+=littleincx;
			py+=littleincy;
			e+= m;
		} 
		else 
		{
			px+=bigincx;
			py+=bigincy;
			e-= w;
		}
		if ((surface->h-y2)>1)
			SGU_SetPixelAlpha(surface,px,py,r,g,b,(Uint8)(e>>FIXED));
		SGU_SetPixelAlpha(surface,px+alphadecx,py+alphadecy,r,g,b,(Uint8)(I-(e>>FIXED)));
	}
    

}

void SGU_Rect(SDL_Surface *surface,int x1,int y1,int w,int h,SGU_RGBA c)
{
	SGU_HorzLine(surface,x1,y1,w,c);
	SGU_HorzLine(surface,x1,y1+h-1,w,c);
	SGU_VertLine(surface,x1,y1,h,c);
	SGU_VertLine(surface,x1+w-1,y1,h,c);
}

void SGU_FillRect(SDL_Surface *surface,int x,int y,int w,int h,SGU_RGBA c)
{
	SDL_Rect rect;
	Uint32 color;
	rect.x=(Sint16)x;
	rect.y=(Sint16)y;
	rect.w=(Uint16)w;
	rect.h=(Uint16)h;
	color=SDL_MapRGB(surface->format,c.R,c.G,c.B);
	SDL_FillRect(surface,&rect,color);
}

void SGU_FillRectAlpha(SDL_Surface *surface,int x,int y,int w,int h,SGU_RGBA c)
{
	int cx,cy;
	Uint32 pixel;
    Uint8 *bits, bpp;
	Uint8 tr,tg,tb;
	Uint32 r=c.R;
	Uint32 g=c.G;
	Uint32 b=c.B;
	Uint32 a=c.A;
	

	if ((x>=surface->w) || (y>=surface->h))
		return;
	if (x<0)
	{
		w-=x;
		x=0;
	}
	if (y<0)
	{
		h-=y;
		y=0;
	}
	if ((x+w)>surface->w)
	{
		w-=surface->w-x;
	}
	if ((y+h)>=surface->h)
	{
		h-=surface->h-y;
	}

    bpp = surface->format->BytesPerPixel;
	bits = ((Uint8 *)surface->pixels)+y*surface->pitch+x*bpp;				
	// Fill with alpha, premult
	r*=(255-a);
	g*=(255-a);
	b*=(255-a);
    switch(bpp) 
	{
        case 1:
			{
				for (cy=h-1;cy>=0;cy--)
				{
					bits = ((Uint8 *)surface->pixels)+(y+cy)*surface->pitch+x*bpp;
					for (cx=w-1;cx>=0;cx--)
					{
						pixel=*((Uint8 *)(bits));
						SDL_GetRGB(pixel,surface->format,&tr,&tg,&tb);
						tr=(r+tr*a)>>8;
						tg=(g+tg*a)>>8;
						tb=(b+tb*a)>>8;
						pixel=SDL_MapRGB(surface->format,tr,tg,tb);
						*((Uint8 *)(bits)) = (Uint8)pixel;
						bits+=1;
					}
				}
			}
            break;
		case 2:
			{
				for (cy=h-1;cy>=0;cy--)
				{
					bits = ((Uint8 *)surface->pixels)+(y+cy)*surface->pitch+x*bpp;
					for (cx=w-1;cx>=0;cx--)
					{
						pixel=*((Uint16 *)(bits));
						SDL_GetRGB(pixel,surface->format,&tr,&tg,&tb);
						tr=(r+tr*a)>>8;
						tg=(g+tg*a)>>8;
						tb=(b+tb*a)>>8;
						pixel=SDL_MapRGB(surface->format,tr,tg,tb);
						*((Uint16 *)(bits)) = (Uint16)pixel;
						bits+=2;
					}
				}
			}
            break;
        case 3: 
			{
				for (cy=h-1;cy>=0;cy--)
				{
					bits = ((Uint8 *)surface->pixels)+(y+cy)*surface->pitch+x*bpp;
					for (cx=w-1;cx>=0;cx--)
					{
						/*pixel = SDL_MapRGB(surface->format, r, g, b);
						// this stupid code is for endian conveniance
						tr = (pixel>>surface->format->Rshift)&0xFF;
						tg = (pixel>>surface->format->Gshift)&0xFF;
						tb = (pixel>>surface->format->Bshift)&0xFF;*/
						pixel=*((Uint32 *)(bits));
						SDL_GetRGB(pixel,surface->format,&tr,&tg,&tb);
						tr=(r+tr*a)>>8;
						tg=(g+tg*a)>>8;
						tb=(b+tb*a)>>8;
						*((bits)+surface->format->Rshift/8) = tr; 
						*((bits)+surface->format->Gshift/8) = tg;
						*((bits)+surface->format->Bshift/8) = tb;
						bits+=3;
					}
				}
            }
            break;
        case 4:
			{
				for (cy=h-1;cy>=0;cy--)
				{
					bits = ((Uint8 *)surface->pixels)+(y+cy)*surface->pitch+x*bpp;
					for (cx=w-1;cx>=0;cx--)
					{
						pixel=*((Uint32 *)(bits));
						SDL_GetRGB(pixel,surface->format,&tr,&tg,&tb);
						tr=(r+tr*a)>>8;
						tg=(g+tg*a)>>8;
						tb=(b+tb*a)>>8;
						pixel=SDL_MapRGB(surface->format,tr,tg,tb);
						*((Uint32 *)(bits)) = (Uint32)pixel;
						bits+=4;
					}
				}					
			}
            break;
    }
}

void SGU_HorzLineShade(SDL_Surface *surface,Sint16 x,Sint16 y,Sint16 l,SGU_RGBA c1,SGU_RGBA c2)
{
	Uint32 pixel;
    Uint8 *bits, bpp;
	Uint32 n;
	Uint8 r1=c1.R;
	Uint8 g1=c1.G;
	Uint8 b1=c1.B;
	Uint8 r2=c2.R;
	Uint8 g2=c2.G;
	Uint8 b2=c2.B;


	// clip on y
	if ((y<0) || (y>=surface->h))
		return;
	
	// set l positiv
	if (l<0)
	{
		x+=l;
		l=-l;
	}

	// clip on x at left
	if (x<0)
	{
		l-=x;
		x=0;
	}

	// ignore wrong case
	if (((x+l)<0) || (x>=surface->w))
		return;

	// clip on x at right
	if ((x+l)>=surface->w)
	{
		l=surface->w-x;
	}
	if (l==0)
		return;

	bpp = surface->format->BytesPerPixel;
    bits = ((Uint8 *)surface->pixels)+y*surface->pitch+x*bpp;

    /* Set the pixels */
    switch(bpp) 
	{
        case 1:
			for (n=0;n<(unsigned)l;n++)
			{
				pixel = SDL_MapRGB(surface->format, (r1*(l-n)+r2*n)/l,  (g1*(l-n)+g2*n)/l,  (b1*(l-n)+b2*n)/l);
                *((Uint8 *)(bits)) = (Uint8)pixel;
				bits++;
			}
            break;
        case 2:
			for (n=0;n<(unsigned)l;n++)
			{
				pixel = SDL_MapRGB(surface->format, (r1*(l-n)+r2*n)/l,  (g1*(l-n)+g2*n)/l,  (b1*(l-n)+b2*n)/l);
                *((Uint16 *)(bits)) = (Uint16)pixel;
				bits+=2;
			}
            break;
        case 3: 
			 /* Format/endian independent */
            Uint8 nr, ng, nb;
			for (n=0;n<(unsigned)l;n++)
			{
				pixel = SDL_MapRGB(surface->format, (r1*(l-n)+r2*n)/l,  (g1*(l-n)+g2*n)/l,  (b1*(l-n)+b2*n)/l);
                nr = (pixel>>surface->format->Rshift)&0xFF;
                ng = (pixel>>surface->format->Gshift)&0xFF;
                nb = (pixel>>surface->format->Bshift)&0xFF;
                *((bits)+surface->format->Rshift/8) = nr; 
                *((bits)+surface->format->Gshift/8) = ng;
                *((bits)+surface->format->Bshift/8) = nb;
				bits+=3;
            }
            break;
        case 4:
			for (n=0;n<(unsigned)l;n++)
			{
				pixel = SDL_MapRGB(surface->format, (r1*(l-n)+r2*n)/l,  (g1*(l-n)+g2*n)/l,  (b1*(l-n)+b2*n)/l);
                *((Uint32 *)(bits)) = (Uint32)pixel;
				bits+=4;
			}
            break;
    }
}

void SGU_VertLineShade(SDL_Surface *surface,Sint16 x,Sint16 y,Sint16 l,SGU_RGBA c1,SGU_RGBA c2)
{
	Uint32 pixel;
    Uint8 *bits, bpp;
	Uint32 n;
	Uint8 r1=c1.R;
	Uint8 g1=c1.G;
	Uint8 b1=c1.B;
	Uint8 r2=c2.R;
	Uint8 g2=c2.G;
	Uint8 b2=c2.B;

	// clip on x
	if ((x<0) || (x>=surface->w))
		return;
	
	// set l positiv
	if (l<0)
	{
		y+=l;
		l=-l;
	}

	// clip on y at left
	if (y<0)
	{
		l-=y;
		y=0;
	}

	// ignore wrong case
	if (((y+l)<0) || (y>=surface->h))
		return;

	// clip on y at right
	if ((y+l)>=surface->h)
	{
		l=surface->h-y;
	}
	if (l==0)
		return;

	bpp = surface->format->BytesPerPixel;
    bits = ((Uint8 *)surface->pixels)+y*surface->pitch+x*bpp;

    /* Set the pixels */
    switch(bpp) 
	{
        case 1:
			for (n=0;n<(unsigned)l;n++)
			{
				pixel = SDL_MapRGB(surface->format, (r1*(l-n)+r2*n)/l,  (g1*(l-n)+g2*n)/l,  (b1*(l-n)+b2*n)/l);
                *((Uint8 *)(bits)) = (Uint8)pixel;
				bits+=surface->pitch;
			}
            break;
        case 2:
			for (n=0;n<(unsigned)l;n++)
			{
				pixel = SDL_MapRGB(surface->format, (r1*(l-n)+r2*n)/l,  (g1*(l-n)+g2*n)/l,  (b1*(l-n)+b2*n)/l);
                *((Uint16 *)(bits)) = (Uint16)pixel;
				bits+=surface->pitch;
			}
            break;
        case 3: 
			 /* Format/endian independent */
            Uint8 nr, ng, nb;
			for (n=0;n<(unsigned)l;n++)
			{
				pixel = SDL_MapRGB(surface->format, (r1*(l-n)+r2*n)/l,  (g1*(l-n)+g2*n)/l,  (b1*(l-n)+b2*n)/l);
                nr = (pixel>>surface->format->Rshift)&0xFF;
                ng = (pixel>>surface->format->Gshift)&0xFF;
                nb = (pixel>>surface->format->Bshift)&0xFF;
                *((bits)+surface->format->Rshift/8) = nr; 
                *((bits)+surface->format->Gshift/8) = ng;
                *((bits)+surface->format->Bshift/8) = nb;
				bits+=surface->pitch;
            }
            break;
        case 4:
			for (n=0;n<(unsigned)l;n++)
			{
				pixel = SDL_MapRGB(surface->format, (r1*(l-n)+r2*n)/l,  (g1*(l-n)+g2*n)/l,  (b1*(l-n)+b2*n)/l);
                *((Uint32 *)(bits)) = (Uint32)pixel;
				bits+=surface->pitch;
			}
            break;
    }
}

void SGU_RectShade(SDL_Surface *surface,int x,int y,int w,int h,SGU_RGBA c1,SGU_RGBA ct,SGU_RGBA c2)
{
	SGU_HorzLineShade(surface,x,y,w,c1,ct);
	SGU_HorzLineShade(surface,x,y+h-1,w,ct,c2);
	SGU_VertLineShade(surface,x,y,h,c1,ct);
	SGU_VertLineShade(surface,x+w-1,y,h,ct,c2);
}
#define IFNOTTR(r,g,b,x) if (!((r==255) && (g==0) && (b==255))) x
#define IFTR(r,g,b,x) if ((r>245) && (g<10) && (b>245)) x

void SGU_DoubleSizeBlit(SDL_Surface *src,SDL_Surface *dst,int x,int y)
{
	int xn,yn;
	Uint8 r1,g1,b1;
	Uint8 r2,g2,b2;
	Uint32 r,g,b;

	if ((x<0) || (y<0) || (x+2*src->w>dst->w) ||(y+2*src->h>dst->h))
		return;

	// pass 1, horiz fill
	for (yn=0;yn<src->h;yn++)
	{
		SGU_GetPixel(src,0,yn,&r1,&g1,&b1);
		IFTR(r1,g1,b1,SGU_GetPixel(dst,x+0,y+(yn<<1),&r1,&g1,&b1));
		SGU_SetPixel(dst,x+0,y+(yn<<1),r1,g1,b1);
		for (xn=1;xn<src->w;xn++)
		{
			SGU_GetPixel(src,xn-1,yn,&r1,&g1,&b1);
			SGU_GetPixel(src,xn,yn,&r2,&g2,&b2);
			// If we detect any transaprence, look on destination
			IFTR(r1,g1,b1,SGU_GetPixel(dst,x+(xn<<1)-1,y+(yn<<1),&r1,&g1,&b1));
			IFTR(r2,g2,b2,SGU_GetPixel(dst,x+(xn<<1),y+(yn<<1),&r2,&g2,&b2));
			r=(r1+r2);
			g=(g1+g2);
			b=(b1+b2);
			SGU_SetPixel(dst,x+(xn<<1)-1,y+(yn<<1),r>>1,g>>1,b>>1);
			SGU_SetPixel(dst,x+(xn<<1),y+(yn<<1),r2,g2,b2);
		}
	}
	// pass 2,vertical fill
	for (yn=1;yn<src->h;yn++)
	{
		for (xn=0;xn<(src->w<<1)-1;xn++)
		{
			SGU_GetPixel(dst,x+xn,y+((yn-1)<<1),&r1,&g1,&b1);
			SGU_GetPixel(dst,x+xn,y+(yn<<1),&r2,&g2,&b2);
			r=(r1+r2);
			g=(g1+g2);
			b=(b1+b2);
			SGU_SetPixel(dst,x+xn,y+(yn<<1)-1,r>>1,g>>1,b>>1);
			SGU_SetPixel(dst,x+xn,y+(yn<<1),r2,g2,b2);
		}
	}

}

bool SGU_RegionSaver::SaveRegion(SDL_Surface *surface,int rx,int ry,int rw,int rh)
{
	rect.x=rx;
	rect.y=ry;
	rect.w=rw;
	rect.h=rh;
	SDL_Rect trect;
	trect.x=0;
	trect.y=0;
	trect.w=rw;
	trect.h=rh;
	if (data)
		SDL_FreeSurface(data);

	data=SDL_CreateRGBSurface(SDL_SWSURFACE,
											rw,
											rh,
											surface->format->BitsPerPixel,
											surface->format->Rmask,
											surface->format->Gmask,
											surface->format->Bmask,
											surface->format->Amask);
	return (!SDL_BlitSurface(surface,&rect,data,&trect));
}

bool SGU_RegionSaver::RestaurRegion(SDL_Surface *surface)
{
	if (data)
	{
		SDL_Rect trect;
		trect.x=0;
		trect.y=0;
		trect.w=data->w;
		trect.h=data->h;
		return (!SDL_BlitSurface(data,&trect,surface,&rect));
	}
	else
		return false;
}

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
