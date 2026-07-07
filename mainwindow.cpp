#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    showMaximized();
    scene = new QGraphicsScene(this);
    timer = new QTimer(this);
    connect(timer,
            &QTimer::timeout,
            this,
            &MainWindow::bubbleSortStep);

    ui->graphicsView->setScene(scene);

    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    scene->setSceneRect(0, 0,
                        ui->graphicsView->width(),
                        ui->graphicsView->height());

    ui->graphicsView->setScene(scene);
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

    QRectF rect = scene->sceneRect();

    int sceneWidth = rect.width();
    int sceneHeight = rect.height();

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

        else if(i >= array.size() - this->i)
            color=Qt::green;

        int maxValue = *std::max_element(array.begin(), array.end());

        int barHeight = (array[i] * sceneHeight) / maxValue;

        scene->addRect(
            i * barWidth,
            sceneHeight - barHeight,
            barWidth - 4,
            barHeight,
            QPen(Qt::white),
            QBrush(color)
            );
    }
}

void MainWindow::on_generateButton_clicked()
{
    generateArray();
    drawArray();
    comparisons = 0;
    swaps = 0;

    ui->comparisonLabel->setText("Comparisons : 0");
    ui->swapLabel->setText("Swaps : 0");
}

void MainWindow::on_startButton_clicked()
{
    i = 0;
    j = 0;

    sorting = true;

    timer->start(ui->speedSlider->value());
}

void MainWindow::on_pauseButton_clicked()
{
    if(timer->isActive())
    {
        timer->stop();
        ui->pauseButton->setText("Resume");
    }
    else
    {
        timer->start(40);
        ui->pauseButton->setText("Pause");
    }
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

    comparisons++;

    ui->comparisonLabel->setText(
        "Comparisons : " + QString::number(comparisons));

    if (array[j] > array[j + 1])
    {
        swaps++;

        ui->swapLabel->setText(
            "Swaps : " + QString::number(swaps));

        std::swap(array[j],array[j+1]);

    }

    drawArray(j, j + 1);

    j++;
}

MainWindow::~MainWindow()
{
    delete ui;
}
