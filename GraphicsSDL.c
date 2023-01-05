
#include "GraphicsSDL.h"

void handlegraph(float *tab, int len, char *legend, float xmax, float ymax, char *legendx, char *legendy, float rez, int xlegendDisplayPeriod, int ylegendDisplayPeriod)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Renderer *renderertext = NULL;

    SDL_Surface *icon = IMG_Load("icon.png");

    //Lancement SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        SDL_ExitWithError("Initialisation SDL");
    TTF_Init();
    IMG_Init(IMG_INIT_JPG);

    //Création fenêtre
    if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer) != 0)
        SDL_ExitWithError("Creation fenetre et rendu echouee");

    SDL_SetWindowTitle(window, "A simple Graph");
    SDL_SetWindowIcon(window, icon);

    drawGraphicBase(renderer, legend, xmax, ymax, legendx, legendy, rez, xlegendDisplayPeriod, ylegendDisplayPeriod, 0);
    drawgraphfromTab(renderer, tab, len, rez, ymax);
    SDL_RenderPresent(renderer);

    SDL_RenderPresent(renderer);

    /*---------------------------  Boucle SDL   ---------------------------------*/

    SDL_bool program_launched = SDL_TRUE;
    while (program_launched)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {

            case SDL_QUIT:
                program_launched = SDL_FALSE;
                break;
            default:
                break;
            }
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(20);
    }
    /*------------------------------------------------------------*/

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_FreeSurface(icon);

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void SDL_ExitWithError(const char *message)
{
    SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}

