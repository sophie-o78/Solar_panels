#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "GraphicsSDL.h"
#include "simulation.h"

/*******************************************************************************************************/

void printabfloat(float *tab, int n) //Fonction de Test
{
    printf("[");
    for (int i = 0; i < n; i++)
    {
        printf(" %.2f ", tab[i]);
    }
    printf("]\n");
}

int main(int argc, char *argv[]) //La SDL necessite les arguments argc et argv dans le main
{
    HashTable *mycoolHashtable = createHashTableFromTxt(CATALOG);
    menu(mycoolHashtable);
    freehashtable(mycoolHashtable);

    return EXIT_SUCCESS;
}

/*******************************************************************************************************/
