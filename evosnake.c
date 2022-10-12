//
// Created by NutshellEOF on 2022/10/2.
//

#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "util.h"

MapBlock *map;

void init() {
    //设置窗口
    initscr();
    noecho();
    cbreak();
    keypad(stdscr,TRUE);
    halfdelay(3);
    if (has_colors() == FALSE) {
        printf("Your terminal does not support color.\n");
        //printf("We will work in no-color mode\n");
        printf("Press any key to continue.");
        getchar();
        endwin();
        exit(3);
    }
    int c,r;//判断大小是否合适
    getmaxyx(stdscr, r, c);
    if (r <= MAP_H+1 || c <= MAP_L+2) {
        endwin();
        printf("Terminal size error, %dx%d is required.\n",MAP_L,MAP_H);
        exit(2);
    }
    map = (MapBlock *) malloc(sizeof(MapBlock) * (MAP_L + 2) * (MAP_H + 2));
    start_color();
    init_pair(MAP_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(SNAKE_PAIR, COLOR_CYAN, COLOR_BLACK);
    init_pair(FOOD_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(ERR_PAIR, COLOR_CYAN, COLOR_BLACK);
}

char blockDisplay(int stat) {
    switch (stat) {
        case -1:
            return '#';
        case 3:
            return '*';
        case 2:
            return 'o';
        case 1:
            return '@';
        default:
            return ' ';
    }
}

void initMap() {
    attron(COLOR_PAIR(MAP_PAIR));
    for (int i = 0; i < (MAP_H+2)*(MAP_L+2); ++i) {
        map[i].x = i % (MAP_L + 2);
        map[i].y = i / (MAP_L + 2);

        map[i].stat = 0;
        if (map[i].x == (MAP_L+1) || map[i].x == 0)
            map[i].stat = -1;
        if (map[i].y == (MAP_H+1) || map[i].y == 0)
            map[i].stat = -1;
        mvaddch(map[i].y,map[i].x,
                blockDisplay(map[i].stat));
        refresh();
    }
    attroff(COLOR_PAIR(MAP_PAIR));
}

void drawMap(int changeId, int pairId) {
    attron(COLOR_PAIR(pairId));
    mvaddch(map[changeId].y,map[changeId].x,
            blockDisplay(map[changeId].stat));
    attroff(COLOR_PAIR(pairId));
    refresh();
}

void gFood() {
    srand(time(NULL));
    int fid = rand() % ((MAP_H+2)*(MAP_L+2));
    while (map[fid].stat != 0){
        fid = rand() % ((MAP_H+2)*(MAP_L+2));
    }
    map[fid].stat = 3;
    drawMap(fid, FOOD_PAIR);
}

direction getd(direction pt) {
    direction tmp = pt;
    switch (getch()) {
        case KEY_DOWN:
            tmp = DOWN;
            break;
        case KEY_UP:
            tmp = UP;
            break;
        case KEY_LEFT:
            tmp = LEFT;
            break;
        case KEY_RIGHT:
            tmp = RIGHT;
            break;
    }
    (tmp+pt == 0)?tmp=pt:tmp;
    return tmp;
}

struct Snake **initSnake() {
    int x = MAP_L/2 + 1;
    int y = MAP_H/2 + 1;


    int head = y*(MAP_L+2) + x;
    map[head].stat = 1;
    map[head+1].stat = 2;
    drawMap(head, SNAKE_PAIR);
    drawMap(head+1, SNAKE_PAIR);

    struct Snake *ptr = (struct Snake *)malloc(sizeof(struct Snake));

    ptr->dest = head;
    ptr->next = (struct Snake *)malloc(sizeof(struct Snake));
    ptr->pre = NULL;

    ptr->next->dest = head+1;
    ptr->next->next = NULL;
    ptr->next->pre = ptr;

    //使用数组存储链表头尾节点
    struct Snake **ptrs = (struct Snake **)malloc(sizeof(struct Snake *)*2);

    ptrs[0] = ptr;//头节点
    ptrs[1] = ptr->next;//尾节点

    return ptrs;
}

int moveSnake(struct Snake **ptrs,int length, direction t) {
    /* 这些代码先后顺序非常重要，谨慎更改！ */
    struct Snake *nhead = (struct Snake *) malloc(sizeof(struct Snake)); //新的头节点

    map[ptrs[0]->dest].stat = 2;//将原来的头在地图中改为身体
    drawMap(ptrs[0]->dest,SNAKE_PAIR);

    nhead->dest = ptrs[0]->dest + t;
    nhead->next = ptrs[0];
    nhead->pre = NULL;

    ptrs[0]->pre = nhead;
    ptrs[0] = nhead;

    if (map[ptrs[0]->dest].stat == 3) {
        struct Snake *addedtail = (struct Snake *)malloc(sizeof(struct Snake));
        length++;
        gFood();

        addedtail->dest = ptrs[1]->dest-t;//先在链表后面加一个，实际上在后面这个节点要被去掉
        addedtail->next = NULL;
        addedtail->pre = ptrs[1];

        ptrs[1]->next = addedtail;
        ptrs[1] = addedtail;
        map[addedtail->dest].stat = 2;
        drawMap(addedtail->dest, SNAKE_PAIR);
    }
    //撞墙或吃到自己
    else if (map[nhead->dest].stat == -1 || map[nhead->dest].stat == 2) {
        return 0;
    }

    map[nhead->dest].stat = 1;
    drawMap(nhead->dest,SNAKE_PAIR);
    //清掉最后的节点
    struct Snake *tail = ptrs[1];
    map[tail->dest].stat = 0;
    drawMap(tail->dest, SNAKE_PAIR);

    ptrs[1] = tail->pre;
    free(tail);
    ptrs[1]->next = NULL;
    return length;
}

void terminate(int code, int score) {
    attron(COLOR_PAIR(ERR_PAIR));
    if (code == 0) {
        mvaddstr((MAP_H+1)/2,(MAP_L+1)/2-4,"You lost.");
        mvprintw((MAP_H+1)/2+1,(MAP_L+1)/2-4,"Score: %d",score);
    }
    refresh();
    attroff(COLOR_PAIR(ERR_PAIR));
}