void drawGraphicBase(SDL_Renderer *renderer, char *legend, float xmax, float ymax, char *legendx, char *legendy, float rez, int xlegendDisplayPeriod, int ylegendDisplayPeriod, float XOffset)
{

    SDL_Rect whitebackground;
    whitebackground.x = 0;
    whitebackground.y = 0;
    whitebackground.w = WINDOW_WIDTH;
    whitebackground.h = WINDOW_HEIGHT;

    if (SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255) != 0)
    {
        SDL_ExitWithError("Erreur changement de couleur");
        exit(EXIT_FAILURE);
    }

    if (SDL_RenderFillRect(renderer, &whitebackground) != 0) //on remplit le rectangle avec la bonne couleur
    {
        SDL_ExitWithError("Erreur fill rectangle");
        exit(EXIT_FAILURE);
    }

    if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255) != 0)
    {
        SDL_ExitWithError("Erreur changement de couleur");
        exit(EXIT_FAILURE);
    }

    //AXE Y
    if (SDL_RenderDrawLine(renderer, OFFSETGRAPH, OFFSETGRAPH, OFFSETGRAPH, GRAPHWIDTH + OFFSETGRAPH + CROSSLEN) != 0)
    {
        SDL_ExitWithError("Erreur Draw Line");
        exit(EXIT_FAILURE);
    }

    //AXE X
    if (SDL_RenderDrawLine(renderer, OFFSETGRAPH - CROSSLEN, OFFSETGRAPH + GRAPHWIDTH, OFFSETGRAPH + GRAPHWIDTH, OFFSETGRAPH + GRAPHWIDTH) != 0)
    {
        SDL_ExitWithError("Erreur Draw Line");
        exit(EXIT_FAILURE);
    }

    //AXE X \ //
    if (SDL_RenderDrawLine(renderer, OFFSETGRAPH + GRAPHWIDTH, OFFSETGRAPH + GRAPHWIDTH, GRAPHWIDTH + OFFSETGRAPH - ARROWLENGHT, GRAPHWIDTH + OFFSETGRAPH - ARROWLENGHT) != 0)
    {
        SDL_ExitWithError("Erreur Draw Line");
        exit(EXIT_FAILURE);
    }

    // AXEX / //
    if (SDL_RenderDrawLine(renderer, OFFSETGRAPH + GRAPHWIDTH, OFFSETGRAPH + GRAPHWIDTH, GRAPHWIDTH + OFFSETGRAPH - ARROWLENGHT, GRAPHWIDTH + OFFSETGRAPH + ARROWLENGHT) != 0)
    {
        SDL_ExitWithError("Erreur Draw Line");
        exit(EXIT_FAILURE);
    }

    // AXEY \ //
    if (SDL_RenderDrawLine(renderer, OFFSETGRAPH, OFFSETGRAPH, OFFSETGRAPH + ARROWLENGHT, OFFSETGRAPH + ARROWLENGHT) != 0)
    {
        SDL_ExitWithError("Erreur Draw Line");
        exit(EXIT_FAILURE);
    }

    // AXEY / //
    if (SDL_RenderDrawLine(renderer, OFFSETGRAPH, OFFSETGRAPH, OFFSETGRAPH - ARROWLENGHT, OFFSETGRAPH + ARROWLENGHT) != 0)
    {
        SDL_ExitWithError("Erreur Draw Line");
        exit(EXIT_FAILURE);
    }

    TTF_Font *font = TTF_OpenFont("arial.ttf", 12);
    SDL_Color colorBlack = {0, 0, 0};

    //LEGEND
    SDL_RenderPresent(renderer);

    TTF_SetFontStyle(font, TTF_STYLE_UNDERLINE);

    SDL_Surface *surface = TTF_RenderText_Solid(font, legend, colorBlack);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = {OFFSETGRAPH + GRAPHWIDTH / 2 - texW / 2, OFFSETGRAPH + GRAPHWIDTH + LEGENDDOWNOFFSET - texH / 2, texW, texH};

    SDL_RenderCopy(renderer, texture, NULL, &dstrect);

    TTF_SetFontStyle(font, TTF_STYLE_NORMAL);

    //LEGENDX

    surface = TTF_RenderText_Solid(font, legendx, colorBlack);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

    dstrect.x = OFFSETGRAPH + GRAPHWIDTH - texW / 2;
    dstrect.y = OFFSETGRAPH + GRAPHWIDTH + LEGENDDOWNOFFSET - texH / 2;
    dstrect.w = texW;
    dstrect.h = texH;

    SDL_RenderCopy(renderer, texture, NULL, &dstrect);

    //LEGENDY
    surface = TTF_RenderText_Solid(font, legendy, colorBlack);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

    dstrect.x = OFFSETGRAPH - texW / 2 - LEGENDDOWNOFFSET;
    dstrect.y = OFFSETGRAPH - texH / 2;
    dstrect.w = texW;
    dstrect.h = texH;

    SDL_RenderCopyEx(renderer, texture, NULL, &dstrect, 270, NULL, SDL_FLIP_NONE);

    float nf = xmax / rez;
    int ni = nf;
    Bool near1inf = (nf - ni - 1) > (FLOATERROR);
    int n = nf + near1inf;

    float rezy = ymax / (xmax / rez);
    int rezspat = GRAPHWIDTH / n;

    font = TTF_OpenFont("arial.ttf", 20);

    for (int i = 1; i < n; i++)
    {
        if (i % ylegendDisplayPeriod == 1 || ylegendDisplayPeriod == 1)
        {
            char nombre[6];
            sprintf(nombre, "%3.1f", i * rezy);

            surface = TTF_RenderText_Solid(font, nombre, colorBlack);
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

            dstrect.x = OFFSETGRAPH - texW / 2 - STEPPEROFFSET;
            dstrect.y = OFFSETGRAPH + GRAPHWIDTH - i * rezspat - texH / 2;
            dstrect.w = texW;
            dstrect.h = texH;

            SDL_RenderCopyEx(renderer, texture, NULL, &dstrect, 270, NULL, SDL_FLIP_NONE);
        }

        if (SDL_RenderDrawLine(renderer, OFFSETGRAPH - ARROWLENGHT, OFFSETGRAPH + GRAPHWIDTH - i * rezspat, OFFSETGRAPH + ARROWLENGHT, OFFSETGRAPH + GRAPHWIDTH - i * rezspat) != 0)
        {
            SDL_ExitWithError("Erreur Draw Line");
            exit(EXIT_FAILURE);
        }

        if (SDL_SetRenderDrawColor(renderer, 153, 153, 153, 50) != 0)
        {
            SDL_ExitWithError("Erreur changement de couleur");
            exit(EXIT_FAILURE);
        }
        if (SDL_RenderDrawLine(renderer, OFFSETGRAPH + ARROWLENGHT + 1, OFFSETGRAPH + GRAPHWIDTH - i * rezspat, OFFSETGRAPH + GRAPHWIDTH, OFFSETGRAPH + GRAPHWIDTH - i * rezspat) != 0)
        {
            SDL_ExitWithError("Erreur Draw Line");
            exit(EXIT_FAILURE);
        }

        if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255) != 0)
        {
            SDL_ExitWithError("Erreur changement de couleur");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 1; i < n; i++)
    {
        if (i % xlegendDisplayPeriod == 1 || xlegendDisplayPeriod == 1)
        {
            char nombre[6];
            sprintf(nombre, "%3.1f", (i * rez + XOffset));

            surface = TTF_RenderText_Solid(font, nombre, colorBlack);
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

            dstrect.x = OFFSETGRAPH + i * rezspat - texW / 2;
            dstrect.y = OFFSETGRAPH + GRAPHWIDTH - texH / 2 + STEPPEROFFSET;
            dstrect.w = texW;
            dstrect.h = texH;
            SDL_RenderCopy(renderer, texture, NULL, &dstrect);
        }

        if (SDL_RenderDrawLine(renderer, OFFSETGRAPH + i * rezspat, OFFSETGRAPH + GRAPHWIDTH - ARROWLENGHT, OFFSETGRAPH + i * rezspat, OFFSETGRAPH + GRAPHWIDTH + ARROWLENGHT) != 0)
        {
            SDL_ExitWithError("Erreur Draw Line");
            exit(EXIT_FAILURE);
        }
        if (SDL_SetRenderDrawColor(renderer, 153, 153, 153, 50) != 0)
        {
            SDL_ExitWithError("Erreur changement de couleur");
            exit(EXIT_FAILURE);
        }

        if (SDL_RenderDrawLine(renderer, OFFSETGRAPH + i * rezspat, OFFSETGRAPH + GRAPHWIDTH - ARROWLENGHT - 1, OFFSETGRAPH + i * rezspat, OFFSETGRAPH) != 0)
        {
            SDL_ExitWithError("Erreur Draw Line");
            exit(EXIT_FAILURE);
        }

        if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255) != 0)
        {
            SDL_ExitWithError("Erreur changement de couleur");
            exit(EXIT_FAILURE);
        }
    }

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

