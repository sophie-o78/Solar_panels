#ifndef __FILESMANAGEMENT__H__
#define __FILESMANAGEMENT__H__

#define CATALOG "puissance_eletromenagers.csv"
#define MONTHLYDATA "Monthlydata_Gardanne (2).csv"
#define DAILYDATA "Dailydata_Gardanne.csv"
#define P 1.7 //surface moyenne d'un panneau
#define CHARLEN 100
#define __BIG__PRIME__INT__ 37987
#define MAXLENOVERFLOWHASHTABLE 9
#define STARTINGYEAR 2005
#define ENDINGYEAR 2016

typedef enum
{
    False,
    True
} Bool;

typedef struct _equipement
{
    char nom[CHARLEN];
    char type[CHARLEN];
    struct _equipement *next;
    float puissance;
    float dailyHourUseDuration;
} Equipement;

typedef struct
{
    int taille; //nombre d'équipements
    Equipement *first;
    Equipement *last;
    float surface;
    int nbpanneau;

} Maison;

Maison *newmaison(float surface);
Equipement *newequipement(char *name, char *type, float puissance);
Equipement *copyEquipementWithoutNext(Equipement *unEquipement);
void freemaison(Maison *unemaison);
void ajouterEquipement(Maison *uneMaison, Equipement *unEquipement);
void enleverEquipement(Maison *uneMaison, int n);
float *extractMonthInYearComparaison(int year, char *filename, Maison *unemaison);
float *extractMonthComparaisonAcrossYears(int month, char *filename, Maison *unemaison);
float *extractFromDailyData(char *filename);
float *extractDayComparaisonAcrossMonth(int month, char *filename, Maison *unemaison);

/* HASHTABLE */

typedef struct _hashTable
{
    unsigned int size;
    unsigned int nbOccupiedEntries;
    unsigned int nbElements;
    Equipement **Equipements;
} HashTable; //table de hachage qui peut servir pour le catalogue d'équipements

HashTable *newhashtable(unsigned int amaxtaille);
void freehashtable(HashTable *ahashtable);
unsigned long long ASCIItoInt(char *achar);
void insertEquipementInHahsTable(HashTable *ahashtable, Equipement *unEquipement);
HashTable *createHashTableFromTxt(char *filename);

Equipement *searchEquipementFromHash(HashTable *ahashtable, char *searchingstring);

void addequipementToCatalog(char *room, char *name, int puissance);

#endif