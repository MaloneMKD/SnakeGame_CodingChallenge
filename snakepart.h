#ifndef SNAKEPART_H
#define SNAKEPART_H

#include <QGraphicsRectItem>
#include <QtCore>

class SnakePart : public QGraphicsRectItem
{
public:
    SnakePart(QRectF rect, QGraphicsItem* parent = nullptr);

    int snakeSpeedX() const;
    int snakeSpeedY() const;
    void setSnakeSpeed(int newSnakeSpeedX, int newSnakeSpeedY);
    QPointF getNextPos() const;
    void setNextPos(QPointF pos);

    int crashDX() const;
    void setCrashDX(int newCrashDX);

    int crashDY() const;
    void setCrashDY(int newCrashDY);

private:
    QPointF m_nextPos;
    int m_snakeSpeedX;
    int m_snakeSpeedY;
    qreal m_crashDX;
    qreal m_crashDY;
};

#endif // SNAKEPART_H
