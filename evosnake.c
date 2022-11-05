//
// Created by NutshellEOF on 2022/10/2.
//

//规范写代码只是为了可读性

#include "evosnake.h"

MapBlock *map;
struct Snake **ptrs;
//这个d数组是为了简化方向判断代码
int d[4]={DOWN,UP,LEFT,RIGHT};

void init() {
    initscr();noecho();cbreak();keypad(stdscr,TRUE);curs_set(0);halfdelay(3);
    //暂不支持在无颜色的模式下运行
    if (!has_colors()) {
        printf("[Error:2]Your terminal does not support color.\n");
        endwin();exit(2);
    }
    //暂不支持自定义大小，maybe下一次提交会更新？
    if (getmaxy(stdscr) <= MAP_H+3 || getmaxx(stdscr) <= MAP_L+4) {
        printf("[Error:3]Terminal size error, %dx%d is required.\n",MAP_L+3,MAP_H+4);
        endwin();exit(3);
    }
    map = (MapBlock *) malloc(sizeof(MapBlock) * (MAP_L + 2) * (MAP_H + 2));
    start_color();
    init_pair(WALL_PAIR, COLOR_WHITE, COLOR_WHITE);
    init_pair(SNAKE_PAIR, COLOR_CYAN, COLOR_BLACK);
    init_pair(FOOD_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(MSG_PAIR, COLOR_CYAN, COLOR_BLACK);
    init_pair(DHEAD_PAIR, COLOR_RED, COLOR_BLACK);
    initSnake();initMap();
}

char blockDisplay(int stat) {
    switch (stat) {
        case 3:return '$';
        case 2:return 'o';
        case 1:return '@';
        default:return ' ';
    }
}

void initMap() {
    attron(COLOR_PAIR(MSG_PAIR));
    mvprintw(START_Y-1, START_X, "EvoSnake v1.5");
    mvprintw(START_Y-1, START_X+MAP_L-12, "By NutshellEOF");
    attroff(COLOR_PAIR(MSG_PAIR));
    attron(COLOR_PAIR(WALL_PAIR));
    for (int i = 0; i < (MAP_H+2)*(MAP_L+2); ++i) {
        map[i].x = i % (MAP_L + 2);map[i].y = i / (MAP_L + 2); map[i].stat = 0;
        if (map[i].x == (MAP_L+1) || map[i].x == 0) map[i].stat = -1;
        if (map[i].y == (MAP_H+1) || map[i].y == 0) map[i].stat = -1;
        if (map[i].stat==-1) {
            mvaddch(map[i].y+START_Y,map[i].x+START_X,blockDisplay(map[i].stat));
            refresh();
        }
    }
    attroff(COLOR_PAIR(WALL_PAIR));
    gFood();
}

void drawMap(int changeId, int pairId) {
    attron(COLOR_PAIR(pairId));
    mvaddch(map[changeId].y+START_Y,map[changeId].x+START_X,blockDisplay(map[changeId].stat));
    attroff(COLOR_PAIR(pairId));
    refresh();
}

void gFood() {
    srand(time(NULL));int fid;
    while (map[fid = rand() % ((MAP_H+2)*(MAP_L+2))].stat != 0);
    map[fid].stat = 3;drawMap(fid, FOOD_PAIR);
}

int getd(int pt) {
    //0402: KEY_DOWN in ncurses
    int c=getch(), tmp=((c>=0402&&c<=0405)?d[c-0402]:pt);
    return (tmp+pt == 0)?pt:tmp;
}

void initSnake() {
    map[(MAP_H/2+1)*(MAP_L+2)+MAP_L/2+1].stat=1;drawMap((MAP_H/2+1)*(MAP_L+2)+MAP_L/2+1,SNAKE_PAIR);
    map[(MAP_H/2+1)*(MAP_L+2)+MAP_L/2+2].stat=2;drawMap((MAP_H/2+1)*(MAP_L+2)+MAP_L/2+2,SNAKE_PAIR);
    map[(MAP_H/2+1)*(MAP_L+2)+MAP_L/2+3].stat=2;drawMap((MAP_H/2+1)*(MAP_L+2)+MAP_L/2+3,SNAKE_PAIR);
    struct Snake *shead=(struct Snake*)malloc(sizeof(struct Snake));
    struct Snake *sbody1=(struct Snake*)malloc(sizeof(struct Snake));
    struct Snake *sbody2=(struct Snake*)malloc(sizeof(struct Snake));
    shead->dest=(MAP_H/2+1)*(MAP_L+2)+MAP_L/2+1;sbody1->dest=(MAP_H/2+1)*(MAP_L+2)+MAP_L/2+1+1;sbody2->dest=(MAP_H/2+1)*(MAP_L+2)+MAP_L/2+3;
    shead->next=sbody1;sbody1->next=sbody2;sbody2->next=NULL;
    shead->pre=NULL;sbody1->pre=shead;sbody2->pre=sbody1;
    ptrs = (struct Snake **)malloc(sizeof(struct Snake *)*2);
    ptrs[0] = shead;ptrs[1] = sbody2;
}

int moveSnake(int length, int t) {
    struct Snake *nhead = (struct Snake *) malloc(sizeof(struct Snake));
    map[ptrs[0]->dest].stat = 2;
    drawMap(ptrs[0]->dest,SNAKE_PAIR);
    nhead->dest = ptrs[0]->dest + t;
    nhead->next = ptrs[0];
    nhead->pre = NULL;
    ptrs[0]->pre = nhead;ptrs[0] = nhead;
    if (map[ptrs[0]->dest].stat == 3) {
        struct Snake *addedtail = (struct Snake *)malloc(sizeof(struct Snake));
        length++;gFood();
        addedtail->dest = ptrs[1]->dest-t;addedtail->next = NULL;addedtail->pre = ptrs[1];
        ptrs[1]->next = addedtail;ptrs[1] = addedtail;
        map[addedtail->dest].stat = 2;
        drawMap(addedtail->dest, SNAKE_PAIR);
    } else if (map[nhead->dest].stat == -1 || map[nhead->dest].stat == 2) {
        attron(COLOR_PAIR(DHEAD_PAIR));
        mvaddch(map[nhead->dest].y+START_Y,map[nhead->dest].x+START_X,'X');
        attroff(COLOR_PAIR(DHEAD_PAIR));return 0;
    }
    map[nhead->dest].stat = 1;
    drawMap(nhead->dest,SNAKE_PAIR);
    struct Snake *tail = ptrs[1];
    map[tail->dest].stat = 0;
    drawMap(tail->dest, SNAKE_PAIR);
    ptrs[1] = tail->pre;free(tail);
    ptrs[1]->next = NULL;
    return length;
}

void terminate(int length) {
    attron(COLOR_PAIR(MSG_PAIR));
    mvaddstr((MAP_H+1)/2+START_Y,(MAP_L+1)/2-4+START_X,"You lost.");
    mvprintw((MAP_H+1)/2+1+START_Y,(MAP_L+1)/2-4+START_X,"Score: %d0", length-3);
    refresh();
    attroff(COLOR_PAIR(MSG_PAIR));getchar();endwin();
    //释放是一个好习惯
    struct Snake *p = ptrs[0];while (p->next!=NULL) {p=p->next;free(p->pre);}
    free(p);free(ptrs);free(map);
}