float *GenerateTableFromFunct(float (*function)(float), float increment, int nbdata)
{
    float *tab = calloc(nbdata, sizeof(float));
    for (int i = 0; i < nbdata; i++)
    {
        tab[i] = (*function)(i * increment);
    }

    return tab;
}

void drawgraphfromTab(SDL_Renderer *renderer, float *data, int nbdata, float increment, float ymax)
{
    if (SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255) != 0)
    {
        SDL_ExitWithError("Erreur changement de couleur");
        exit(EXIT_FAILURE);
    }

    int incrementspat = GRAPHWIDTH / nbdata;

    for (int i = 0; i < nbdata - 1; i++)
    {
        int x1 = OFFSETGRAPH + (float)((i * incrementspat));
        int x2 = OFFSETGRAPH + (float)(((i + 1) * incrementspat));
        int y1 = OFFSETGRAPH + GRAPHWIDTH - (float)((data[i] / ymax) * GRAPHWIDTH);
        int y2 = OFFSETGRAPH + GRAPHWIDTH - (float)((data[i + 1] / ymax) * GRAPHWIDTH);
        if (SDL_RenderDrawLine(renderer, x1, y1, x2, y2) != 0)
        {
            SDL_ExitWithError("Erreur Draw Line");
            exit(EXIT_FAILURE);
        }
    }

    if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255) != 0)
    {
        SDL_ExitWithError("Erreur changement de couleur");
        exit(EXIT_FAILURE);
    }
}

