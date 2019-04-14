
 /* ===================================================================
File: screen.h
Version: 1.0
Date: 11-01-2017
Autores: Profesores PPROG
Description: Contiene todos los prototipos de las funciones de screen.c
Revision historica: 
11-01-2017 versión 1.0 (Código inicial)
=================================================================== */
#ifndef __SCREEN__
#define __SCREEN__

#define SCREEN_MAX_STR 80

typedef struct _Area Area;

void screen_init();
void screen_destroy();
void screen_paint();
void screen_gets(char *str);

Area* screen_area_init(int x, int y, int width, int height);
void screen_area_destroy(Area* area);
void screen_area_clear(Area* area);
void screen_area_reset_cursor(Area* area);
void screen_area_puts(Area* area, char *str);

#endif
