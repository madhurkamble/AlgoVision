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
    ui->pauseButton->setEnabled(false);
    connect(ui->speedSlider, &QSlider::valueChanged, this, [=](int value)
            {
                animationDelay = 510 - (value * 5);

        if (timer->isActive())
            timer->start(animationDelay);
            });
    showMaximized();
    scene = new QGraphicsScene(this);
    timer = new QTimer(this);
    connect(timer,
            &QTimer::timeout,
            this,
            &MainWindow::sortStep);

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

        else if(sortingFinished)
            color = Qt::green;

        else if(i >= array.size() - this->i)
            color = Qt::green;

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
    currentAlgorithm = ui->algorithmComboBox->currentText();
    sortingFinished = false;
    if(currentAlgorithm == "Insertion Sort")
    {
        i = 1;
    }

    else
    {
        i = 0;
    }

    if(currentAlgorithm == "Merge Sort")
    {
        animationSteps.clear();
        currentAnimationStep = 0;

        workingArray = array;

        mergeSort(0, workingArray.size() - 1);
    }

    if(currentAlgorithm == "Quick Sort")
    {
        animationSteps.clear();
        currentAnimationStep = 0;

        workingArray = array;

        quickSort(0, workingArray.size() - 1);
    }

    if(currentAlgorithm == "Heap Sort")
    {
        animationSteps.clear();
        currentAnimationStep = 0;

        workingArray = array;

        heapSort();
    }

    j = 0;
    minIndex = 0;
    keyValue = 0;
    insertionStarted = false;
    i = 0;
    j = 0;
    minIndex=0;
    keyValue = 0;
    resetStatistics();

    sorting = true;
    ui->pauseButton->setEnabled(true);

    elapsedTimer.restart();

    disableControls();

    timer->start(animationDelay);
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
        timer->start(animationDelay);
        ui->pauseButton->setText("Pause");
    }
}

