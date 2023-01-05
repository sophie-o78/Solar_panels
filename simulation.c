#include "simulation.h"
#include <stdlib.h>
#include <stdio.h>
#include "GraphicsSDL.h"
#include "string.h"

float cout_installation(Maison *unemaison)
{
    return 2.500 * WCPerSolarPannel * unemaison->nbpanneau; //l'installation coute 2500 euros par Watt crete
}

float consommation_kW_max(Maison *unemaison)
{
    Equipement *temp = unemaison->first;
    float consommation = 0;
    while (temp != NULL)
    {
        consommation = consommation + temp->puissance;
        temp = temp->next;
    }
    return consommation;
}

float consommation_WHPerDay(Maison *unemaison)
{
    Equipement *temp = unemaison->first;
    float consommation = 0;
    while (temp != NULL)
    {
        consommation = consommation + (temp->puissance * temp->dailyHourUseDuration);
        temp = temp->next;
    }
    return consommation;
}

float consommationKWHPerYear(Maison *unemaison)
{
    return 365.25 * consommation_WHPerDay(unemaison);
}

float priceOfElectricityPerYear(Maison *unemaison)
{
    return consommationKWHPerYear(unemaison) * (ELECTRICITY_PRICE / 1000);
}

int MaxNumberOfSolarPanel(Maison *unemaison)
{
    return unemaison->surface / PANNEAUSURFACE;
}

int duree_rentabilite(Maison *unemaison)
{
    float consopricePerDay = (ELECTRICITY_PRICE / 1000) * consommation_WHPerDay(unemaison);
    float installationcost = cout_installation(unemaison);
    return 1 + (installationcost / consopricePerDay);
}

int nb_panneaux_necessaires(Maison *unemainson)
{
    float consomoy;
    consomoy = consommation_WHPerDay(unemainson) / 24;
    return 1 + (consomoy / WCPerSolarPannel);
}

int nb_panneaux_necessaires_conso_max(Maison *unemaison)
{
    int n = 1 + (consommation_kW_max(unemaison) / (WCPerSolarPannel));
    if (n > MaxNumberOfSolarPanel(unemaison))
    {
        printf("Votre maison n'a pas la capacite d'accueillir plus de %d panneaux\n", MaxNumberOfSolarPanel(unemaison));
    }
    return n;
}
float surplusjournalier(float *tab, Maison *unemaison)
{
    float returnedvalue = 0;
    float puissanceInstantAVG = consommation_WHPerDay(unemaison) / 24;

    for (int i = 0; i < 24; i++)
    {
        float instantprod = tab[i] * unemaison->nbpanneau * PANNEAUSURFACE;
        if (instantprod > puissanceInstantAVG)
        {
            returnedvalue += instantprod - puissanceInstantAVG;
        }
    }
    return returnedvalue;
}

float deficitsjournaliers(float *tab, Maison *unemaison)
{
    float returnedvalue = 0;
    float puissanceInstantAVG = consommation_WHPerDay(unemaison) / 24;

    for (int i = 0; i < 24; i++)
    {
        float instantprod = tab[i] * unemaison->nbpanneau * PANNEAUSURFACE;
        if (instantprod < puissanceInstantAVG)
        {
            returnedvalue += puissanceInstantAVG - instantprod;
        }
    }
    return returnedvalue;
}

