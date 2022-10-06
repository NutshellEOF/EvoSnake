//
// Created by NutshellEOF on 2022/10/2.
//

#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "util.h"

char blockDisplay(int stat) {
    switch (stat) {
        case -1:
            return '#';
        case 3:
            return '*';
        case 2:
            return '+';
        case 1:
            return '@';
        default:
            return ' ';
    }
}

MapBlock *initMap(MapBlock *map) {
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
}

void drawMap(MapBlock *map, int changeId) {
    mvaddch(map[changeId].y,map[changeId].x,
            blockDisplay(map[changeId].stat));
    refresh();
}

void gFood(MapBlock *map) {
    srand(time(NULL));
    int fid = rand()%((MAP_H+2)*(MAP_L+2));
    while (map[fid].stat != 0){
        fid = rand()%((MAP_H+2)*(MAP_L+2));
    }
    map[fid].stat = 3;
    drawMap(map,fid);
}

struct Snake **initSnake(MapBlock *map) {
    int x = MAP_L/2 + 1;
    int y = MAP_H/2 + 1;

    int head = y*(MAP_L+2) + x;
    map[head].stat = 1;
    map[head+1].stat = 2;
    drawMap(map, head);
    drawMap(map, head+1);

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

int moveSnake(MapBlock *map, struct Snake **ptrs,int length, direction t) {
    /* 这些代码先后顺序非常重要，谨慎更改！ */
    struct Snake *nhead = (struct Snake *) malloc(sizeof(struct Snake)); //新的头节点

    map[ptrs[0]->dest].stat = 2;//将原来的头在地图中改为身体
    drawMap(map, ptrs[0]->dest);

    nhead->dest = ptrs[0]->dest + t;
    nhead->next = ptrs[0];
    nhead->pre = NULL;

    ptrs[0]->pre = nhead;
    ptrs[0] = nhead;

    if (map[ptrs[0]->dest].stat == 3) {
        struct Snake *addedtail = (struct Snake *)malloc(sizeof(struct Snake));
        length++;
        gFood(map);

        addedtail->dest = ptrs[1]->dest-t;//先在链表后面加一个，实际上在后面这个节点要被去掉
        addedtail->next = NULL;
        addedtail->pre = ptrs[1];

        ptrs[1]->next = addedtail;
        ptrs[1] = addedtail;
        map[addedtail->dest].stat = 2;
        drawMap(map,addedtail->dest);
    }
    //撞墙或吃到自己
    else if (map[nhead->dest].stat == -1 || map[nhead->dest].stat == 2) {
        return 0;
    }

    map[nhead->dest].stat = 1;
    drawMap(map,nhead->dest);
    //清掉最后的节点
    struct Snake *tail = ptrs[1];
    map[tail->dest].stat = 0;
    drawMap(map, tail->dest);

    ptrs[1] = tail->pre;
    free(tail);
    ptrs[1]->next = NULL;
    return length;
}

void terminate(int code, int score) {
    if (code == 0) {
        mvaddstr((MAP_H+1)/2,(MAP_L+1)/2-4,"You lost.");
        mvprintw((MAP_H+1)/2+1,(MAP_L+1)/2-4,"Score: %d",score);
    }
    refresh();
}