#include "Console.h"
#include <QKeyEvent>
#include <QTextLine>
#include <QTextCursor>
#include <QDebug>
#include "MainWindow.h"
Console::Console(QWidget *parent) : QTextEdit(parent),runner(new Coderunner(this))
{expressWelcome();

}

void Console::clear()
{
    QTextEdit::clear();
}

void Console::write(string msg)
{
    this->append(QString::fromStdString(msg));
}

void Console::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Backspace)
        if(this->textCursor().atBlockStart()) return;
    if (event->key() == Qt::Key_Delete)
        return;
    if (this->textCursor().hasSelection())
        return;
    if (event->key() == Qt::Key_Return) {
        QTextCursor cursor = this->textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor.select(QTextCursor::LineUnderCursor);
        string lastLine = cursor.selectedText().toStdString();
        if(lastLine=="\\KILL") {//user force the execution to terminate
            runner->runnermode=INPUTCODE;
            runner->currentLine=0;
            write("Execution terminated with exit code 1.\n");
        }
        if(runner->runnermode==INPUTCODE){//user input new code
            newLineWritten(lastLine);
            processOrder(lastLine);
        }else{
            if(runner->runnermode==INPUTVALUE){//runner paused for user input
                int value=cursor.selectedText().toInt();
                runner->vars[runner->currentVar].value=value;
                runner->runnermode=SUCCESSIVERUN;
                if(runner->runCodes()!=Pause4UsrInput)
                    runner->runnermode=INPUTCODE;
            }
        }

    }

    QTextEdit::keyPressEvent(event);
}

void Console::processOrder(const string &commandline){
    InputType inputtype = runner->processOrder(commandline);//let runner to analyse
    if(inputtype == QUIT_ORDER) exit(0);//"QUIT"
    else {
        if(inputtype == BASIC_CODE) {
            Linecode newcode = Linecode(commandline);

            if(newcode.lineno == INT32_MIN) {//code without linenumber
                try{
                    runner->runCode(commandline);//execute directly without storing it
                }
                catch(int x){
                    write("Execution error of single line of code.\n");
                }
            } else {
                Rank r = runner->codes.binarysearch(newcode);
                if(r>=0&&newcode.lineno==runner->codes[r].lineno) runner->codes[r]=newcode;
                else runner->codes.insert(newcode,r+1);//store the code
            }
        }
    }
}

