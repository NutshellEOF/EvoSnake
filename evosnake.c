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
    int fid=rand()%((MAP_H+2)*(MAP_L+2));
    while (map[fid].stat!=0){
        fid=rand()%((MAP_H+2)*(MAP_L+2));
    }
    map[fid].stat=3;
    drawMap(map,fid);
}

struct Snake **initSnake(MapBlock *map) {
    int x=MAP_L/2 + 1;
    int y=MAP_H/2 + 1;

    int head=y*(MAP_L+2) + x;
    map[head].stat=1;
    map[head+1].stat=2;
    drawMap(map, head);
    drawMap(map, head+1);

    struct Snake *ptr = malloc(sizeof(struct Snake));

    ptr->dest=head;
    ptr->next=malloc(sizeof(struct Snake));
    ptr->next->dest=ptr->dest+1;
    ptr->next->next=NULL;

    struct Snake **ptrs = malloc(sizeof(struct Snake *)*3);
    ptrs[0]=ptr;
    ptrs[1]=ptr->next;
    ptrs[2]=ptr;

    return ptrs;
}

int moveSnake(MapBlock *map, struct Snake **ptrs,int length, direction t) {
    //新的头节点
    struct Snake *nhead = malloc(sizeof(struct Snake));
    //将原来的头修改为身体
    map[ptrs[0]->dest].stat=2;
    drawMap(map, ptrs[0]->dest);

    nhead->dest = ptrs[0]->dest + t;
    nhead->next=ptrs[0];
    //是否进化的标志位
    int flag = 0;
    struct Snake *addedtail = malloc(sizeof(struct Snake));
    if (map[nhead->dest].stat==3) {
        length++;flag=1;
        gFood(map);

        addedtail->dest=ptrs[0]->dest+t;
        addedtail->next=NULL;
    }
    //撞墙或吃到自己
    else if (map[nhead->dest].stat==-1||map[nhead->dest].stat==2) {
            length=0;
    }
    //将新的头在地图中更新
    map[nhead->dest].stat=1;
    drawMap(map, nhead->dest);
    //将新的头加入到链表中
    ptrs[0]=nhead;
    //读取尾节点，并将其在地图中标记为空白
    struct Snake *tail = ptrs[1];
    map[tail->dest].stat=0;
    drawMap(map,tail->dest);

    //更新存储三个重要节点的数组，其实有一个节点不必要
    ptrs[1]=ptrs[2];
    //tmp指向原尾节点的前节点，ptrs[2]在后面几步后要给尾节点赋值
    struct Snake *tmp = ptrs[0];
    //清除原尾节点，不要忘了free掉内存
    ptrs[2]->next=NULL;
    free(tail);
    //ptrs[2]=nhead;//修改此处！
    //暂时用下列代替，如果有更好的办法那一定是重写结构
    //若已经检测到进化，则撤销尾节点操作
    if (flag==1) {
        //usleep(400*1000);
        addedtail->next=ptrs[0];
        ptrs[0]=addedtail;
        drawMap(map,addedtail->dest);

    }

    if (length==2) {
        ptrs[2]=nhead;
        return length;
    }
    while (tmp->next->next!=NULL) {
        tmp=tmp->next;
    }
    ptrs[2]=tmp;
    return length;
}

void terminate(int code, int score) {
    if (code==0) {
        mvaddstr((MAP_H+1)/2,(MAP_L+1)/2-4,"You lost.");
        mvprintw((MAP_H+1)/2+1,(MAP_L+1)/2-4,"Score: %d",score);
    }
    refresh();
}