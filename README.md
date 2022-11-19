## EvoSnake介绍

![pic](https://ghproxy.com/raw.githubusercontent.com/NutshellEOF/EvoSnake/main/pic/Snipaste_2022-11-07_23-19-15.png)

一个在你的unix终端中运行的贪吃蛇小游戏

这个小玩具是我用来练手的

这只是一个拿来练手的小玩具，后续将慢慢地修正

## 如何Play

首先确保你的电脑或者服务器上有`ncurses`库

因为这个程序依赖`libncurses.so`运行

安装`ncurses`步骤如下，以Debian系为例

```shell
sudo apt install libncurses-dev
```

打开程序后用方向键移动，碰到食物（显示为*）时长度会增加，撞墙或者咬到自己都会结束游戏

**运行错误返回值**

1：用户自己造成的

2：终端大小不够（可以修改`util.h`中`MAP_L`和`MAP_H`的值以自定义大小）

3：终端不支持颜色

其他：出现了一些未知错误

## 如何编译

我在项目里配置了`CMake`你可以在clion中直接编译
也可以执行./build文件

或者可以：

```
gcc main.c evosnake.c evosnake.h  util.h -Wall -O2 -o evosnake -lncurses
```

随后你会得到一个无error无warning的可执行文件evosnake

## 实现原理

详见我的文档 [https://inutsh.me/p/7dc77920/](https://inutsh.me/p/7dc77920/)

## License

MIT License

您可以不受限制地处理本软件，包括但不限于使用、复制、修改、合并 、发布、分发、再许可的权利，**惟需在软件和软件的所有副本中都必须包含以上著作权声明和本许可声明**。
