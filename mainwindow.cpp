#include "mainwindow.h"

#define TIMER 50
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setAttribute(Qt::WA_AlwaysShowToolTips);
    timer = new QTimer;
    timer->setSingleShot(false);
    timer->setInterval(TIMER);
    auto wid = new Widget;
    auto scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setWidget(wid);
    setCentralWidget(scroll);
    auto bar = new QToolBar;
    auto a = bar->addAction("Next");
    connect(a, &QAction::triggered, wid, &Widget::loadNext);
    auto timerCheck = new QCheckBox("Auto");
    bar->addWidget(timerCheck);
    connect(timerCheck, &QCheckBox::toggled, this, [=] (bool checked){
        checked ? timer->start() : timer->stop();
    });
    connect(timer, &QTimer::timeout, wid, &Widget::loadNext);

    auto timerSpin = new QSpinBox;
    timerSpin->setSuffix("ms");
    timerSpin->setMinimum(10);
    timerSpin->setMaximum(2000);
    timerSpin->setSingleStep(10);
    timerSpin->setValue(TIMER);
    bar->addWidget(timerSpin);
    connect(timerSpin, QOverload<int>::of(&QSpinBox::valueChanged), timer, QOverload<int>::of(&QTimer::setInterval));

    addToolBar(bar);
}


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);

    cellSize = 10;
    cols = 200;
    rows = 156;

    for (int col  = 0; col < cols; ++col) {
//        QList<bool> rowList;
        board.append(QList<bool>());
        for (int row = 0; row < rows; ++row) {
            board[col].append(false);
        }
    }
//    https://conwaylife.appspot.com/pattern/acorn
//    board[141][70] = true;
//    board[140][72] = true;
//    board[141][72] = true;
//    board[143][71] = true;
//    board[144][72] = true;
//    board[145][72] = true;
//    board[146][72] = true;

    https://www.conwaylife.com/w/images/9/9f/Gosperglidergun.png
    board[52][26] = true;
    board[52][27] = true;
    board[53][26] = true;
    board[53][27] = true;

    board[62][26] = true;
    board[62][27] = true;
    board[62][28] = true;

    board[63][25] = true;
    board[63][29] = true;

    board[64][24] = true;
    board[64][30] = true;
    board[65][24] = true;
    board[65][30] = true;

    board[66][27] = true;

    board[67][25] = true;
    board[67][29] = true;

    board[68][26] = true;
    board[68][27] = true;
    board[68][28] = true;

    board[69][27] = true;

    board[72][24] = true;
    board[72][25] = true;
    board[72][26] = true;
    board[73][24] = true;
    board[73][25] = true;
    board[73][26] = true;

    board[74][23] = true;
    board[74][27] = true;

    board[76][22] = true;
    board[76][23] = true;
    board[76][27] = true;
    board[76][28] = true;

    board[86][24] = true;
    board[86][25] = true;
    board[87][24] = true;
    board[87][25] = true;

    //reversed
    board[148][36] = true;
    board[148][37] = true;
    board[147][36] = true;
    board[147][37] = true;

    board[138][36] = true;
    board[138][37] = true;
    board[138][38] = true;

    board[137][35] = true;
    board[137][39] = true;

    board[136][34] = true;
    board[136][40] = true;
    board[135][34] = true;
    board[135][40] = true;

    board[134][37] = true;

    board[133][35] = true;
    board[133][39] = true;

    board[132][36] = true;
    board[132][37] = true;
    board[132][38] = true;

    board[131][37] = true;

    board[128][34] = true;
    board[128][35] = true;
    board[128][36] = true;
    board[127][34] = true;
    board[127][35] = true;
    board[127][36] = true;

    board[126][33] = true;
    board[126][37] = true;

    board[124][32] = true;
    board[124][33] = true;
    board[124][37] = true;
    board[124][38] = true;

    board[114][34] = true;
    board[114][35] = true;
    board[113][34] = true;
    board[113][35] = true;


    setFixedSize(QSize(cols * cellSize + 1, rows * cellSize + 1));
}

void Widget::loadNext()
{
    QList<QList<bool>> newBoard;
    for (int col  = 0; col < board.count(); ++col) {
        newBoard.append(QList<bool>());
        for (int row = 0; row < board[col].count(); ++row) {
            int count = 0;
            bool alive = board[col][row];
            count += (col == 0 || row == 0) ? false : board[col-1][row-1];
            count += (col == 0) ? false : board[col-1][row];
            count += (col == 0 || row == board[col].count() - 1) ? false : board[col-1][row+1];
            count += (row == 0) ? false : board[col][row-1];
            count += (row == board[col].count() - 1) ? false : board[col][row+1];
            count += (col == board.count() - 1 || row == 0) ? false : board[col+1][row-1];
            count += (col == board.count() - 1) ? false : board[col+1][row];
            count += (col == board.count() - 1 || row == board[col].count() - 1) ? false : board[col+1][row+1];
//            Births: Each dead cell adjacent to exactly three live neighbors will become live in the next generation.
//            Death by isolation: Each live cell with one or fewer live neighbors will die in the next generation.
//            Death by overcrowding: Each live cell with four or more live neighbors will die in the next generation.
//            Survival: Each live cell with either two or three live neighbors will remain alive for the next generation.
            if (!alive && count == 3)
                alive = true;
            else if (alive && count <= 1)
                alive = false;
            else if (alive && count >= 4)
                alive = false;
            else if (alive && (count == 2 || count == 3))
                alive = true;
            else
                alive = false;
            newBoard[col].append(alive);
        }
    }
    board = newBoard;
    update();
}

void Widget::mouseMoveEvent(QMouseEvent *e)
{
//    qDebug() << "mousemove";
    e->accept();
    int x = e->pos().x();
    int y = e->pos().y();
    int col = x / cellSize;
    int row = y / cellSize;
    if (col < board.count() - 1) {
        if (row < board[col].count() - 1) {

            QString text = QString::number(row) + "," + QString::number(col) + (board[col][row] ? " - Alive" : " - Dead");
            hoverCol = col;
            hoverRow = row;
            QToolTip::showText(e->globalPos(), text);
            update();
        }
    }
}

void Widget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        e->accept();
        int x = e->pos().x();
        int y = e->pos().y();
        int col = x / cellSize;
        int row = y / cellSize;
        qDebug() << "x" << x << "y" << y;
        if (col < board.count() - 1) {
            if (row < board[col].count() - 1) {
                qDebug() << "cell" << col << row << board[col][row];
                board[col][row] = !board[col][row];
                update();
            }
        }
    }
}

QSize Widget::sizeHint() const
{
    return QSize(cols * cellSize + 1, rows * cellSize + 1);
}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter p(this);


    QColor green(0,200,0);
    QColor dark(50,50,50);
    QColor white(255,255,255);
    p.setPen(white);

    for (int col  = 0; col < board.count(); ++col) {
        for (int row = 0; row < board[col].count(); ++row) {

            bool alive = board[col][row];
            int x = col * cellSize;
            int y = row * cellSize;

            p.fillRect(QRect(x+1, y+1, cellSize-1, cellSize-1),alive ? green : dark);
            if (hoverCol == col && hoverRow == row) {
                p.drawRect(QRect(x, y, cellSize, cellSize));
            }

        }
    }
}

