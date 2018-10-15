#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include "include/mysql.h"
#include "configuration.h"

#include "menu.h"
#include "leftovers.h"
#include "searchByKeyword.h"

int main(int argc, char **argv)
{
  int go = 0;
  MYSQL *mysql = NULL;

  mysql = mysql_init(mysql);

  if (!mysql) {
    puts("Init faild, out of memory?");
    return EXIT_FAILURE;
  }

  if (!mysql_real_connect(mysql,       /* MYSQL structure to use */
			  MYSQL_HOST,         /* server hostname or IP address */
			  MYSQL_USER,         /* mysql user */
			  MYSQL_PWD,          /* password */
			  MYSQL_DB,               /* default database to use, NULL for none */
			  0,           /* port number, 0 for default */
			  NULL,        /* socket file or named pipe name */
			  CLIENT_FOUND_ROWS /* connection flags */ )) {
    puts("Echec de la connexion a la base de donnees...\nVeuillez relancer l\'application en verifiant les parametres.");
  } else {
    puts("Connect OK\n");
    do {
        go = menu(mysql);
    } while(go == 1);

  }

  mysql_close(mysql);

  return EXIT_SUCCESS;
}
