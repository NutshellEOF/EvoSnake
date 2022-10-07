//
// Created by NutshellEOF on 2022/10/2.
//

#ifndef EVOSNAKE_EVOSNAKE_H
#define EVOSNAKE_EVOSNAKE_H

#endif //EVOSNAKE_EVOSNAKE_H

#include "util.h"

void init();

MapBlock *initMap(MapBlock *map);

void drawMap(MapBlock *map, int changeId);

void gFood(MapBlock *map);

struct Snake **initSnake(MapBlock *map);

direction getd(direction pt);

int moveSnake(MapBlock *map, struct Snake **ptrs, int length, direction t);

void terminate(int code, int score);