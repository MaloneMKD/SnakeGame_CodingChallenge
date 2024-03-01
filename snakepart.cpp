#include "snakepart.h"

SnakePart::SnakePart(QRectF rect, QGraphicsItem *parent): QGraphicsRectItem(rect, parent)
{
    m_snakeSpeedX = 1;
    m_snakeSpeedY = 0;
}


int SnakePart::snakeSpeedX() const
{
    return m_snakeSpeedX;
}

int SnakePart::snakeSpeedY() const
{
    return m_snakeSpeedY;
}

void SnakePart::setSnakeSpeed(int newSnakeSpeedX, int newSnakeSpeedY)
{
    m_snakeSpeedX = newSnakeSpeedX;
    m_snakeSpeedY = newSnakeSpeedY;
}

QPointF SnakePart::getNextPos() const
{
    return m_nextPos;
}

void SnakePart::setNextPos(QPointF pos)
{
    m_nextPos = pos;
}

int SnakePart::crashDX() const
{
    return m_crashDX;
}

void SnakePart::setCrashDX(int newCrashDX)
{
    m_crashDX = newCrashDX;
}

int SnakePart::crashDY() const
{
    return m_crashDY;
}

void SnakePart::setCrashDY(int newCrashDY)
{
    m_crashDY = newCrashDY;
}

