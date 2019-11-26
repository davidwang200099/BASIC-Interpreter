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
        //qDebug()<<"Key return.\n";
        QTextCursor cursor = this->textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor.select(QTextCursor::LineUnderCursor);
        if(runner->runnermode==INPUTCODE){
            string lastLine = cursor.selectedText().toStdString();
            newLineWritten(lastLine);
            processOrder(lastLine);
        }else{
            if(runner->runnermode==INPUTVALUE){
                int value=cursor.selectedText().toInt();
                //qDebug()<<"value:"<<value<<"\n";
                string varname=readVar(runner->codes[runner->currentLine-1].expression);
                NamedVar var(varname);
                Rank r = runner->vars.search(var);
                if(r >= 0) runner->vars[r].value = value;
                else {var.value=value;runner->vars.push_back(var);}
                runner->runnermode=SUCCESSIVERUN;
                if(runner->runCodes()==SeqFinished)
                    runner->runnermode=INPUTCODE;
            }

            //runner->regs.rdi=var;
            //emit newVarInput();
        }
    }
    QTextEdit::keyPressEvent(event);
}

void Console::processOrder(const string &commandline){
    //runner->processOrder(commandline);
    InputType inputtype = runner->processOrder(commandline);
    if(inputtype == QUIT_ORDER) exit(0);
    else {
        if(inputtype == BASIC_CODE) {
            Linecode newcode = Linecode(commandline);

            if(newcode.lineno == INT32_MIN) {
                try{runner->runCode(commandline);}
                catch(int x){
                    write("Execution error of single line of code.\n");
                }
            } else {
                Rank r = runner->codes.binarysearch(newcode);
                if(r>=0&&newcode.lineno==runner->codes[r].lineno) runner->codes[r]=newcode;
                else runner->codes.insert(newcode,r+1);
            }

        }
    }
}

