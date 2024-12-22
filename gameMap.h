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
#include "parameters.h"
#include "belt.h"
#include "miner.h"
#include "cutter.h"
#include "trash.h"
#include "helpPage.h"
#include "mineral.h"

// #include "ui_gameMap.h"
class Belt;
class Mineral;

struct MapData
{
	// 存储矿物和障碍的位置
	QVector<QPoint> cycleMines;
	QVector<QPoint> rectMines;
	QVector<QPoint> barriers;
	QVector<QPoint> hubSmall;
	QVector<QVector<Device*>> devices;

	MapData() :devices(WINDOW_WIDTH / GRID_SIZE * 2, QVector<Device*>(WINDOW_HEIGHT / GRID_SIZE * 2, nullptr)) {}
};

class GameMap : public QWidget
{
	Q_OBJECT

public:
	GameMap(QWidget* parent = nullptr);
	~GameMap();
	static QVector<Mineral*> mineralList; // 全局矿物对象列表

protected:
	// 处理鼠标事件
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void deleteDeviceAt(const QPoint& pos);
	// 当前地图数据
	MapData currentMap;


private:
	// Ui::gameMapClass ui;

	/*** 图标 ***/
	QPixmap cycleMineImage;
	QPixmap rectMineImage;
	QPixmap barrierImage;
	QPixmap hubSmallImage;

	QVector<QPixmap> beltToPlace;
	QVector<QPixmap> minerToPlace;
	QVector<QPixmap> cutterToPlace;
	QVector<QPixmap> trashToPlace;

	// 当前鼠标位置
	QPoint mousePosition;
	// 图标旋转角度
	int rotationState = 0;

	/*** 地图 ***/

	// 存储多个地图数据
	QVector<MapData> maps;
	// 初始化地图
	void initializeMapA(MapData& mapA);
	void initializeMapB(MapData& mapB);
	void initializeMapC(MapData& mapC);
	// 选择需加载的地图号
	void loadMap(int level);
	// 绘制地图
	void paintEvent(QPaintEvent* event);
	// 格子是否为空
	bool isEmptyGrid = true;

	/*** 物品 ***/

	// 是否可以放下/删除物品
	bool canPlaceBelt;
	bool canPlaceMiner;
	bool canPlaceCutter;
	bool canPlaceTrash;
	bool isPlacingBelt;
	bool isDeleting;

	// 放置物品的具体过程
	void placeBeltAt(const QPoint& pos);
	void placeMinerAt(const QPoint& pos);
	void placeCutterAt(const QPoint& pos);
	void placeTrashAt(const QPoint& pos);

	// 放置的传送带列表
	QVector<Belt*> beltList;
	// 判断传送带方向
	void getBeltDirection(QPoint currentPosition);
	bool isDirectionChanged = false;

	/*** 槽 ***/

private slots:
	// 鼠标单击某按钮后，标记为可以放下该物品
	void onButtonClicked(QString s);
	// 右键删除操作
	void rightClicked(Device* device);
	// 新生成矿物的槽函数
	void onNewMineralGenerated(Mineral* mineral);
};

#endif