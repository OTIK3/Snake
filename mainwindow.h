#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTimer>
#include <QTimerEvent>
#include <QTextStream>
#include <QList>
#include <QRect>
#include <QHash>

enum Action{
    START, MOVE, TOUCH, RUN,
    LEFT, RIGHT, UP, DOWN, OVER
};



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
    void keyPressEvent(QKeyEvent* event);

private:
    Ui::MainWindow *ui;

    const int SIZE_MAIN = 500;
    const int SIZE_SNAKE = 10;
    const int MAX_SIZE = 200;

    struct CaseSnake{
        int x;
        int y;
    };

    Action act = START;
    Action t = RUN;
    Action turn = LEFT;

    CaseSnake c;
    CaseSnake c1;
    QList<CaseSnake> body;

    int x_food;
    int y_food;
    int sizeFood = 0;

    void spawnFood(QPainter *painter);
    void paintSnake(QPainter *painter);
    void moveSnake(QPainter *painter);
    void createSquare(QPainter *painter, int x, int y);

    bool rightArea(CaseSnake c);
    bool rightTurn(Action turn1);
    bool touchFood(CaseSnake c);
    void turnSnake();
    void selfTouch();
    void bordersTouch();
};
#endif // MAINWINDOW_H
