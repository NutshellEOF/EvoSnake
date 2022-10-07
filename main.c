//
// Created by NutshellEOF on 2022/10/2.
//

#include <curses.h>
#include <stdlib.h>
#include "util.h"
#include "evosnake.h"

/*
Compile: gcc *.c -lncurses
*/
int main()
{
    init();
    //初始化地图
    MapBlock *map = (MapBlock *) malloc(sizeof(MapBlock)*(MAP_L+2)*(MAP_H+2));
    int length = 2;
    struct Snake **ptrs = initSnake(map);
    initMap(map);
    direction t =LEFT;
    gFood(map);
    int score=0;
    while (length!=0) {
        t=getd(t);
        score=(length-2)*10;
        length = moveSnake(map,ptrs,length,t);
        mvaddch(MAP_H+1,MAP_L+1,'#');
    }
    terminate(length,score);
    getchar();
    endwin(); /* 结束curses 模式*/
    return 0;
}