float xaucube(float x)
{
    return x * x * x;
}

void handlegraphMonthComp(Maison *unemaison, int len, float xmax, float ymax, float rez, int xlegendDisplayPeriod, int ylegendDisplayPeriod)
{
    int i = 1;
    char legendx[CHARLEN];
    char legendy[CHARLEN];
    sprintf(legendy, "Production en Wh");
    char legend[CHARLEN];
    char *stringtab[12] = {"janvier", "fevrier", "mars", "avril", "mai", "juin", "juillet", "aout", "septembre", "octobre", "novembre", "decembre"};
    sprintf(legend, "Production en wh pour les mois de %s %d a %d", stringtab[i - 1], STARTINGYEAR, ENDINGYEAR);
    sprintf(legendx, "mois de %s", stringtab[i - 1]);
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Renderer *renderertext = NULL;
    SDL_Surface *icon = IMG_Load("icon.png");
    Bool windowjustchanged = False;

    //Lancement SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        SDL_ExitWithError("Initialisation SDL");
    TTF_Init();
    IMG_Init(IMG_INIT_JPG);

    //Création fenêtre
    if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer) != 0)
        SDL_ExitWithError("Creation fenetre et rendu echouee");

    SDL_SetWindowTitle(window, "A simple Graph");
    SDL_SetWindowIcon(window, icon);

    float *tab = extractMonthComparaisonAcrossYears(i, MONTHLYDATA, unemaison);
    drawGraphicBase(renderer, legend, xmax, ymax, legendx, legendy, rez, xlegendDisplayPeriod, ylegendDisplayPeriod, 2005);
    drawgraphfromTab(renderer, tab, len, rez, ymax);
    SDL_RenderPresent(renderer);

    SDL_RenderPresent(renderer);

    /*---------------------------  Boucle SDL   ---------------------------------*/

    SDL_bool program_launched = SDL_TRUE;
    while (program_launched)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {

            case SDL_QUIT:
                program_launched = SDL_FALSE;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_LEFT:
                    i--;
                    if (i < 1)
                        i += 12;
                    sprintf(legend, "Production en wh pour les mois de %s %d a %d", stringtab[i - 1], STARTINGYEAR, ENDINGYEAR);
                    tab = extractMonthComparaisonAcrossYears(i, MONTHLYDATA, unemaison);
                    sprintf(legendx, "mois de %s", stringtab[i - 1]);
                    windowjustchanged = 1;
                    break;
                case SDLK_RIGHT:
                    i++;
                    if (i > 12)
                        i -= 12;
                    sprintf(legend, "Production en wh pour les mois de %s %d a %d", stringtab[i - 1], STARTINGYEAR, ENDINGYEAR);
                    tab = extractMonthComparaisonAcrossYears(i, MONTHLYDATA, unemaison);
                    sprintf(legendx, "mois de %s", stringtab[i - 1]);
                    windowjustchanged = 1;
                    break;

                default:
                    break;
                }
            default:
                break;
            }
        }
        if (windowjustchanged == True)
        {
            drawGraphicBase(renderer, legend, xmax, ymax, legendx, legendy, rez, xlegendDisplayPeriod, ylegendDisplayPeriod, 2005);
            drawgraphfromTab(renderer, tab, len, rez, ymax);
            windowjustchanged = False;
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(20);
    }
    /*------------------------------------------------------------*/

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_FreeSurface(icon);

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    free(tab);
}

