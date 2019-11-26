#include "MainWindow.h"
#include <QFile>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("BASIC");
    this->setFixedSize(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);

    console = new Console(this);

    console->setGeometry(30, 320, 740, 260);
    //console->write("Welcome to BASIC!");
}


MainWindow::~MainWindow()
{
}
