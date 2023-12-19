#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QTextStream out(stdout);
    out << "---" << touchFood(c);
    act = START;
    ui->setupUi(this);
    setFixedSize(SIZE_MAIN + 300, SIZE_MAIN);
    startTimer(200);
    sizeFood = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent* )
{
    if (t == OVER)
        exit(1);
    QPainter* painter = new QPainter();
    painter->begin(this);
    for(int i = 0; i < 2; i++){
        QRect rec(i, i, SIZE_MAIN-i-5, SIZE_MAIN-i-5);
        painter->drawRect(rec);
    }
    spawnFood(painter);
    paintSnake(painter);
    painter->end();
}

void MainWindow::timerEvent(QTimerEvent* )
{
    repaint();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    if (key == Qt::Key_Left){
        if (turn == RIGHT)
            return;
        turn = LEFT;
    }
    else if (key == Qt::Key_Right){
        if (turn == LEFT)
            return;
        turn = RIGHT;
    }
    else if (key == Qt::Key_Up){
        if (turn == DOWN)
            return;
        turn = UP;
    }
    else if (key == Qt::Key_Down){
        if (turn == UP)
            return;
        turn = DOWN;
    }

    repaint();
}

void MainWindow::spawnFood(QPainter *painter)
{
    if (act == START || t == TOUCH){
        do {
            x_food = qrand()%(SIZE_MAIN - SIZE_SNAKE - 2);
            y_food = qrand()%(SIZE_MAIN - SIZE_SNAKE - 2);
            c.x = x_food;
            c.y = y_food;
        }while(!rightArea(c));
        t = RUN;
    }

    painter->drawEllipse(x_food, y_food, SIZE_SNAKE, SIZE_SNAKE);
}

void MainWindow::paintSnake(QPainter *painter)
{

    selfTouch();
    if (act == START){
        c.x = SIZE_MAIN / 2;
        c.y = c.x;
        body.push_back(c);
        for(int i = 1; i < 4; i++){
            c.x = body[i - 1].x + SIZE_SNAKE;
            body.push_back(c);
            createSquare(painter, body[i].x, body[i].y);
        }

        act = MOVE;
    }

    if (act == MOVE){
        moveSnake(painter);
    }
}

bool MainWindow::rightArea(CaseSnake c)
{
    bool flag;
    for(int i = 0; i < body.size(); i++){
        int x = body[i].x - 2*SIZE_SNAKE;
        int y = body[i].y - 2*SIZE_SNAKE;
        flag = (c.x >= x && c.x <= x + 3*SIZE_SNAKE)
                && (c.y >= y && c.y <= y + 3*SIZE_SNAKE);
        if (flag)
            return false;
    }
    return true;
}

bool MainWindow::touchFood(CaseSnake c)
{
    bool current = false;
    int x = c.x, xF = x_food;
    int y = c.y, yF = y_food;
    if (turn == LEFT){
        xF += SIZE_SNAKE;
        current = (x <= xF && x >= xF - SIZE_SNAKE) &&
                (y >= (yF - SIZE_SNAKE) && y <= (yF + SIZE_SNAKE));
    }
    if (turn == RIGHT){
        x += SIZE_SNAKE;
        current = (x >= xF && x <= xF + SIZE_SNAKE) &&
                (y >= (yF - SIZE_SNAKE) && y <= (yF + SIZE_SNAKE));
    }
    if (turn == UP){
        yF += SIZE_SNAKE;
        current = (y <= yF && y >= yF - SIZE_SNAKE) &&
                (x >= (xF - SIZE_SNAKE) && x <= (xF + SIZE_SNAKE));
    }
    if (turn == DOWN){
        y += SIZE_SNAKE;
        current = (y >= yF && y <= yF + SIZE_SNAKE) &&
                (x >= (xF - SIZE_SNAKE) && x <= (xF + SIZE_SNAKE));
    }
    return current;
}

void MainWindow::selfTouch()
{
    for(int i = 1; i < body.size(); i++){
        if (body[0].x == body[i].x && body[0].y == body[i].y){
            QTextStream out(stdout);
            out << "---" << body[0].x << "___" << body[0].y <<"\n";
            t = OVER;
        }
    }
}

void MainWindow::bordersTouch()
{
    if (body[0].x >= SIZE_MAIN + SIZE_SNAKE){
        exit(1);
    }
    else if (body[0].x <= -2*SIZE_SNAKE){
        exit(1);
    }

    if (body[0].y >= SIZE_MAIN + SIZE_SNAKE){
        exit(1);
    }
    else if (body[0].y <= -2*SIZE_SNAKE){
        exit(1);
    }
}

void MainWindow::moveSnake(QPainter* painter)
{

    c = body[0];
    if (touchFood(c)){
        ui->lcdNumber->display(++sizeFood);
        t = TOUCH;
        CaseSnake c2;
        body.push_back(c2);
    }


    bordersTouch();

    if (turn == LEFT){
        body[0].x -= SIZE_SNAKE;
    }
    if (turn == RIGHT){
        body[0].x += SIZE_SNAKE;
    }
    if (turn == UP){
        body[0].y -= SIZE_SNAKE;
    }
    if (turn == DOWN){
        body[0].y += SIZE_SNAKE;
    }

    for(int i = 1; i < body.size(); i++){
        c1 = body[i];
        body[i] = c;
        c = c1;
        createSquare(painter, body[i].x, body[i].y);
    }
}

void MainWindow::createSquare(QPainter *painter, int x, int y)
{
    QRect rec(x, y, SIZE_SNAKE, SIZE_SNAKE);
    painter->fillRect(rec, "green");
    painter->drawRect(rec);
}


