#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include "include/mysql.h"

int menu(MYSQL* mysql) {
    int choice = 0;
    int ipt = 0;
    system("cls");
    printf("*********************\n");
    printf("*                   *\n");
    printf("*  COOKING RECIPES  *\n");
    printf("*                   *\n");
    printf("*********************\n\n\n");

    printf( "1. Recherche de recette par ID\n"
            "2. Recherche de recette par mot cle\n"
            "3. Recherche de recette en fonction des ingredients saisis\n"
            "4. Quitter\n\n");

    do {
        printf("Votre choix : ");
        fflush(stdin);
        scanf("%d", &choice);
        if(choice < 1 || choice > 4) {
            printf("Veuillez saisir un nombre correct\n\n");
        }
    } while(choice < 1 || choice > 4);

    switch(choice) {
        case 1:
            do {
                printf("\nSaisir un id de recette a chercher : ");
                scanf("%d", &ipt);
            } while(ipt <= 0);
            recipeById(mysql, ipt);
            system("pause");
            break;
        case 2:
            searchByKeyword(mysql);
            system("pause");
            break;
        case 3:
            leftovers(mysql);
            break;
        case 4:
            return 0;
            break;
        default:
            break;
    }
    return 1;
}