void menu(HashTable *aHashTable)
{
    Bool quitter = False;
    int choix = 1;
    Maison *unemaison = newmaison(0);
    Equipement *unequipement = NULL;
    char string[CHARLEN];
    char string2[CHARLEN];
    float afloat;
    int anint;

    printf("1- Ajouter les equipements (et leur temps d'utilisation) a la maison (case sensitive) \n");
    printf("2- Indiquer la surface exploitable pour les panneaux solaires et connaitre le nombre max de panneaux instalables \n");
    printf("3- Connaitre le nombre de panneaux pour sastisfaire la consommation moyenne de la maison \n");
    printf("4- Connaitre le nombre de panneaux pour sastisfaire la consommation instantannée maximale de la maison \n");
    printf("5- Specifier le nombre de panneaux a installer \n");
    printf("6- Obtenir les graphs (comparaison d'un meme mois au cours des annees 2005-2016) de la production en Wh moyenne sur une heure  de la maison avec le nombre de panneaux specifies  \n");
    printf("7- Obtenir les graphs (comparaison des mois d'une meme annee) de la production en Wh moyenne sur une heure de la maison avec le nombre de panneaux specifies  \n");
    printf("8- Duree de rentabilite de l'exploitation par energie solaire (en supposant que les besoins de la maison sont assurees)  \n");
    printf("9- Obtenir la consommation journaliere de la maison \n");
    printf("10- Puissance moyenne requise par la maison \n");
    printf("11- Prix de la facture d'electricite a l'annee sans panneau solaire \n");
    printf("12- Connaitre les surplus totaux (heure/heure) en energie d'une journee typique d'un mois a specifier \n");
    printf("13- Connaitre les deficits totaux (heure/heure) en energie d'une journee typique d'un mois a specifier \n");
    printf("14- Graph production en Wh de l'installation (+comparaison avec conso Moyenne) au cours d'une journee typique d'un mois a specifier \n");
    printf("15- Ajouter un equipement au catalogue ( ! conflits de nom non geres ! ) \n");
    printf("16- Aider l'utilisateur a gerer efficacement ses besoins energetiques\n");

    while (!quitter)
    {
        printf("\n");
        printf("Mettez le numero de l'operation a effectuer\n");

        scanf("%d", &choix);

        switch (choix)
        {
        case 1:
            printf("Saisir les equipements et le temps de consommation (en h ) presents ou ''0 0'' pour arreter la selection \n");
            scanf("%s %f", string, &afloat);
            unequipement = searchEquipementFromHash(aHashTable, string);
            while (unequipement != NULL)
            {
                unequipement = copyEquipementWithoutNext(unequipement);
                unequipement->dailyHourUseDuration = afloat;
                ajouterEquipement(unemaison, unequipement);
                scanf("%s %f", string, &afloat);
                unequipement = searchEquipementFromHash(aHashTable, string);
            }

            break;
        case 2:
            printf("Saisir la surface exploitable de la maison \n");
            scanf("%f", &afloat);
            unemaison->surface = afloat;
            printf("Le nombre maximal de panneaux que peut accueillir votre charmante demeure vaut %d :\n", MaxNumberOfSolarPanel(unemaison));
            break;
        case 3:
            printf("Votre consommation moyenne necessite %d panneaux solaires:\n", nb_panneaux_necessaires(unemaison));
            break;
        case 4:
            printf("Votre consommation maximale necessite %d panneaux solaires\n", nb_panneaux_necessaires_conso_max(unemaison));
            break;

        case 5:
            printf("Entrez le nombre de panneaux que vous souhaitez installer:\n");
            scanf("%d", &anint);
            unemaison->nbpanneau = anint;
            break;

        case 6:
            printf("Voici la production en kWh de l'installation prevue en fonction du temps pour le meme mois d'une annee a une autre\n");
            printf("Utilisez les fleches directionelles <-- et --> pour changer de mois, fermez la fenetre pour continuer le programme \n");

            handlegraphMonthComp(unemaison, ENDINGYEAR - STARTINGYEAR + 1, ENDINGYEAR - STARTINGYEAR + 1, 240 * PANNEAUSURFACE * (unemaison->nbpanneau), 1, 2, 2);
            printf("Pour changer de mois, veuillez appuyer sur les fleches directionnelles -> <-\n");
            break;

        case 7:
            printf("Voici la production en kWh de l'installation prevue au cours d'une annee\n");
            printf("Utilisez les fleches directionelles <-- et --> pour changer d'annee, fermez la fenetre pour continuer le programme \n");
            handlegraphYear(unemaison, 12, 12, 240 * PANNEAUSURFACE * (unemaison->nbpanneau), 1, 2, 2);
            break;

        case 8:
            printf("Sachant que vous avez installe %d panneaux, l'installation sera rentable au bout de %d jours \n", unemaison->nbpanneau, duree_rentabilite(unemaison));
            break;

        case 9:
            printf("Votre consommation journaliere vaut %1.f WH \n", consommation_WHPerDay(unemaison));
            break;

        case 10:
            printf("Votre consommation instantanee moyenne vaut %1.f W \n", consommation_WHPerDay(unemaison) / 24);
            break;

        case 11:
            printf("Le prix de l'electricite sans panneau solaire par an pour votre consommation est de : %2.f euros \n", priceOfElectricityPerYear(unemaison));
            break;

        case 12:
            printf("Saisissez le numero du mois pour lequel vous voulez connaitre les surplus journaliers typiques\n");
            scanf("%d", &anint);
            sprintf(string, "Dailydata/Dailydata_Gardanne_%d.csv", anint);
            printf("La somme des surplus energetique d'une journee typique du mois %d est %1.f Wh \n", anint, surplusjournalier(extractFromDailyData(string), unemaison));
            break;
        case 13:
            printf("Saisissez le numero du mois pour lequel vous voulez connaitre les deficits journaliers typiques\n");
            scanf("%d", &anint);
            sprintf(string, "Dailydata/Dailydata_Gardanne_%d.csv", anint);
            printf("La somme des deficits energetiques d'une journee typique du mois %d est %1.f Wh \n", anint, deficitsjournaliers(extractFromDailyData(string), unemaison));
            break;

        case 14:
            printf("Voici la production journaliere en Wh de l'installation pour un mois, de 2005 jusque 2016\n");
            printf("Utilisez les fleches directionelles <-- et --> pour changer de mois, fermez la fenetre pour continuer le programme \n");
            handlegraphDay(unemaison, 24, 24, 1000 * PANNEAUSURFACE * (unemaison->nbpanneau), 1, 2, 4);
            break;
        case 15:
            printf("Saisir le type de l'equipement \n");
            scanf("%s", string);
            printf("Saisir le nom de l'equipement \n");
            scanf("%s", string2);
            printf("Saisir la puissance de l'equipement \n");
            scanf("%d", &anint);
            addequipementToCatalog(string, string2, anint);
            break;
        case 16:
            for (int i = 0; i < 12; i++)
            {
                sprintf(string, "Dailydata/Dailydata_Gardanne_%d.csv", i + 1);
                afloat = surplusjournalier(extractFromDailyData(string), unemaison) - deficitsjournaliers(extractFromDailyData(string), unemaison);
                if (afloat < 0)
                {
                    printf("il y a une difference de %2.f entre la production d'electricite des panneaux et vos besoins en electricite; il est avise de revoir vos besoins a la baisse\n", -afloat);
                    printf("\n");
                }
                if (afloat > 0)
                {
                    printf("il y a une difference de %2.f entre la production d'electricite des panneaux et vos besoins en electricite; vos besoins energetiques sont couverts\n", afloat);
                    printf("\n");
                }
            }

            break;
        case 17:
            quitter = True;
            freemaison(unemaison);
            break;
        default:
            break;
        }
    }
}
