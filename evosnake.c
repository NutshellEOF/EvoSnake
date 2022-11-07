//
// Created by NutshellEOF on 2022/10/2.
//

//规范写代码只是为了可读性

#include "evosnake.h"

MapBlock *map;
//ptrs[0]为头部 ptrs[1]为尾部
struct Snake **ptrs;
//这个d数组是为了简化方向判断代码
int d[4]={DOWN,UP,LEFT,RIGHT};

void init() {
    initscr();noecho();cbreak();keypad(stdscr,TRUE);curs_set(0);halfdelay(4);
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
    initMap();initSnake();
}

char blockDisplay(int stat) {
    switch (stat) {
        case 3:return '$';
        case 2:return 'O';
        case 1:return '@';
        default:return ' ';
    }
}

void initMap() {
    attron(COLOR_PAIR(MSG_PAIR));
    mvprintw(START_Y-2, START_X-1, "EvoSnake v1.6");
    mvprintw(START_Y-2, START_X+MAP_L-13, "By NutshellEOF");
    attroff(COLOR_PAIR(MSG_PAIR));
    for (int i = 0; i < MAP_H*MAP_L; ++i) {
        map[i].x = i % MAP_L ;map[i].y = i / MAP_L ; map[i].stat = 0;
    }
    //绘制围墙
    attron(COLOR_PAIR(WALL_PAIR));
    for (int i = 0; i < MAP_L+2; ++i) {
        mvaddch(START_Y-1, START_X-1+i, ' ');//上
        mvaddch(START_Y+MAP_H, START_X-1+i, ' ');//下
    }
    for (int i = 0; i < MAP_H+2; ++i) {
        mvaddch(START_Y-1+i, START_X-1, ' ');//左
        mvaddch(START_Y-1+i, START_X+MAP_L, ' ');//右
    }
    attroff(COLOR_PAIR(WALL_PAIR));
    gFood();
}
//绘制元素时均使用加粗加亮显示，这样更好看
void drawMap(int changeId, int pairId) {
    attron(COLOR_PAIR(pairId));attron(A_BOLD);
    mvaddch(map[changeId].y+START_Y,map[changeId].x+START_X,blockDisplay(map[changeId].stat));
    attroff(A_BOLD);attroff(COLOR_PAIR(pairId));
    refresh();
}

void gFood() {
    srand(time(NULL));int fid;
    while (map[fid = rand() % (MAP_H*MAP_L)].stat != 0);
    map[fid].stat = 3;drawMap(fid, FOOD_PAIR);
}
//免判断直接得出方向
int getd(int pt) {
    //0402: KEY_DOWN in ncurses
    int c=getch(), tmp=((c>=0402&&c<=0405)?d[c-0402]:pt);
    return (tmp+pt == 0)?pt:tmp;
}

void initSnake() {

    map[SNAKE_INIT_HDEST].stat=1;
    drawMap(SNAKE_INIT_HDEST, SNAKE_PAIR);
    map[SNAKE_INIT_HDEST+1].stat=2;
    drawMap(SNAKE_INIT_HDEST+1,SNAKE_PAIR);
    map[SNAKE_INIT_HDEST+2].stat=2;
    drawMap(SNAKE_INIT_HDEST+2,SNAKE_PAIR);

    struct Snake *shead=(struct Snake*)malloc(sizeof(struct Snake));
    struct Snake *sbody1=(struct Snake*)malloc(sizeof(struct Snake));
    struct Snake *sbody2=(struct Snake*)malloc(sizeof(struct Snake));
    //这样写链表初始化的代码不舒服吗[/doge]
    shead->dest=SNAKE_INIT_HDEST;sbody1->dest=SNAKE_INIT_HDEST+1;sbody2->dest=SNAKE_INIT_HDEST+2;
    shead->next=sbody1;sbody1->next=sbody2;sbody2->next=NULL;
    shead->pre=NULL;sbody1->pre=shead;sbody2->pre=sbody1;
    //分配存储首尾节点的空间
    ptrs = (struct Snake**)malloc(sizeof(struct Snake *)*2);
    ptrs[0] = shead;ptrs[1] = sbody2;
}

//为了解决那个问题最后写的烂代码！
int validDest(int dest, int t) {
    int y=(dest+t)/MAP_L, x=(dest+t)%MAP_L;
    int py=dest/MAP_L, px=dest%MAP_L;
    if (y<=MAP_H-1 && y>=0 && x>=0 && x<=MAP_L-1) {
        if (abs(px-x)!=MAP_L-1|| abs(py-y)!=1) return 1;
    }
    return 0;
}
//绘制dead head
int drawDhead(int dest) {
    attron(COLOR_PAIR(DHEAD_PAIR));attron(A_BOLD);
    mvaddch(dest/MAP_L+START_Y, dest%MAP_L+START_X, 'X');
    attroff(A_BOLD);attroff(COLOR_PAIR(DHEAD_PAIR));
    refresh();
    return 0;
}

int moveSnake(int length, int t) {
    int  pdest=ptrs[0]->dest, ndest=pdest+t;
    if (validDest(pdest, t)) {//首先判断是否超出地图，以免越界读写
        if (map[ndest].stat==2) return drawDhead(pdest);
        else if (map[ndest].stat==3) {
            struct Snake *addedtail = (struct Snake *)malloc(sizeof(struct Snake));
            length++;gFood();
            addedtail->dest = ptrs[1]->dest-t;addedtail->next = NULL;addedtail->pre = ptrs[1];
            ptrs[1]->next = addedtail;ptrs[1] = addedtail;
            map[addedtail->dest].stat = 2;
            drawMap(addedtail->dest, SNAKE_PAIR);
        }
    } else return drawDhead(pdest);
    //这才是真正的开始
    struct Snake *nhead = (struct Snake *) malloc(sizeof(struct Snake));
    map[ptrs[0]->dest].stat = 2;
    drawMap(ptrs[0]->dest,SNAKE_PAIR);
    nhead->dest = ptrs[0]->dest + t;
    nhead->next = ptrs[0];
    nhead->pre = NULL;
    ptrs[0]->pre = nhead;ptrs[0] = nhead;

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
    mvprintw((MAP_H+1)/2+1+START_Y,(MAP_L+1)/2-4+START_X,"Score: %d", (length-3)*10);
    refresh();
    attroff(COLOR_PAIR(MSG_PAIR));getchar();endwin();
    //释放是一个好习惯
    struct Snake *p = ptrs[0];while (p->next!=NULL) {p=p->next;free(p->pre);}
    free(p);free(ptrs);free(map);
}