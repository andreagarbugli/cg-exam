/*
 * file GCGraLib2.h
 * by Giulio Casciola 2015
 */
#ifndef __GCGraLib_H__
#define __GCGraLib_H__
#include <SDL2/SDL.h>
#ifdef __APPLE__
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_image/SDL_image.h>
#else
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#endif

enum textquality {solid, shaded, blended};

void GC_DrawText(SDL_Renderer *ren, TTF_Font *fonttodraw, char fgR, char fgG, char fgB, char fgA,
              char bgR, char bgG, char bgB, char bgA, char text[], int x, int y,
              enum textquality quality);

void GC_DrawRect(SDL_Renderer *ren, int ,int ,int ,int);

void GC_DrawRect1(SDL_Surface *s,int ax,int ay,int width,int height, Uint32 c);

SDL_Surface* GC_LoadImage(char *file, int *exitstate);

void GC_DrawImage(SDL_Surface *srcimg, int sx, int sy, int sw, int sh, 
                       SDL_Renderer *ren, int dx, int dy);

Uint32 GC_GetPixelImage(SDL_Surface *image, int x, int y);

Uint32 GC_GetPixel(SDL_Renderer *ren, int x, int y);

//void GC_PutPixel(SDL_Renderer *ren, int x, int y);

Uint32 GC_GetPixel1(SDL_Surface *surface, int x, int y);

void GC_HorizLine1(SDL_Surface *s, int x, int x2, int y, Uint32 color);

void GC_VerticLine1(SDL_Surface *s, int x, int y, int y2, Uint32 color);

void GC_DrawCircle(SDL_Renderer *ren, int x1, int y1, int ir);

void GC_FillCircle(SDL_Renderer *ren, int xin, int yin, int rad);

void GC_DrawCircle1(SDL_Surface *s, int x1, int y1, int ir, Uint32 color);

void GC_FillCircle1(SDL_Surface *s, int xin, int yin, int rad, Uint32 color);

//void GC_DrawLine(SDL_Renderer *ren, int x0, int y0, int x1, int y1);

void GC_PutPixel1(SDL_Surface *surface, int x, int y, Uint32 pixel);

void GC_DrawLine1(SDL_Surface *s, int x0, int y0, int x1, int y1, Uint32 color);

#endif/*__GCGraLib_H__*/
