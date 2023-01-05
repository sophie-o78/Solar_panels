#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "filesmanagement.h"
#include "simulation.h"

Maison *newmaison(float surface)
{
    Maison *uneMaison = malloc(sizeof(Maison));
    uneMaison->first = NULL;
    uneMaison->last = NULL;
    uneMaison->taille = 0;
    uneMaison->surface = surface;
    uneMaison->nbpanneau = 0;
    return uneMaison;
}

Equipement *newequipement(char *name, char *type, float puissance)
{
    Equipement *unEquipement = malloc(sizeof(Equipement));
    unEquipement->puissance = puissance;
    strcpy(unEquipement->nom, name);
    strcpy(unEquipement->type, type);
    unEquipement->next = NULL;
    unEquipement->dailyHourUseDuration = 0;
    return unEquipement;
}

Equipement *copyEquipementWithoutNext(Equipement *unEquipement)
{
    Equipement *copiedEquipement = newequipement(unEquipement->nom, unEquipement->type, unEquipement->puissance);
    return copiedEquipement;
}

void freemaison(Maison *unemaison)
{
    while (unemaison->taille > 0)
    {
        enleverEquipement(unemaison, 0);
    }
}
void ajouterEquipement(Maison *uneMaison, Equipement *unEquipement)
{
    if (uneMaison->taille == 0)
    {
        uneMaison->first = unEquipement;
        uneMaison->last = unEquipement;
        uneMaison->taille++;
    }
    else
    {
        uneMaison->last->next = unEquipement;
        uneMaison->last = unEquipement;
        uneMaison->taille++;
    }
}

void enleverEquipement(Maison *uneMaison, int n)
{
    Equipement *equipementIterator = uneMaison->first;
    if (n == 0)
    {
        uneMaison->first = uneMaison->first->next;
        if (equipementIterator != NULL)
        {
            free(equipementIterator);
            equipementIterator = NULL;
        }
        if (uneMaison->taille > 0)
            uneMaison->taille--;
    }
    else
    {
        for (int i = 0; i < n - 1; i++)
        {
            equipementIterator = equipementIterator->next;
        }
        Equipement *tofree = equipementIterator->next;
        equipementIterator->next = tofree->next;
        if (n - 1 == uneMaison->taille)
        {
            uneMaison->last = equipementIterator;
        }
        if (tofree != NULL)
        {
            free(tofree);
            tofree = NULL;
        }
        if (uneMaison->taille > 0)
            uneMaison->taille--;
    }
}

/*                 HASHTABLE                     */

HashTable *newhashtable(unsigned int amaxtaille)
{
    HashTable *aHashtable = malloc(sizeof(HashTable));
    aHashtable->size = amaxtaille;
    aHashtable->nbOccupiedEntries = 0;
    aHashtable->nbElements = 0;
    aHashtable->Equipements = calloc(amaxtaille, sizeof(HashTable *));
    return aHashtable;
}
void freehashtable(HashTable *ahashtable)
{
    for (int i = 0; i < ahashtable->size; i++)
    {
        Equipement *ahashtableiterator = ahashtable->Equipements[i];
        while (ahashtableiterator != NULL)
        {
            Equipement *temp = ahashtableiterator;
            ahashtableiterator = ahashtableiterator->next;
            if (temp != NULL)
            {
                free(temp);
                temp = NULL;
            }
        }
    }
    if (ahashtable->Equipements != NULL)
    {
        free(ahashtable->Equipements);
        ahashtable->Equipements = NULL;
    }
}
unsigned long long ASCIItoInt(char *achar)
{
    unsigned long long n = 0;
    char mycara = 0;

    for (unsigned int i = 0; i < strlen(achar) && i < MAXLENOVERFLOWHASHTABLE; i++)
    {
        mycara = achar[i];

        n += mycara * pow(128, i);
    }
    return n;
}

;
void insertEquipementInHahsTable(HashTable *ahashtable, Equipement *unEquipement)
{
    unsigned long long key = ASCIItoInt(unEquipement->nom);
    key = key % ahashtable->size;
    Equipement *ahashtableiterator = ahashtable->Equipements[key];
    if (ahashtableiterator == NULL)
    {
        ahashtable->Equipements[key] = unEquipement;
        ahashtable->nbOccupiedEntries++;
        ahashtable->nbElements++;
    }
    else
    {
        while (ahashtableiterator->next != NULL)
        {
            ahashtableiterator = ahashtableiterator->next;
        }
        ahashtableiterator->next = unEquipement;
        ahashtable->nbElements++;
    }
}

