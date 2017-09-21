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

#ifndef __SGU_PIXELUTILS_H
#define __SGU_PIXELUTILS_H

	// Get a pixel from a given SDL Surface 
inline void SGU_GetPixel(SDL_Surface *surface,Sint16 x,Sint16 y,Uint8 *r,Uint8 *g,Uint8 *b)
{
	Uint32   pixel;
    Uint8   *bits, bpp;

	if ((x<0) || (x>=surface->w) || (y<0) || (y>=surface->h))
		return;

	bpp = surface->format->BytesPerPixel;
    bits = ((Uint8 *)surface->pixels)+y*surface->pitch+x*bpp;

    /* Set the pixel */
    switch(bpp) {
        case 1:
            pixel=*((Uint8 *)(bits));
			SDL_GetRGB(pixel,surface->format,r,g,b);
            break;
        case 2:
            pixel=*((Uint16 *)(bits));
			SDL_GetRGB(pixel,surface->format,r,g,b);
            break;
        case 3: 
			{ /* Format/endian independent */
				// ignore 24 bpp
                /*Uint8 nr, ng, nb;
				
                nr = (pixel>>surface->format->Rshift)&0xFF;
                ng = (pixel>>surface->format->Gshift)&0xFF;
                nb = (pixel>>surface->format->Bshift)&0xFF;
                *((bits)+surface->format->Rshift/8) = nr; 
                *((bits)+surface->format->Gshift/8) = ng;
                *((bits)+surface->format->Bshift/8) = nb;*/
            }
            //break;
        case 4:
            pixel=*((Uint32 *)(bits));
			SDL_GetRGB(pixel,surface->format,r,g,b);
            break;
    }
}
	/// Set a pixel of a given SDL Surface with a given colour
inline void SGU_SetPixel(SDL_Surface *surface,Sint16 x,Sint16 y,Uint8 r,Uint8 g,Uint8 b)
{
    Uint32   pixel;
    Uint8   *bits, bpp;

	if ((x<0) || (x>=surface->w) || (y<0) || (y>=surface->h))
		return;

    pixel = SDL_MapRGB(surface->format, r, g, b);

    bpp = surface->format->BytesPerPixel;
    bits = ((Uint8 *)surface->pixels)+y*surface->pitch+x*bpp;

    /* Set the pixel */
    switch(bpp) {
        case 1:
            *((Uint8 *)(bits)) = (Uint8)pixel;
			break;
        case 2:
            *((Uint16 *)(bits)) = (Uint16)pixel;
            break;
        case 3: 
			{ /* Format/endian independent */
                Uint8 nr, ng, nb;

                nr = (pixel>>surface->format->Rshift)&0xFF;
                ng = (pixel>>surface->format->Gshift)&0xFF;
                nb = (pixel>>surface->format->Bshift)&0xFF;
                *((bits)+surface->format->Rshift/8) = nr; 
                *((bits)+surface->format->Gshift/8) = ng;
                *((bits)+surface->format->Bshift/8) = nb;
            }
            break;
        case 4:
            *((Uint32 *)(bits)) = (Uint32)pixel;
            break;
    }
}

	/// Set a pixel of a given SDL Surface with a given colour with alpha
inline void SGU_SetPixelAlpha(SDL_Surface *surface,Sint16 x,Sint16 y,Uint8 r,Uint8 g,Uint8 b,Uint8 a)
{
    Uint32 pixel;
    Uint8 *bits, bpp;
	Uint8 tr,tg,tb;

	if ((x<0) || (x>=surface->w) || (y<0) || (y>=surface->h))
		return;

    bpp = surface->format->BytesPerPixel;
    bits = ((Uint8 *)surface->pixels)+y*surface->pitch+x*bpp;

    /* Set the pixel */
    switch(bpp) 
	{
        case 1:
			{
				pixel=*((Uint8 *)(bits));
				SDL_GetRGB(pixel,surface->format,&tr,&tg,&tb);
				tr=(r*(255-a)+tr*a)>>8;
				tg=(g*(255-a)+tg*a)>>8;
				tb=(b*(255-a)+tb*a)>>8;
				pixel=SDL_MapRGB(surface->format,tr,tg,tb);
				*((Uint8 *)(bits)) = (Uint8)pixel;
			}
            break;
		case 2:
			{
				pixel=*((Uint16 *)(bits));
				SDL_GetRGB(pixel,surface->format,&tr,&tg,&tb);
				tr=(r*(255-a)+tr*a)>>8;
				tg=(g*(255-a)+tg*a)>>8;
				tb=(b*(255-a)+tb*a)>>8;
				pixel=SDL_MapRGB(surface->format,tr,tg,tb);
                *((Uint16 *)(bits)) = (Uint16)pixel;
			}
            break;
        case 3: 
			{
				pixel=*((Uint32 *)(bits));
				SDL_GetRGB(pixel,surface->format,&tr,&tg,&tb);
				// this stupid code is for endian conveniance
				/*
				tr = (pixel>>surface->format->Rshift)&0xFF;
                tg = (pixel>>surface->format->Gshift)&0xFF;
                tb = (pixel>>surface->format->Bshift)&0xFF;*/
				tr=(r*(255-a)+tr*a)>>8;
				tg=(g*(255-a)+tg*a)>>8;
				tb=(b*(255-a)+tb*a)>>8;
                *((bits)+surface->format->Rshift/8) = tr; 
                *((bits)+surface->format->Gshift/8) = tg;
                *((bits)+surface->format->Bshift/8) = tb;
            }
            break;
        case 4:
			{
				pixel=*((Uint32 *)(bits));
				SDL_GetRGB(pixel,surface->format,&tr,&tg,&tb);
				tr=(r*(255-a)+tr*a)>>8;
				tg=(g*(255-a)+tg*a)>>8;
				tb=(b*(255-a)+tb*a)>>8;
				pixel=SDL_MapRGB(surface->format,tr,tg,tb);
				*((Uint32 *)(bits)) = (Uint32)pixel;
			}
            break;
    }
}

#endif