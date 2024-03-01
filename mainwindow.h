#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QtGui>
#include <QtCore>
#include "GraphicsScene.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void draw();

private:
    Ui::MainWindow *ui;

    QGraphicsView *m_view;
    game_scene *m_scene;
    QGraphicsRectItem *m_gameCanvas;
};
#endif // MAINWINDOW_H
