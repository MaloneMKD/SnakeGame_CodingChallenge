#include "GraphicsScene.h"
#include <QDebug>
#include <QGraphicsItem>
#include <math.h>
#include <random>
#include <time.h>
#include <QList>

game_scene::game_scene(QRectF rect, QGraphicsView *view, QObject *parent): QGraphicsScene(rect, parent)
{
    // Initialize other variables
    m_view = view;
    srand(time(0));

    // Create the game canvas
    m_gameCanvas = new QGraphicsRectItem(QRectF(0, 0, 1480, 720));
    m_gameCanvas->setPen(QPen(QColor("#242424")));
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->addItem(m_gameCanvas);

    // Create grid
    int x_pos = 0;
    int y_pos = 0;
    for(int i = 0; i < 36; i++)
    {
        for(int j = 0; j < 74; j++)
        {
            this->addRect(QRectF(x_pos, y_pos, 20, 20), QPen(QColor("#A4A4A4")));
            x_pos += 20;
        }
        x_pos = 0;
        y_pos += 20;
    }

    // Setup the snake
    m_snakeHead = new SnakePart(QRectF(1, 1, 20, 20));
    m_snakeHead->setPen(QPen(Qt::NoPen));
    m_snakeHead->setBrush(QBrush(QColor("#242424")));
    //m_snake.append(m_snakeHead);
    this->addItem(m_snakeHead);

    m_snakeSpeedX = 1;
    m_snakeSpeedY = 0;

    // Setup food
    m_food = new QGraphicsEllipseItem(QRect(0, 0, 20, 20));
    m_food->setPen(Qt::NoPen);
    m_food->setBrush(QBrush(Qt::darkRed));
    m_food->setPos(rand() % int(m_gameCanvas->rect().width()/20) * 20, rand() % int(m_gameCanvas->rect().height()/20) * 20);
    this->addItem(m_food);

    // Set up animation timer
    m_crashAnimationTimer = new QTimer(this);
    m_crashAnimationTimer->setInterval(40);
    connect(m_crashAnimationTimer, SIGNAL(timeout()), this, SLOT(crashAnimation()));

    // Set up game timer
    m_gameTimer = new QTimer(this);
    m_gameTimer->setInterval(40);
    connect(m_gameTimer, SIGNAL(timeout()), this, SLOT(updateObjects()));
    m_gameTimer->start();
}

void game_scene::reset()
{
	
}

void game_scene::restart()
{
    
}

void game_scene::updateObjects()
{
    // Save the previous head position for the first body part to follow
    QPointF headPrev = QPointF(m_snakeHead->scenePos().x() , m_snakeHead->scenePos().y());

    // Move snake head
    if(m_snakeHead->scenePos().x() >= 0 && m_snakeHead->scenePos().x() <= 1460 &&
        m_snakeHead->scenePos().y() >= 0 && m_snakeHead->scenePos().y() <= 700)
        m_snakeHead->setPos(m_snakeHead->scenePos().x() + m_snakeHead->snakeSpeedX() * 20,
                           m_snakeHead->scenePos().y() + m_snakeHead->snakeSpeedY() * 20);

    // Corrections
    if(m_snakeHead->scenePos().x() < 0)
        m_snakeHead->setPos(1460, m_snakeHead->scenePos().y());
    if(m_snakeHead->scenePos().x() > 1460)
        m_snakeHead->setPos(0, m_snakeHead->scenePos().y());
    if(m_snakeHead->scenePos().y() < 0)
        m_snakeHead->setPos(m_snakeHead->scenePos().x(), 700);
    if(m_snakeHead->scenePos().y() > 700)
        m_snakeHead->setPos(m_snakeHead->scenePos().x(), 0);

    // Collision with food
    if(m_snakeHead->scenePos().x() == m_food->scenePos().x() && m_snakeHead->scenePos().y() == m_food->scenePos().y())
    {
        // Spawn the food
        spawnFood();

        // Make snake longer
        for(int i = 0; i < 5; i++)
        {
            SnakePart *snakePart = new SnakePart(QRectF(0, 0, 20, 20));

            // If first body part, go behind head otherwise go behind last body part
            if(m_snakeBody.length() > 0)
                snakePart->setPos(m_snakeBody[m_snakeBody.length() - 1]->scenePos().x(), m_snakeBody[m_snakeBody.length() - 1]->scenePos().y());
            else
            {
                if(m_snakeHead->snakeSpeedX() == 1 && m_snakeHead->snakeSpeedY() == 0) // Heading Right
                    snakePart->setPos(m_snakeHead->scenePos().x() - 21, m_snakeHead->scenePos().y());
                else if(m_snakeHead->snakeSpeedX() == -1 && m_snakeHead->snakeSpeedY() == 0) // Heading Left
                    snakePart->setPos(m_snakeHead->scenePos().x() + 21, m_snakeHead->scenePos().y());
                else if(m_snakeHead->snakeSpeedX() == 0 && m_snakeHead->snakeSpeedY() == 1) // Heading Down
                    snakePart->setPos(m_snakeHead->scenePos().x(), m_snakeHead->scenePos().y() + 21);
                else if(m_snakeHead->snakeSpeedX() == 0 && m_snakeHead->snakeSpeedY() == -1) // Heading Up
                    snakePart->setPos(m_snakeHead->scenePos().x(), m_snakeHead->scenePos().y() - 21);
            }
            m_snakeBody.append(snakePart);

            snakePart->setPen(QPen(Qt::NoPen));
            snakePart->setBrush(QBrush(QColor("#141414")));
            this->addItem(snakePart);
        }
    }

    // Collision with self
    for(int i = 0; i < m_snakeBody.length(); i++)
    {
        if(m_snakeHead->scenePos().x() == m_snakeBody[i]->scenePos().x() && m_snakeHead->scenePos().y() == m_snakeBody[i]->scenePos().y())
        {
            stop();
            // Give all body parts a direction to move
            /*qreal angleOffset = 360.0/m_snakeBody.length();
            for(int i = 0; i < m_snakeBody.length(); i++)
            {
                m_snakeBody[i]->setCrashDX(cos(angleOffset * i) * 10);
                m_snakeBody[i]->setCrashDY(sin(angleOffset * i) * 10);
            }
            m_crashAnimationTimer->start();*/
        }
    }

    // Move snake body parts
    for(int i = 0; i < m_snakeBody.length(); i++)
    {
        // If first body part, follow the head
        if(i == 0)
            m_snakeBody[0]->setNextPos(headPrev);

        // If second or later body part, floow the body part in front
        if(i + 1 < m_snakeBody.length())
            m_snakeBody[i + 1]->setNextPos(m_snakeBody[i]->scenePos());

        m_snakeBody[i]->setPos(m_snakeBody[i]->getNextPos());
    }

    this->update();
    m_view->update();
}

