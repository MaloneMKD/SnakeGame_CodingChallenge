#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QtGui>
#include <QtCore>
#include <QTimer>
#include <QGraphicsSceneMouseEvent>
#include "snakepart.h"

class game_scene: public QGraphicsScene
{
    Q_OBJECT

public:
    game_scene(QRectF rect, QGraphicsView *view, QObject *parent);

    void reset();
    void restart();

public slots:
    void updateObjects();
    void spawnFood();
    void start();
    void stop();
    void scored();
    void crashAnimation();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    QGraphicsView *m_view;
    QGraphicsRectItem *m_gameCanvas;
    QGraphicsEllipseItem *m_food;
    SnakePart *m_snakeHead;
    QList<SnakePart*> m_snakeBody;
    QTimer *m_gameTimer;
    QTimer *m_crashAnimationTimer;
    int m_snakeSpeedX;
    int m_snakeSpeedY;
};

#endif // GAME_SCENE_H
