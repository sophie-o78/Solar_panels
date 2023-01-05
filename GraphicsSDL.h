#ifndef GRAPHICSDLH
#define GRAPHICSDLH

#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "simulation.h"
#include "filesmanagement.h"

#define WINDOW_WIDTH 720
#define WINDOW_HEIGHT 720
#define GRAPHWIDTH 600
#define OFFSETGRAPH (int)(WINDOW_WIDTH - GRAPHWIDTH) / 2
#define ARROWLENGHT 3
#define LEGENDDOWNOFFSET 40
#define CROSSLEN 2
#define STEPPEROFFSET 10
#define FLOATERROR -0.01

void handlegraph(float *tab, int len, char *legend, float xmax, float ymax, char *legendx, char *legendy, float rez, int xlegendDisplayPeriod, int ylegendDisplayPeriod);
void SDL_ExitWithError(const char *message);
void drawGraphicBase(SDL_Renderer *renderer, char *legend, float xmax, float ymax, char *legendx, char *legendy, float rez, int xlegendDisplayPeriod, int ylegendDisplayPeriod, float XOffset);
float *GenerateTableFromFunct(float (*function)(float), float increment, int nbdata);
void drawgraphfromTab(SDL_Renderer *renderer, float *data, int nbdata, float increment, float ymax);
void drawgraphfromTabRed(SDL_Renderer *renderer, float *data, int nbdata, float increment, float ymax);
float xaucube(float x);
void handlegraphYear(Maison *unemaison, int len, float xmax, float ymax, float rez, int xlegendDisplayPeriod, int ylegendDisplayPeriod);
void handlegraphMonthComp(Maison *unemaison, int len, float xmax, float ymax, float rez, int xlegendDisplayPeriod, int ylegendDisplayPeriod);
void handlegraphDay(Maison *unemaison, int len, float xmax, float ymax, float rez, int xlegendDisplayPeriod, int ylegendDisplayPeriod);
#endif