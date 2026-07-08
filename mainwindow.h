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

private:
    QTimer *timer;
    int animationDelay = 40;
    QElapsedTimer elapsedTimer;
    QString currentAlgorithm = "Bubble Sort";
    int i = 0;
    int j = 0;
    int minIndex = 0;

    bool sorting = false;
    int comparisons = 0;
    int swaps = 0;
    Ui::MainWindow *ui;

    QGraphicsScene *scene;
    std::vector<int> array;

    void generateArray();
    void drawArray(int current = -1, int compare = -1);

    void finishSorting();
    void resetStatistics();
    void disableControls();
    void enableControls();
};

#endif