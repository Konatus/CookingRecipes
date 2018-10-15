#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include "include/mysql.h"

// Vérifie que l'ID saisi existe dans la base de données
int isIdOk(int *indiceIng, int size, int**selec, int sizeS, int x) {
    int i;
    for(i = 0; i < size; i++) {
        if(indiceIng[i] == x) {
            return 0;
        }
    }

    for(i = 0; i < sizeS; i++) {
        if(selec[i][0] == x) {
            return 1;
        }
    }
    return 1;
}

// Recupere les ingredients restants qui doivent composer une recette
void leftovers(MYSQL* mysql) {
    int choice = 0;
    int i;
    char *query;
    MYSQL_RES *result = NULL;
    MYSQL_ROW row;
    int nbIng = 0;
    int *indiceIng = NULL;
    char **ing = NULL;
    int **selection = NULL;
    int selectionSize;

    query = "SELECT * FROM ingredients";

    mysql_query(mysql, query);

    result = mysql_store_result(mysql);

    // Calcul du nombre d'ingredients dans la table ingredients
    if(result != NULL) {
        while(row = mysql_fetch_row(result)) {
            nbIng += 1;
        }
    } else {
        printf("ERROR No values in ingredient tab");
    }

    // Création des tableaux pour récuperer les ingredients
    indiceIng = malloc(sizeof(int) * nbIng);
    // ing : nom des ingredients
    ing = malloc(sizeof(char*) * nbIng);
    for(i = 0; i < nbIng; i++) {
        ing[i] = malloc(sizeof(char) * 100);
    }

    // Affectation des ingréidents dans les tableaux
    for(i = 0; i < nbIng; i++) {
        mysql_data_seek(result, i);
        row = mysql_fetch_row(result);
        indiceIng[i] = *row[0] - '0';
        strcpy(ing[i], row[1]);
        ing[i][strlen(ing[i])] = '\0';
    }

    mysql_free_result(result);

    system("cls");
    printf("*********************\n");
    printf("*                   *\n");
    printf("*     LEFTOVERS     *\n");
    printf("*                   *\n");
    printf("*********************\n\n\n");

    printf("Liste des ingredients : \n\n");

    for(i = 0; i < nbIng; i++) {
        printf("%d - %s\n", indiceIng[i], ing[i]);
    }

    // Saisie du nombre ingredients a rechercher
    printf("\nEntrer le nombre d'ingredients desire, puis l\'ID de l\'ingredient et sa quantite.");
    do {
        printf("\nNombre d\'ingredients : ");
        scanf("%d", &selectionSize);
    } while(selectionSize <= 0);


    selection = malloc(sizeof(int*) * selectionSize);
    printf("\nSelection des ingredients (ex : 1 2)\n");
    for(i = 0; i < selectionSize; i++) {
        int a;
        int b;
        selection[i] = malloc(sizeof(int) * 2);
        do {
            printf("\nIngredient %d : ", i + 1);
            fflush(stdin);
            scanf("%d %d", &a, &b);
        } while(isIdOk(indiceIng, nbIng, selection, i, a) || b <= 0);
        selection[i][0] = a;
        selection[i][1] = b;
    }

    loadRecipes(selection, selectionSize, mysql);
    system("pause");
}

// Charge toutes les recettes
void loadRecipes(int** tab, int size, MYSQL* mysql) {
    char *query = "SELECT * FROM recipes";
    int nbRec = 0;
    MYSQL_RES *result = NULL;
    MYSQL_ROW row;
    int *recipes = NULL;
    char **res = NULL;
    char **recName = NULL;
    int write = 0;
    int i;
    int ret;

    mysql_query(mysql, query);

    result = mysql_store_result(mysql);

    if(result != NULL) {
        while(row = mysql_fetch_row(result)) {
            nbRec += 1;
        }
    } else {
        printf("ERROR No values in recipes tab");
        return;
    }

    recipes = malloc(sizeof(int) * nbRec);
    recName = malloc(sizeof(char*) * nbRec);
    res = malloc(sizeof(char*) * nbRec);
    for(i = 0; i < nbRec; i++) {
        recName[i] = malloc(sizeof(char) * 100);
        res[i] = malloc(sizeof(char) * 100);
    }


    for(i = 0; i < nbRec; i++) {
        mysql_data_seek(result, i);
        row = mysql_fetch_row(result);
        recipes[i] = *row[0] - '0';
        strcpy(recName[i], row[1]);
        recName[i][strlen(recName[i])] = '\0';
    }

    mysql_free_result(result);

    // Appel de la fonction verifiant les ingredients pour chaque recette
    for(i = 0; i < nbRec; i++) {
        ret = checkIngRecipe(recipes[i], mysql, tab, size);
        if(ret != -1) {
            strcpy(res[write], recName[i]);
            res[i][strlen(res[i])] = '\0';
            write += 1;
        }
    }
    printRes(res, write);
}

// Recupere les ingredients d'une recette, puis les compare aux ingredients restants
int checkIngRecipe(int id_rec, MYSQL *mysql, int **ing, int size) {
    char query[100];
    sprintf(query, "SELECT ingredient_id, amount FROM quantity WHERE ingredient_id = %d", id_rec);
    int nbIng = 0;
    int **comp = NULL;
    MYSQL_RES *result = NULL;
    MYSQL_ROW row;
    int i;
    int ok;

    mysql_query(mysql, query);

    result = mysql_store_result(mysql);

    if(result != NULL) {
        while(row = mysql_fetch_row(result)) {
            nbIng += 1;
        }
    } else {
        return -1;
    }

    // Faux si il y a plus d'ingredients necessaires que ce que l'on a
    if(nbIng > size) {
        return -1;
    }

    // On stocke les ingredients de la recette à tester
    comp = malloc(sizeof(int*) * nbIng);
    for(i = 0; i < nbIng; i++) {
        comp[i] = malloc(sizeof(int) * 2);
    }

    for(i = 0; i < nbIng; i++) {
        mysql_data_seek(result, i);
        row = mysql_fetch_row(result);
        comp[i][0] = *row[0] - '0';
        comp[i][1] = *row[1] - '0';
    }

    // Pour chaque ingredient de la recette, verifier qu'il est dans la liste des ingredients restants
    for(i = 0; i < nbIng; i++) {
        ok = isIngOk(ing, size, comp[i]);
        if(ok == -1) {
            return -1;
        }
    }
    return ok;
}

// vérifie que l'ingrédient de la recette fait partie (en bonne quantité) dans les ingredients restants
int isIngOk(int** tab, int size, int* arr) {
    int i;
    int ok = -1;
    for(i = 0; i < size; i++) {
        if(tab[i][0] == arr[0]) {
            if(tab[i][1] >= arr[1]) {
                ok = 1;
                break;
            } else {
                // Break car il n'y a pas de doublon dans les ingredients restants
                break;
            }
        }
    }

    return ok;
}

// Afficher le résultat
void printRes(char** res, int size) {
    int i;
    if(size == 0) {
        printf("Desole, aucune recette n\'est faisable avec vos ingredients...\n");

    } else {
        printf("Voici les recettes realisables avec vos ingredients :");
        for(i = 0; i < size; i++) {
            printf("- %s\n", res[i]);
        }
    }
    return;
}
