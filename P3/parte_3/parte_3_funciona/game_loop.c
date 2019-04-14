
/* ===================================================================
File: game_loop.c
Version: 3.0
Date: 1-10-2018
Autores: Saul Almazan y Alvaro Becerra Jiminez
Description: Ejecuta el main y el juego hasta que pongamos el comando exit
Revision historica:
13-01-2015 versión 1.0 (código inicial)
1-10-2018 versión 2.0
     Se pide el nombre del jugador
21-10-2018 versión 3.0
        Añadida la posibilidad de utilizar un fichero .log

=================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "graphic_engine.h"
#include "game.h"

int main(int argc, char *argv[]) {
    Game *game = NULL;
    Comand *comand = NULL;
    Graphic_engine *gengine;
    int flag = 0;
    STATUS log;
    extern char *cmd_to_str[];
    FILE *f = NULL;
    if (argc < 2) {
        fprintf(stderr, "Use: %s <game_data_file>\n", argv[0]);
        return 1;
    }
    if (argc == 4) {
        if (strcmp(argv[2], "-l") == 0) {
            f = fopen(argv[3], "w");
            if (f == NULL) {
                fprintf(stderr, "Error al abrir el archivo log\n");
                return 1;
            }
            flag = 1;
        }
    }
    srand(time(NULL));

    game = game_create_from_file(argv[1]);
    if (game == NULL) {
        fprintf(stderr, "Error while initializing game.\n");
        if (flag == 1) fclose(f);
        return 1;
    }
    /*creamos el motor grafico*/
    if ((gengine = graphic_engine_create()) == NULL) {
        fprintf(stderr, "Error while initializing graphic engine.\n");
        game_destroy(game);
        if (flag == 1) fclose(f);
        return 1;
    }
    /*creamos comando*/
    if ((comand = comand_create()) == NULL) {
        fprintf(stderr, "Error al crear comando.\n");
        game_destroy(game);
        graphic_engine_destroy(gengine);
        if (flag == 1) fclose(f);
        return 1;
    }
    /*jugamos hasta que haya un error o un exit*/
    while ((comand_get_comand(comand) != QUIT) && !game_is_over(game)) {
        graphic_engine_paint_game(gengine, game);
        if (comand_get_user_input(comand) == ERROR) {
            fprintf(stderr, "Error al leer comando.\n");
            game_destroy(game);
            graphic_engine_destroy(gengine);
            comand_destroy(comand);
            if (flag == 1) fclose(f);
            return 1;
        }
        log = game_update(game, comand);
        if (flag == 1) {
            if (log == OK) {
                if (strcmp(comand_get_name(comand), "") != 0) {
                    fprintf(f, "%s %s: OK\n", cmd_to_str[game_get_last_command(game) - NO_CMD], comand_get_name(comand));
                } else {
                    fprintf(f, "%s: OK\n", cmd_to_str[game_get_last_command(game) - NO_CMD]);
                }
            } else {
                if (strcmp(comand_get_name(comand), "") != 0) {
                    fprintf(f, "%s %s: ERROR\n", cmd_to_str[game_get_last_command(game) - NO_CMD], comand_get_name(comand));
                } else {
                    fprintf(f, "%s: ERROR\n", cmd_to_str[game_get_last_command(game) - NO_CMD]);
                }
            }
        }
    }
    /*liberamos la memoria*/
    game_destroy(game);
    graphic_engine_destroy(gengine);
    comand_destroy(comand);
    if (flag == 1) {
        fclose(f);
    }
    return 0;
}