void MainWindow::bubbleSortStep()
{

    ui->timeLabel->setText(
        "Time : " +
        QString::number(elapsedTimer.elapsed()) +
        " ms");

    if (i >= array.size() - 1)
    {
        finishSorting();

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

void MainWindow::on_resetButton_clicked()
{
    timer->stop();

    sorting = false;
    sortingFinished = false;

    i = 0;
    j = 0;

    resetStatistics();

    ui->pauseButton->setText("Pause");

    ui->pauseButton->setText("Pause");

    ui->pauseButton->setEnabled(false);

    generateArray();
    drawArray();

    enableControls();
}


void MainWindow::sortStep()
{
    if(currentAlgorithm == "Bubble Sort")
    {
        bubbleSortStep();
    }
    else if(currentAlgorithm == "Selection Sort")
    {
        selectionSortStep();
    }
    else if(currentAlgorithm == "Insertion Sort")
    {
        insertionSortStep();
    }
    else if(currentAlgorithm == "Merge Sort")
    {
        mergeSortStep();
    }
    else if(currentAlgorithm == "Quick Sort")
    {
        quickSortStep();
    }
    else if(currentAlgorithm == "Heap Sort")
    {
        heapSortStep();
    }
}

void MainWindow::selectionSortStep()
{

    ui->timeLabel->setText(
        "Time : " +
        QString::number(elapsedTimer.elapsed()) +
        " ms");
    if (i >= array.size() - 1)
    {
        finishSorting();

        return;
    }

    if (j == 0)
    {
        minIndex = i;
        j = i + 1;
    }

    if (j < array.size())
    {
        comparisons++;

        ui->comparisonLabel->setText(
            "Comparisons : " + QString::number(comparisons));

        if (array[j] < array[minIndex])
        {
            minIndex = j;
        }

        drawArray(minIndex, j);

        j++;
    }
    else
    {
        if (minIndex != i)
        {
            swaps++;

            ui->swapLabel->setText(
                "Swaps : " + QString::number(swaps));

            std::swap(array[i], array[minIndex]);
        }

        drawArray();

        i++;
        j = 0;
    }
}

void MainWindow::resetStatistics()
{
    comparisons = 0;
    swaps = 0;

    ui->comparisonLabel->setText("Comparisons : 0");
    ui->swapLabel->setText("Swaps : 0");
    ui->timeLabel->setText("Time : 0 ms");
}

void MainWindow::disableControls()
{
    ui->startButton->setEnabled(false);
    ui->generateButton->setEnabled(false);
    ui->arraySizeSpinBox->setEnabled(false);
    ui->algorithmComboBox->setEnabled(false);
    ui->pauseButton->setEnabled(true);
}

void MainWindow::enableControls()
{
    ui->startButton->setEnabled(true);
    ui->generateButton->setEnabled(true);
    ui->arraySizeSpinBox->setEnabled(true);
    ui->algorithmComboBox->setEnabled(true);
    ui->pauseButton->setEnabled(false);
}

void MainWindow::finishSorting()
{
    timer->stop();

    sorting = false;
    sortingFinished = true;
    drawArray();

    ui->timeLabel->setText(
        "Time : " +
        QString::number(elapsedTimer.elapsed()) +
        " ms");

    enableControls();
}

void MainWindow::insertionSortStep()
{

    ui->timeLabel->setText(
        "Time : " +
        QString::number(elapsedTimer.elapsed()) +
        " ms");
    if (i >= array.size())
    {
        finishSorting();
        return;
    }

    if (!insertionStarted)
    {
        keyValue = array[i];
        j = i - 1;
        insertionStarted = true;
    }

    if (j >= 0)
    {
        comparisons++;

        ui->comparisonLabel->setText(
            "Comparisons : " + QString::number(comparisons));

        if (array[j] > keyValue)
        {
            swaps++;

            ui->swapLabel->setText(
                "Swaps : " + QString::number(swaps));

            array[j + 1] = array[j];

            drawArray(j, j + 1);

            j--;

            return;
        }
    }

    array[j + 1] = keyValue;

    drawArray(j + 1);

    i++;
    insertionStarted = false;
    if (i >= array.size())
    {
        drawArray();
        finishSorting();
        return;
    }
}

void MainWindow::mergeSort(int left, int right)
{
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;

    mergeSort(left, mid);
    mergeSort(mid + 1, right);

    merge(left, mid, right);
}

void MainWindow::merge(int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> leftArray(n1);
    std::vector<int> rightArray(n2);

    for (int i = 0; i < n1; i++)
        leftArray[i] = workingArray[left + i];

    for (int j = 0; j < n2; j++)
        rightArray[j] = workingArray[mid + 1 + j];

    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2)
    {
        animationSteps.push_back(
            {AnimationStep::Compare, left + i, mid + 1 + j, 0});

        if (leftArray[i] <= rightArray[j])
        {
            workingArray[k] = leftArray[i];

            animationSteps.push_back(
                {AnimationStep::Write, k, -1, leftArray[i]});

            i++;
        }
        else
        {
            workingArray[k] = rightArray[j];

            animationSteps.push_back(
                {AnimationStep::Write, k, -1, rightArray[j]});

            j++;
        }

        k++;
    }

    while (i < n1)
    {
        workingArray[k] = leftArray[i];

        animationSteps.push_back(
            {AnimationStep::Write, k, -1, leftArray[i]});

        i++;
        k++;
    }

    while (j < n2)
    {
        workingArray[k] = rightArray[j];

        animationSteps.push_back(
            {AnimationStep::Write, k, -1, rightArray[j]});

        j++;
        k++;
    }
}

void MainWindow::mergeSortStep()
{
    playAnimationStep();
}

void MainWindow::quickSort(int low, int high)
{
    if (low < high)
    {
        int pivotIndex = partition(low, high);

        quickSort(low, pivotIndex - 1);
        quickSort(pivotIndex + 1, high);
    }
}

int MainWindow::partition(int low, int high)
{
    int pivot = workingArray[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        animationSteps.push_back(
            {AnimationStep::Compare, j, high, 0});

        if (workingArray[j] < pivot)
        {
            i++;

            std::swap(workingArray[i], workingArray[j]);

            animationSteps.push_back(
                {AnimationStep::Write, i, -1, workingArray[i]});

            animationSteps.push_back(
                {AnimationStep::Write, j, -1, workingArray[j]});
        }
    }

    std::swap(workingArray[i + 1], workingArray[high]);

    animationSteps.push_back(
        {AnimationStep::Write, i + 1, -1, workingArray[i + 1]});

    animationSteps.push_back(
        {AnimationStep::Write, high, -1, workingArray[high]});

    return i + 1;
}

void MainWindow::quickSortStep()
{
    playAnimationStep();
}

void MainWindow::playAnimationStep()
{

    ui->timeLabel->setText(
        "Time : " +
        QString::number(elapsedTimer.elapsed()) +
        " ms");

    if (currentAnimationStep >= animationSteps.size())
    {
        drawArray();
        finishSorting();
        return;
    }

    AnimationStep step = animationSteps[currentAnimationStep];

    if (step.type == AnimationStep::Compare)
    {
        comparisons++;

        ui->comparisonLabel->setText(
            "Comparisons : " + QString::number(comparisons));

        drawArray(step.index1, step.index2);
    }
    else if (step.type == AnimationStep::Write)
    {
        swaps++;

        ui->swapLabel->setText(
            "Swaps : " + QString::number(swaps));

        array[step.index1] = step.value;

        drawArray(step.index1);
    }

    currentAnimationStep++;
}

void MainWindow::heapify(int n, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if(left < n)
    {
        animationSteps.push_back(
            {AnimationStep::Compare, left, largest, 0});

        if(workingArray[left] > workingArray[largest])
            largest = left;
    }

    if(right < n)
    {
        animationSteps.push_back(
            {AnimationStep::Compare, right, largest, 0});

        if(workingArray[right] > workingArray[largest])
            largest = right;
    }

    if(largest != i)
    {
        std::swap(workingArray[i], workingArray[largest]);

        animationSteps.push_back(
            {AnimationStep::Write, i, -1, workingArray[i]});

        animationSteps.push_back(
            {AnimationStep::Write, largest, -1, workingArray[largest]});

        heapify(n, largest);
    }
}

void MainWindow::heapSort()
{
    int n = workingArray.size();

    for(int i = n / 2 - 1; i >= 0; i--)
    {
        heapify(n, i);
    }

    for(int i = n - 1; i > 0; i--)
    {
        std::swap(workingArray[0], workingArray[i]);

        animationSteps.push_back(
            {AnimationStep::Write, 0, -1, workingArray[0]});

        animationSteps.push_back(
            {AnimationStep::Write, i, -1, workingArray[i]});

        heapify(i, 0);
    }
}

void MainWindow::heapSortStep()
{
    playAnimationStep();
}

MainWindow::~MainWindow()
{
    delete ui;
}

