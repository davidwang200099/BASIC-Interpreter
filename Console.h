#ifndef CONSOLE_H
#define CONSOLE_H

#include <QObject>
#include <QTextEdit>
#include <QWidget>
#include <string>
#include "coderunner.h"
using namespace std;
class Console : public QTextEdit
{
    Q_OBJECT
private:
    Coderunner *runner;
public:
    explicit Console(QWidget *parent = nullptr);
    void processOrder(const string &string);
    void expressWelcome(){write("Welcome to BASIC!\n");}
signals:
    void newLineWritten(string newline);
    void newVarInput();
public slots:
    void clear();
    void write(string msg);

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;

};

#endif // CONSOLE_H
