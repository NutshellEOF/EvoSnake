//
// Created by NutshellEOF on 2022/10/2.
//

#ifndef EVOSNAKE_EVOSNAKE_H
#define EVOSNAKE_EVOSNAKE_H

#include "util.h"
#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

extern MapBlock *map;

void init();

void initMap();

void drawMap(int changeId, int pairId);

void gFood();

void initSnake();

int getd(int pt);

int moveSnake(int length, int t);

void terminate(int length);

#endif //EVOSNAKE_EVOSNAKE_H