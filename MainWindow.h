#ifndef BOGGLEWINDOW_H
#define BOGGLEWINDOW_H

#include "Console.h"
#include <QMouseEvent>
#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //void mousePressEvent(QMouseEvent *event){}
    //void keyPressEvent(QKeyEvent *event);
    Console *console;
    static const int BOGGLE_WINDOW_WIDTH = 800;
    static const int BOGGLE_WINDOW_HEIGHT = 600;
};

#endif // BOGGLEWINDOW_H