void handlegraphYear(Maison *unemaison, int len, float xmax, float ymax, float rez, int xlegendDisplayPeriod, int ylegendDisplayPeriod)
{
    int i = STARTINGYEAR;
    char legendx[CHARLEN];
    sprintf(legendx, "Mois de l'annee %d", STARTINGYEAR);
    char legendy[CHARLEN];
    sprintf(legendy, "Production en Wh");
    char legend[CHARLEN];
    sprintf(legend, "Production en Wh pour l'annee %d", STARTINGYEAR);

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Renderer *renderertext = NULL;

    SDL_Surface *icon = IMG_Load("icon.png");
    Bool windowjustchanged = False;

    //Lancement SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        SDL_ExitWithError("Initialisation SDL");
    TTF_Init();
    IMG_Init(IMG_INIT_JPG);

    //Création fenêtre
    if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer) != 0)
        SDL_ExitWithError("Creation fenetre et rendu echouee");

    SDL_SetWindowTitle(window, "A simple Graph");
    SDL_SetWindowIcon(window, icon);

    float *tab = extractMonthInYearComparaison(i, MONTHLYDATA, unemaison);
    drawGraphicBase(renderer, legend, xmax, ymax, legendx, legendy, rez, xlegendDisplayPeriod, ylegendDisplayPeriod, 1);
    drawgraphfromTab(renderer, tab, len, rez, ymax);
    SDL_RenderPresent(renderer);

    SDL_RenderPresent(renderer);

    /*---------------------------  Boucle SDL   ---------------------------------*/

    SDL_bool program_launched = SDL_TRUE;
    while (program_launched)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {

            switch (event.type)
            {
            case SDL_QUIT:
                program_launched = SDL_FALSE;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_RIGHT:
                    i++;
                    if (i > ENDINGYEAR)
                        i -= ENDINGYEAR - STARTINGYEAR + 1;
                    tab = extractMonthInYearComparaison(i, MONTHLYDATA, unemaison);
                    sprintf(legendx, "Mois de l'annee %d", i);
                    sprintf(legend, "Production en Wh pour l'annee %d", i);
                    windowjustchanged = True;
                    break;
                case SDLK_LEFT:
                    i--;
                    if (i < STARTINGYEAR)
                        i += ENDINGYEAR - STARTINGYEAR + 1;
                    tab = extractMonthInYearComparaison(i, MONTHLYDATA, unemaison);
                    sprintf(legendx, "Mois de l'annee %d", i);
                    sprintf(legend, "Production en Wh pour l'annee %d", i);
                    windowjustchanged = True;
                    break;
                default:
                    break;
                }

            default:
                break;
            }
        }
        if (windowjustchanged == True)
        {
            drawGraphicBase(renderer, legend, xmax, ymax, legendx, legendy, rez, xlegendDisplayPeriod, ylegendDisplayPeriod, 1);
            drawgraphfromTab(renderer, tab, len, rez, ymax);
            windowjustchanged = False;
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(20);
    }
    /*------------------------------------------------------------*/

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_FreeSurface(icon);

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    free(tab);
}

