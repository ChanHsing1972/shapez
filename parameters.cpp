// Created by Chenxin.
// 参数类，包含窗口大小、按钮大小、方向、矿物、颜色、设备、参数等信息。

#include "parameters.h"

int WINDOW_WIDTH = 1920;
int WINDOW_HEIGHT = 1200;

int BELT_SPEED = 1; // 1 格/time
int MINER_SPEED = 2405; // 2.4 秒生成一个矿物
int CUTTER_SPEED = 1000; // 1 秒切割一个矿物

//int BELT_SPEED = 3; // 1 格/time
//int MINER_SPEED = 1200; // 2.4 秒生成一个矿物
//int CUTTER_SPEED = 250; // 1 秒切割一个矿物

int minerLevel = 1;
int beltLevel = 1;
int cutterLevel = 1;

int minerCost = 10;
int beltCost = 10;
int cutterCost = 5;