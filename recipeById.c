#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/mysql.h"


int recipeById(MYSQL* mysql, int id){

    if(id < 0){
        return -1;
    }else{
        //Déclaration des objets
        MYSQL_RES *result = NULL;
        MYSQL_ROW row;
        unsigned int i = 0;
        unsigned int num_champs = 0;
        char requete[500] = "";

        //On stock la requete dans notre tableau de char
        sprintf(requete, "SELECT  recipes.name, quantity.amount, ingredients.description, ingredients.price FROM recipes INNER JOIN quantity ON recipes.id = quantity.recipe_id INNER JOIN ingredients ON quantity.ingredient_id =  ingredients.id AND recipes.id = '%d'", id);

        //On execute la requete
        mysql_query(mysql, requete);

        //On met le jeu de résultat dans le pointeur result
        result = mysql_use_result(mysql);

        //On récupère le nombre de champs
        num_champs = mysql_num_fields(result);

        //Tant qu'il y a encore un résultat ...
        while ((row = mysql_fetch_row(result))){
            //On déclare un pointeur long non signé pour y stocker la taille des valeurs
            unsigned long *lengths;

            //On stocke ces tailles dans le pointeur
            lengths = mysql_fetch_lengths(result);

            //On fait une boucle pour avoir la valeur de chaque champs
            for(i = 0; i < num_champs; i++){
                //On ecrit toutes les valeurs
                switch(i){
                case 0:
                    printf("Pour faire la recette : %.*s", (int) lengths[i], row[i] ? row[i] : "NULL");
                    break;
                case 1:
                    printf(", il faut : %.*s ", (int) lengths[i], row[i] ? row[i] : "NULL");
                    break;
                case 2:
                    printf("%.*s ", (int) lengths[i], row[i] ? row[i] : "NULL");
                    break;
                case 3:
                    printf("%.*s ", (int) lengths[i], row[i] ? row[i] : "NULL");
                    break;
                case 4:
                    printf("qui coutent : %.*s ", (int) lengths[i], row[i] ? row[i] : "NULL");
                    break;
               }
            }
            printf("\n");
        }
        //Libération du jeu de résultat
        mysql_free_result(result);

        //Fermeture de MySQL
        mysql_close(&mysql);


    return 0;
}

