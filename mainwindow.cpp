#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    timer = new QTimer(this);
    connect(timer,
            &QTimer::timeout,
            this,
            &MainWindow::bubbleSortStep);

    ui->graphicsView->setScene(scene);

    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    ui->graphicsView->setSceneRect(0,0,1000,450);
}

#include <cstdlib>
#include <ctime>

void MainWindow::generateArray()
{
    array.clear();

    srand(time(nullptr));

    int size = ui->arraySizeSpinBox->value();

    for(int i = 0; i < size; i++)
    {
        array.push_back(rand() % 350 + 30);
    }

    drawArray();
}

void MainWindow::drawArray(int current, int compare)
{
    scene->clear();

    int sceneWidth = ui->graphicsView->viewport()->width();
    int sceneHeight = ui->graphicsView->viewport()->height();

    int n = array.size();

    if(n == 0)
        return;

    int barWidth = sceneWidth / n;

    for(int i = 0; i < n; i++)
    {
        QColor color(0,170,255);

        if(i == current)
            color = Qt::red;

        else if(i == compare)
            color = Qt::yellow;

        scene->addRect(
            i * barWidth,
            sceneHeight - array[i],
            barWidth - 4,
            array[i],
            QPen(Qt::white),
            QBrush(color)
            );
    }
}

void MainWindow::on_generateButton_clicked()
{
    generateArray();
    drawArray(2,5);
}

void MainWindow::on_startButton_clicked()
{
    i = 0;
    j = 0;

    sorting = true;

    timer->start(250);
}

void MainWindow::bubbleSortStep()
{
    if (i >= array.size() - 1)
    {
        timer->stop();
        return;
    }

    if (j >= array.size() - i - 1)
    {
        j = 0;
        drawArray();
        i++;
        return;
    }

    if (array[j] > array[j + 1])
    {
        std::swap(array[j], array[j + 1]);
    }

    drawArray(j, j + 1);

    j++;
}

MainWindow::~MainWindow()
{
    delete ui;
}
