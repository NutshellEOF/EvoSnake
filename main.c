#include <curses.h>
#include <stdlib.h>
#include "util.h"
#include "evosnake.h"

/*
Compile: gcc main.c -lncurses
*/
int main()
{
    //设置窗口
    initscr();
    noecho();
    cbreak();
    keypad(stdscr,TRUE);
    halfdelay(4);
    //初始化地图
    MapBlock *map =malloc(sizeof(MapBlock)*(MAP_L+2)*(MAP_H+2));
    int length = 2;
    struct Snake **ptrs = initSnake(map);
    initMap(map);
    direction t =LEFT;
    gFood(map);
    int score=0;
    while (length!=0) {
        direction tmp = t;
        switch (getch()) {
            case KEY_DOWN:
                t=DOWN;
                break;
            case KEY_UP:
                t=UP;
                break;
            case KEY_LEFT:
                t=LEFT;
                break;
            case KEY_RIGHT:
                t=RIGHT;
                break;
        }
        (tmp+t==0)?t=tmp:t;
        score=(length-2)*10;
        length = moveSnake(map,ptrs,length,t);
        mvaddch(MAP_H+1,MAP_L+1,'#');
    }
    terminate(length,score);
    getchar();
    endwin();
                /* 结束curses 模式*/
    return 0;
}
