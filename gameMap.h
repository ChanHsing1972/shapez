// Created by ChenXin.
// 游戏地图类，是实现游戏主要功能的部分，
// 包括游戏地图的绘制、物品的放置与删除、存档、任务等功能。

#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <QPainter>
#include <QWidget>
#include <QTimer>
#include <QVector>
#include <QPushButton>
#include <QDebug>
#include <QMouseEvent>
#include <QPoint>
#include <QPixmap>
#include <QApplication>
#include <QScreen>
#include <QSoundEffect>
#include <QFile>
#include <QDataStream>
#include <QSettings>
#include "parameters.h"
#include "belt.h"
#include "miner.h"
#include "cutter.h"
#include "trash.h"
#include "helpPage.h"
#include "shopPage.h"
#include "comletePage.h"
#include "mineral.h"
#include "hub.h"

class Belt;
class Mineral;
class Hub;

struct Task // 定义任务结构体
{
	QString description; // 任务描述
	int mineralType;     // 矿物类型
	int targetCount;     // 目标数量
	int currentCount;    // 当前数量
	QPixmap icon;        // 任务图标
};

struct MapData // 定义地图数据结构体
{
	// 存储矿物和障碍的位置
	QVector<QPoint> cycleMines;
	QVector<QPoint> rectMines;
	QVector<QPoint> barriers;
	QVector<QPoint> hubSmall;
	QVector<QVector<Device*>> devices;

	MapData() : devices(WINDOW_WIDTH / GRID_SIZE * 2, QVector<Device*>(WINDOW_HEIGHT / GRID_SIZE * 2, nullptr)) {}
};

class GameMap : public QWidget
{
	Q_OBJECT

public:
	GameMap(QWidget* parent = nullptr);
	~GameMap();
	static QVector<Mineral*> mineralList;                 // 全局矿物对象列表
	void saveGame(const QString& fileName);                // 保存游戏
	void loadGame(const QString& fileName);                // 加载游戏
	void updateTaskProgress(QVector<int>& collectedMines); // 更新任务进度

public slots:
	void startFadeInAnimation(); // 控制淡入动画

protected:
	// 处理鼠标/键盘事件
	void mousePressEvent(QMouseEvent* event) override;   // 鼠标按下事件
	void mouseMoveEvent(QMouseEvent* event) override;    // 鼠标移动事件
	void mouseReleaseEvent(QMouseEvent* event) override; // 鼠标释放事件
	void keyPressEvent(QKeyEvent* event) override;       // 键盘按下事件
	void deleteDeviceAt(const QPoint& pos);              // 删除设备
	// 当前地图数据
	MapData currentMap;

private:
	/*** 图标 ***/

	QPixmap cycleMineImage;
	QPixmap rectMineImage;
	QPixmap barrierImage;
	QPixmap hubSmallImage;

	QVector<QPixmap> beltToPlace;
	QVector<QPixmap> minerToPlace;
	QVector<QPixmap> cutterToPlace;
	QVector<QPixmap> trashToPlace;

	int rotationState = 0; // 图标旋转角度

	/*** 地图 ***/

	QVector<MapData> maps;   // 存储多个地图数据
	QPixmap cachedStaticMap; // 缓存静态地图

	// 初始化地图（可以初始化多个地图，实际游戏中只用到了一个）
	void initializeMapA(MapData& mapA);
	void initializeMapB(MapData& mapB);
	void initializeMapC(MapData& mapC);

	void loadMap(int level);                   // 选择需加载的地图号
	void cacheStaticMap();                     // 缓存静态地图
	void paintEvent(QPaintEvent* event);       // 绘制地图
	bool isEmptyGrid = true;                   // 判断格子是否为空
	bool checkEmptyGrid(int gridX, int gridY); // 检查格子是否为空

	/*** 物品 ***/

	// 是否可以放下/删除物品
	bool canPlaceBelt;
	bool canPlaceMiner;
	bool canPlaceCutter;
	bool canPlaceTrash;
	bool isPlacingBelt;
	bool isDeleting;
	bool placeBeltFeedback;

	// 放置物品的具体过程
	void placeBeltAt(const QPoint& pos);
	void placeMinerAt(const QPoint& pos);
	void placeCutterAt(const QPoint& pos);
	void placeTrashAt(const QPoint& pos);

	Hub* hub;                                      // 交付中心
	QVector<Belt*> beltList;                       // 放置的传送带列表
	void getBeltDirection(QPoint currentPosition); // 获取传送带的方向
	bool isDirectionChanged = false;               // 判断方向是否改变
	QPoint mousePosition;                          // 当前鼠标位置

	/*** 音效 ***/

	QSoundEffect* placeSound;
	QSoundEffect* deleteSound;
	QSoundEffect* chooseSound;

	/*** 存档 ***/

	void createDeviceByTypeID(int typeID, int i, int j); // 根据设备 ID 创建设备
	void autoSaveGame();                                 // 自动保存游戏

	/*** 任务 ***/

	QVector<Task> tasks;            // 任务列表
	QLabel* taskLabel;              // 任务内容
	QLabel* taskIconLabel;          // 任务图标
	QHBoxLayout* taskLayout;        // 任务布局
	int currentTaskIndex;           // 当前任务索引
	CompletePage* completionWidget; // 完成任务的提示窗口
	void updateTaskDisplay();       // 更新任务显示

	/*** 槽 ***/

private slots:
	void onButtonClicked(QString s);              // 鼠标单击某按钮后，标记为可以放下该物品
	void rightClicked(Device* device);            // 右键删除设备操作
	void onNewMineralGenerated(Mineral* mineral); // 新生成矿物的槽函数
};

#endif