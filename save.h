#ifndef SAVE_H
#define SAVE_H

#include <QVector>
#include <QPoint>
#include <QDataStream>

struct Save {
	QVector<QPoint> cycleMines;
	QVector<QPoint> rectMines;
	QVector<QPoint> barriers;
	QVector<QPoint> hubSmall;
	QVector<QVector<int>> devices; // 使用设备的类型ID来表示设备
	int hubMineralCount;

	Save() : devices(WINDOW_WIDTH / GRID_SIZE * 2, QVector<int>(WINDOW_HEIGHT / GRID_SIZE * 2, -1)) {}

	friend QDataStream& operator<<(QDataStream& out, const Save& data) {
		out << data.cycleMines << data.rectMines << data.barriers << data.hubSmall << data.devices << data.hubMineralCount;
		return out;
	}

	friend QDataStream& operator>>(QDataStream& in, Save& data) {
		in >> data.cycleMines >> data.rectMines >> data.barriers >> data.hubSmall >> data.devices >> data.hubMineralCount;
		return in;
	}
};

// 注册自定义类型
Q_DECLARE_METATYPE(QVector<QPoint>)

#endif // SAVE_H