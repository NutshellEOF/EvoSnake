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
    int length = 3;
    struct Snake **ptrs = initSnake();
    initMap();
    direction t = LEFT;
    gFood();
    int score=0;
    while (length!=0) {
        t=getd(t);
        length = moveSnake(ptrs,length,t);
    }
    terminate(length);
    getchar();
    endwin();
    return 0;
}
