#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <vector>
#include <QTimer>
#include <QElapsedTimer>

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

private slots:
    void on_generateButton_clicked();
    void on_startButton_clicked();
    void on_pauseButton_clicked();
    void bubbleSortStep();
    void on_resetButton_clicked();
    void sortStep();
    void selectionSortStep();
    void insertionSortStep();
    void mergeSortStep();

private:
    QTimer *timer;
    int animationDelay = 40;
    QElapsedTimer elapsedTimer;
    QString currentAlgorithm = "Bubble Sort";
    int i = 0;
    int j = 0;
    int minIndex = 0;
    int keyValue = 0;
    bool insertionStarted = false;
    bool sorting = false;
    int comparisons = 0;
    int swaps = 0;
    bool sortingFinished = false;
    Ui::MainWindow *ui;
    std::vector<int> workingArray;
    struct AnimationStep
    {
        enum Type
        {
            Compare,
            Write
        };

        Type type;
        int index1;
        int index2;
        int value;
    };
    std::vector<AnimationStep> animationSteps;
    int currentAnimationStep = 0;

    QGraphicsScene *scene;
    std::vector<int> array;

    void generateArray();
    void drawArray(int current = -1, int compare = -1);

    void finishSorting();
    void resetStatistics();
    void disableControls();
    void enableControls();
    void mergeSort(int left, int right);
    void merge(int left, int mid, int right);

};

#endif