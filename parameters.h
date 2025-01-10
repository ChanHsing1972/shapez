#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <QFontDatabase>
#include <QFont>


// 窗口
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1200
#define GRID_SIZE 60

// 按钮
#define BUTTON_SIZE 80
#define ICON_SIZE 60
#define BELT_BUTTON_X 670
#define BELT_BUTTON_Y 1100
#define MINER_BUTTON_X 770
#define MINER_BUTTON_Y 1100
#define CUTTER_BUTTON_X 870 
#define CUTTER_BUTTON_Y 1100
#define TRASH_BUTTON_X 970 
#define TRASH_BUTTON_Y 1100
#define SHOP_BUTTON_X 1070
#define SHOP_BUTTON_Y 1100
#define HELP_BUTTON_X 1170
#define HELP_BUTTON_Y 1100

// 方向
#define _W 0
#define _D 1
#define _S 2
#define _A 3
#define _A_S 4
#define _W_A 5
#define _D_W 6
#define _S_D 7
#define _D_S 8	
#define _S_A 9
#define _A_W 10
#define _W_D 11

// 矿物
#define CYCLE_MINE 0
#define RECT_MINE 1
#define CYCLE_MINE_L 2
#define CYCLE_MINE_R 3
#define RECT_MINE_L 4
#define RECT_MINE_R 5
#define CYCLE_MINE_VALUE 10
#define RECT_MINE_VALUE 50
#define MINERAL_SIZE GRID_SIZE/1.5

// 颜色
#define GRAY 200, 200, 200

// 参数
extern int BELT_SPEED;
extern int MINER_SPEED;
extern int CUTTER_SPEED;
extern int minerLevel;
extern int beltLevel;
extern int cutterLevel;
#define MOVE_FREQUENCY 10
#define CHECK_FREQUENCY 100
#define EDGE_SIZE 5

#endif