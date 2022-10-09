//
// Created by NutshellEOF on 2022/10/2.
//

#include <curses.h>
#include "util.h"
#include "evosnake.h"

/*
Compile: gcc *.c -lncurses
*/
int main()
{
    init();
    int length = 2;
    struct Snake **ptrs = initSnake();
    initMap();
    direction t =LEFT;
    gFood();
    int score=0;
    while (length!=0) {
        t=getd(t);
        score=(length-2)*10;
        length = moveSnake(ptrs,length,t);
        mvaddch(MAP_H+1,MAP_L+2,' ');
    }
    terminate(length,score);
    getchar();
    endwin(); /* 结束curses 模式*/
    return 0;
}
