//
// Created by NutshellEOF on 2022/10/2.
//
#include "util.h"
#include "evosnake.h"

int main() {
    init();
    int length = 3,tmp=length;
    int t = LEFT;
    while (tmp!=0) {
        t=getd(t);
        tmp = moveSnake(length,t);
        length=(tmp==0)?length:tmp;
    }
    terminate(length);
    return 0;
}
