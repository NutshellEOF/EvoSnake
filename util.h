//
// Created by NutshellEOF on 2022/10/2.
//

#ifndef EVOSNAKE_UTIL_H
#define EVOSNAKE_UTIL_H

#define MAP_L 60
#define MAP_H 18

//Color Pair Setup
#define WALL_PAIR 1
#define SNAKE_PAIR 2
#define FOOD_PAIR 3
#define MSG_PAIR 4
#define DHEAD_PAIR 5

//这样设置是为了免判断直接确定位置
#define UP      (-MAP_L-2)
#define DOWN    (MAP_L+2)
#define LEFT    (-1)
#define RIGHT   1

#define START_Y getmaxy(stdscr)/2-MAP_H/2-1
#define START_X getmaxx(stdscr)/2-MAP_L/2-1

typedef int direction;

typedef struct {
    int x;
    int y;
    int stat;
} MapBlock;

//linktable for snake array
struct Snake {
    int dest;
    struct Snake *next;
    struct Snake *pre;
};

#endif //EVOSNAKE_UTIL_H
