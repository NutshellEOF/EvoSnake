//
// Created by NutshellEOF on 2022/10/2.
//

#ifndef EVOSNAKE_UTIL_H
#define EVOSNAKE_UTIL_H

#define MAP_L 60
#define MAP_H 20

#define UP      (-MAP_L-2)
#define DOWN    (MAP_L+2)
#define LEFT    (-1)
#define RIGHT   1

typedef int direction;

typedef struct {
    int x;
    int y;
    int stat;
} MapBlock;

struct Snake {
    int dest;
    struct Snake *next;
};

#endif //EVOSNAKE_UTIL_H