void handlegraphDay(Maison *unemaison, int len, float xmax, float ymax, float rez, int xlegendDisplayPeriod, int ylegendDisplayPeriod)
{
    char *stringtab[12] = {"janvier", "fevrier", "mars", "avril", "mai", "juin", "juillet", "aout", "septembre", "octobre", "novembre", "decembre"};
    float mytab[24];
    for (int k = 0; k < 24; k++)
    {
        mytab[k] = consommation_WHPerDay(unemaison) / 24;
    }
    int i = 0;
    char legendx[CHARLEN];
    sprintf(legendx, "heures");
    char legendy[CHARLEN];
    sprintf(legendy, "Production en W");
    char legend[CHARLEN];
    sprintf(legend, "journee typique du mois de %s", stringtab[i]);
    char filename[CHARLEN];
    sprintf(filename, "Dailydata/Dailydata_Gardanne_%d.csv", i + 1);

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Renderer *renderertext = NULL;

    SDL_Surface *icon = IMG_Load("icon.png");
    Bool windowjustchanged = False;

    //Lancement SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        SDL_ExitWithError("Initialisation SDL");
    TTF_Init();
    IMG_Init(IMG_INIT_JPG);

    //Création fenêtre
    if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer) != 0)
        SDL_ExitWithError("Creation fenetre et rendu echouee");

    SDL_SetWindowTitle(window, "A simple Graph");
    SDL_SetWindowIcon(window, icon);

    float *tab = extractFromDailyData(filename);

    for (int k = 0; k < 24; k++)
    {
        tab[k] = tab[k] * unemaison->nbpanneau * PANNEAUSURFACE;
    }

    drawGraphicBase(renderer, legend, xmax, ymax, legendx, legendy, rez, xlegendDisplayPeriod, ylegendDisplayPeriod, 0);
    drawgraphfromTab(renderer, tab, len, rez, ymax);
    drawgraphfromTabRed(renderer, mytab, len, rez, ymax);
    SDL_RenderPresent(renderer);

    SDL_RenderPresent(renderer);

    /*---------------------------  Boucle SDL   ---------------------------------*/

    SDL_bool program_launched = SDL_TRUE;
    while (program_launched)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {

            switch (event.type)
            {
            case SDL_QUIT:
                program_launched = SDL_FALSE;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_RIGHT:
                    i++;
                    if (i > 11)
                        i -= 12;
                    sprintf(legend, "journee typique du mois de %s", stringtab[i]);
                    sprintf(filename, "Dailydata/Dailydata_Gardanne_%d.csv", i + 1);
                    tab = extractFromDailyData(filename);
                    for (int k = 0; k < 24; k++)
                    {
                        tab[k] = tab[k] * unemaison->nbpanneau * PANNEAUSURFACE;
                    }
                    windowjustchanged = True;
                    break;
                case SDLK_LEFT:
                    i--;
                    if (i < 0)
                        i += 12;
                    sprintf(legend, "journee typique du mois de %s", stringtab[i]);
                    sprintf(filename, "Dailydata/Dailydata_Gardanne_%d.csv", i + 1);
                    tab = extractFromDailyData(filename);
                    for (int k = 0; k < 24; k++)
                    {
                        tab[k] = tab[k] * unemaison->nbpanneau * PANNEAUSURFACE;
                    }
                    windowjustchanged = True;
                    break;
                default:
                    break;
                }

            default:
                break;
            }
        }
        if (windowjustchanged == True)
        {
            drawGraphicBase(renderer, legend, xmax, ymax, legendx, legendy, rez, xlegendDisplayPeriod, ylegendDisplayPeriod, 1);
            drawgraphfromTab(renderer, tab, len, rez, ymax);
            drawgraphfromTabRed(renderer, mytab, len, rez, ymax);
            windowjustchanged = False;
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(20);
    }
    /*------------------------------------------------------------*/

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_FreeSurface(icon);

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    free(tab);
}

void drawgraphfromTabRed(SDL_Renderer *renderer, float *data, int nbdata, float increment, float ymax)
{
    if (SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255) != 0)
    {
        SDL_ExitWithError("Erreur changement de couleur");
        exit(EXIT_FAILURE);
    }

    int incrementspat = GRAPHWIDTH / nbdata;

    for (int i = 0; i < nbdata - 1; i++)
    {
        int x1 = OFFSETGRAPH + (float)((i * incrementspat));
        int x2 = OFFSETGRAPH + (float)(((i + 1) * incrementspat));
        int y1 = OFFSETGRAPH + GRAPHWIDTH - (float)((data[i] / ymax) * GRAPHWIDTH);
        int y2 = OFFSETGRAPH + GRAPHWIDTH - (float)((data[i + 1] / ymax) * GRAPHWIDTH);
        if (SDL_RenderDrawLine(renderer, x1, y1, x2, y2) != 0)
        {
            SDL_ExitWithError("Erreur Draw Line");
            exit(EXIT_FAILURE);
        }
    }

    if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255) != 0)
    {
        SDL_ExitWithError("Erreur changement de couleur");
        exit(EXIT_FAILURE);
    }
}