void game_scene::spawnFood()
{
    int x = rand() % int(m_gameCanvas->rect().width()/20) * 20;
    int y = rand() % int(m_gameCanvas->rect().height()/20) * 20;
    m_food->setPos(x, y);
}

void game_scene::start()
{
    if(!m_gameTimer->isActive())
        m_gameTimer->start();
}

void game_scene::stop()
{
    if(m_gameTimer->isActive())
        m_gameTimer->stop();
}

void game_scene::scored()
{
    
}

void game_scene::crashAnimation()
{
    for(int i = 0; i < m_snakeBody.length(); i++)
    {
        // Rotate part
        m_snakeBody[i]->setRotation(m_snakeBody[i]->rotation() + 15);

        // Move part
        m_snakeBody[i]->setPos(m_snakeBody[i]->scenePos().x() + m_snakeBody[i]->crashDX(),
                               m_snakeBody[i]->scenePos().y() + m_snakeBody[i]->crashDY());

        // If a body part wondered too far off the screen, bring it back
        if(m_snakeBody[i]->scenePos().x() < 0)
            m_snakeBody[i]->setPos(1460, m_snakeHead->scenePos().y());
        if(m_snakeBody[i]->scenePos().x() > 1460)
            m_snakeBody[i]->setPos(0, m_snakeHead->scenePos().y());
        if(m_snakeBody[i]->scenePos().y() < 0)
            m_snakeBody[i]->setPos(m_snakeHead->scenePos().x(), 700);
        if(m_snakeHead->scenePos().y() > 700)
            m_snakeBody[i]->setPos(m_snakeHead->scenePos().x(), 0);

        // If one body part goes beyond the screen, stop animation
        if(m_snakeBody[i]->scenePos().x() <= 0 || m_snakeBody[i]->scenePos().x() >= 1460 ||
            m_snakeBody[i]->scenePos().y() <= 0 || m_snakeBody[i]->scenePos().y() >= 700)
        {
            // Remove parts that float off the screen
            this->removeItem(m_snakeBody[i]);
            m_snakeBody.remove(i);

            // If all parts have been removed, stop the animation
            if(m_snakeBody.length() == 0)
            {
                m_crashAnimationTimer->stop();
                this->addText("Game Over", QFont("Segoe UI", 30));
            }
        }
    }
}

void game_scene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Up && !(m_snakeHead->snakeSpeedY() == 1))
        m_snakeHead->setSnakeSpeed(0, -1);
    else if(event->key() == Qt::Key_Down && !(m_snakeHead->snakeSpeedY() == -1))
        m_snakeHead->setSnakeSpeed(0, 1);
    else if(event->key() == Qt::Key_Left && !(m_snakeHead->snakeSpeedX() == 1))
        m_snakeHead->setSnakeSpeed(-1, 0);
    else if(event->key() == Qt::Key_Right && !(m_snakeHead->snakeSpeedX() == -1))
        m_snakeHead->setSnakeSpeed(1, 0);
}