HashTable *createHashTableFromTxt(char *filename)
{

    HashTable *myhashtable = newhashtable(__BIG__PRIME__INT__);
    FILE *fichier = NULL;
    char tempstringname[CHARLEN];
    char tempstringtype[CHARLEN];
    float tempconso;
    fichier = fopen(filename, "r");
    if (fichier == NULL)
    {
        printf("fichier non ouvert correctement \n");
        perror(filename);
        exit(EXIT_FAILURE);
    }
    while (fscanf(fichier, "%[^,], %[^,], %f\n", tempstringtype, tempstringname, &tempconso) == 3)
    {
        Equipement *unEquipement = newequipement(tempstringname, tempstringtype, tempconso);
        insertEquipementInHahsTable(myhashtable, unEquipement);
    }
    fclose(fichier);
    return myhashtable;
}

Equipement *searchEquipementFromHash(HashTable *ahashtable, char *searchingstring)
{
    unsigned long long n = ASCIItoInt(searchingstring);
    n = n % ahashtable->size;
    Equipement *equipementIterator = ahashtable->Equipements[n];
    Equipement *retainedequipement = NULL;
    while (equipementIterator != NULL)
    {
        if (strcmp(equipementIterator->nom, searchingstring) == 0)
        {
            retainedequipement = equipementIterator;
        }
        equipementIterator = equipementIterator->next;
    }
    return retainedequipement;
}

float *extractMonthInYearComparaison(int year, char *filename, Maison *unemaison)
{
    float *tab = calloc(12, sizeof(float));
    FILE *fichier = fopen(filename, "r");
    char ignoredlines[100];
    if (fichier == NULL)
    {
        printf("fichier non ouvert correctement \n");
        perror(filename);
        exit(EXIT_FAILURE);
    }
    int skippedLines = 6 + 12 * (year - 2005);
    for (int i = 0; i < skippedLines; i++)
    {
        fgets(ignoredlines, 100, fichier);
    }
    int ayear;
    char Monthname[4];
    float irradiation;
    float temp;
    int i = 0;
    while (fscanf(fichier, "%d  %s  %f  %f\n", &ayear, Monthname, &irradiation, &temp) == 4 && i < 12)
    {
        tab[i] = irradiation * (unemaison->nbpanneau) * PANNEAUSURFACE;
        i++;
    }
    fclose(fichier);
    return tab;
}
float *extractMonthComparaisonAcrossYears(int month, char *filename, Maison *unemaison)
{
    float *tab = calloc(ENDINGYEAR - STARTINGYEAR + 1, sizeof(float));
    FILE *fichier = fopen(filename, "r");
    char ignoredlines[100];
    if (fichier == NULL)
    {
        printf("fichier non ouvert correctement \n");
        perror(filename);
        exit(EXIT_FAILURE);
    }
    int skippedLines = 6 + month - 1;
    for (int i = 0; i < skippedLines; i++)
    {
        fgets(ignoredlines, 100, fichier);
    }
    int year;
    char Monthname[4];
    float irradiation;
    float temp;
    int i = 0;
    while (fscanf(fichier, "%d  %s  %f", &year, Monthname, &irradiation, &temp) == 3 && i < ENDINGYEAR - STARTINGYEAR + 1)
    {
        tab[i] = irradiation * (unemaison->nbpanneau) * PANNEAUSURFACE;
        i++;
        for (int k = 0; k < 12; k++)
        {
            fgets(ignoredlines, 100, fichier);
        }
    }

    fclose(fichier);
    return tab;
}

float *extractFromDailyData(char *filename)
{
    float *tab = calloc(24, sizeof(float));
    FILE *fichier = fopen(filename, "r");
    char ignoredlines[100];
    if (fichier == NULL)
    {
        printf("fichier non ouvert correctement \n");
        perror(filename);
        exit(EXIT_FAILURE);
    }
    int skippedLines = 8;
    for (int i = 0; i < skippedLines; i++)
    {
        fgets(ignoredlines, 100, fichier);
    }

    char mychar1[CHARLEN];
    float float1;
    float float2;
    float float3;

    int k = 0;
    while (fscanf(fichier, "%s %f %f %f\n", mychar1, &float1, &float2, &float3) == 4)
    {

        tab[k++] = float1;
    }
    return tab;
}

void addequipementToCatalog(char *room, char *name, int puissance)
{
    FILE *fichier = fopen(CATALOG, "a+");
    if (fichier == NULL)
    {
        printf("fichier non ouvert correctement \n");
        perror(CATALOG);
        exit(EXIT_FAILURE);
    }
    fprintf(fichier, "%s, %s, %d\n", room, name, puissance);
}