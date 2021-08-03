#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget() = default;

    void loadNext();
private:
    QList<QList<bool>> board;
    int cellSize;
    int cols;
    int rows;
    int hoverCol = -1;
    int hoverRow = -1;

protected:
    void mouseMoveEvent(QMouseEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    QSize sizeHint() const override;
    void paintEvent(QPaintEvent *) override;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;
private:
    QTimer *timer;
};

#endif // MAINWINDOW_H
