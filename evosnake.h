//
// Created by NutshellEOF on 2022/10/2.
//

#ifndef EVOSNAKE_EVOSNAKE_H
#define EVOSNAKE_EVOSNAKE_H

#include "util.h"

extern MapBlock *map;

void init();

void initMap();

void drawMap(int changeId, int pairId);

void gFood();

struct Snake **initSnake();

direction getd(direction pt);

int moveSnake(struct Snake **ptrs, int length, direction t);

void terminate(int length);

#endif //EVOSNAKE_EVOSNAKE_H