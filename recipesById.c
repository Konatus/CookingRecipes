#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include "include/mysql.h"

void recipeById(MYSQL* mysql, int id){


        //D�claration des objets
        MYSQL_RES *result = NULL;
        MYSQL_ROW row;
        unsigned int i = 0;
        char requete[500] = "";

        //On stock la requete dans notre tableau de char
        sprintf(requete, "SELECT  recipes.name, quantity.amount, ingredients.description, ingredients.price FROM recipes INNER JOIN quantity ON recipes.id = quantity.recipe_id INNER JOIN ingredients ON quantity.ingredient_id =  ingredients.id AND recipes.id = '%d'", id);

        //On execute la requete
        mysql_query(mysql, requete);

        //On met le jeu de r�sultat dans le pointeur result
        result = mysql_use_result(mysql);

        if(result == NULL) {
            printf("Aucune recette n\'est associe a cet ID.\n");
            mysql_free_result(result);
            return;
        }

        //Tant qu'il y a encore un r�sultat ...
        while ((row = mysql_fetch_row(result))){

            if(i == 0) {
                printf("Pour faire la recette : %s, il faut :\n", row[0]);
            }
            printf("- %sx ", row[1]);
            printf("%s, ", row[2]);
            printf("prix unitaire : %s\n", row[3]);
            i += 1;
        }
        //Lib�ration du jeu de r�sultat
        mysql_free_result(result);


    return 1;
}
