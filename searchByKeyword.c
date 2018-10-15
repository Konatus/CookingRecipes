#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "include/mysql.h"
#include "configuration.h"

// Recupere les recetes à partir d'un mot clé
void searchByKeyword(MYSQL* mysql){
    system("cls");
    printf("Veuillez saisir un mot que vous recherchez :\n");
    char mot[80];
    scanf("%s", mot);


    char requete[1000];
    strcpy(requete, "SELECT * FROM recipes WHERE name LIKE '%");
    strcat(requete, mot);
    strcat(requete, "%'");


    if (mysql_query(mysql, requete)) {
      printf("Query failed: %s\n", mysql_error(mysql));
    } else
    {

    MYSQL_RES * result = mysql_store_result(mysql);

      if (!result) {
        printf("Couldn't get results set: %s\n", mysql_error(mysql));
      } else {
        MYSQL_ROW row;
        int i;
        unsigned int num_fields = mysql_num_fields(result);
        int cpt = 0;

        printf("\n\n-----------------------------\n");
        while ((row = mysql_fetch_row(result))) {
          printf(" %s (ID = %s)", row[1], row[0]);
          printf("\n\n-----------------------------\n\n");
          cpt += 1;
        }

        printf("Il y a %d recette(s) qui contiennent le mot ou l'expression \"%s\".\n\n", cpt, mot);

        mysql_free_result(result);
      }
    }
}
