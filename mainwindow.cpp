#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QPainter>
#include <QGraphicsRectItem>
#include <random>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Window settings
    this->setWindowTitle("");

    // Misc
    srand(time(0));

    // Create the layouts
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QHBoxLayout *viewLayout = new QHBoxLayout();
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(viewLayout);

    // Create buttons, scene and view
    QPushButton *btn = new QPushButton("");
    buttonLayout->addWidget(btn);

    m_view = new QGraphicsView(this);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setFrameStyle(QFrame::NoFrame);
    viewLayout->addWidget(m_view);

    m_scene = new game_scene(QRectF(), m_view, this);
    m_view->setScene(m_scene);

    QWidget *dummy = new QWidget(this);
    dummy->setLayout(mainLayout);
    this->setCentralWidget(dummy);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::draw()
{
    m_scene->update();
    m_view->update();
}
