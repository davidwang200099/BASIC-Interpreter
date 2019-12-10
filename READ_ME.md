# Project 2: Minimal BASIC Interpreter
## Abstract

​    This interpreter is used to interpret and run codes written in BASIC. The user can input BASIC code,run code and store the input codes to a file.User can also run the codes in interactive mode.

## Structure of Source Files

1. evaluate_rule.h:defines basic rules for evaluation of expression,including priority of operators.

2. evaluate.h & evaluate.cpp:evaluate expressions composed of variables and constants according to the rules defined in evaluate_rule.h.

3. namedvariable.h & namedvariable.cpp:defines a data structure for variables defined in BASIC code.

4. linecode.h & linecode.cpp :defines a data structure used to store a line of code in input BASIC codes or BASIC source files.

5. coderunner.h & coderunner.cpp :The core component of this interpreter.It can store BASIC codes,maintain the variables appearing in the codes and run the codes stored in itself.

6. Console.h & Console.cpp:Used for user input.The user input the order or codes to the console and coderunner will be used to analyse the input.There is strong reaction between Console and Coderunner.

7. MainWindow.h & MainWindow.cpp :The main window of the application.

## Tpes of Statements in BASIC

1. INPUT:ask user to input the value of a viariable

2. LET:assign a value to a variable,either a new one of an existing one.

3. PRINT:print the value of an expression,a value or just a constant.Then the cursor will go to a new line.

4. GOTO:jump to another line n and continue to run from line n.

5. IF:examine the condition expression and execute the statement after "THEN" if the condition is true.

6. END:marks the end of a BASIC program

7. REM:the content after REM will be ignored.

8. Sub:marks the beginning of a function.A function name is required after "Sub" in the same line.The function must be ended with a lind of code:"End Sub".

## Acceptable order to the interpreter

1. "RUN":ask the coderunner to run the codes stored buffer.
2. "LIST":ask the coderunner to list the codes stored in the buffer to the console.If there is no code stored in the buffer,it will give out a notice.
3. "CLEAR":clear the buffer so that the user can input new programs.
4. "QUIT":quit by invoking "exit(0)".
5. "HELP": provides a simple help message describing the interpreter.
6. "LOAD":load a program from a file.Filename is requred.e.g. "LOAD TEST"
7. "WRITE":write the program stored in the buffer to a file.file name is optional.If filename is not assigned,program will be writter to "out.vb".
8. "\KILL":force the interpreter to end the execution of a program.

## Using the Interpreter

### in an Interactive Way

1. User can input codes without a line number.

   ```basic
   LET A=4
   LET B=3
   INPUT C
   PRINT A+B+C
   ```

   BASIC interpreter will run the codes rightly after the user completes the input of each line by typing 'Enter'."LET","INPUT" and "PRINT" statements will be executed immediately.Other kinds of code will be valid only with a code number in the front.But remember:The code input in interactive way will not be stored!
   
## in a Storing Way

```basic
100 REM THIS IS A COMPREHENSIVE TEST
110 Sub COMP
120 IF A>B THEN LET C=1
130 IF A<B THEN LET C=2
140 IF A=B THEN LET C=0
150 End Sub
160 Sub GCD
170 INPUT A
180 INPUT B
190 CALL COMP
200 IF C=0 THEN GOTO 260
210 IF C=2 THEN GOTO 240
220 LET A=A-B
230 GOTO 190
240 LET B=B-A
250 GOTO 190
260 LET RESULT=A
270 End Sub
280 REM THIS IS MAIN FUNCTION
290 CALL GCD
300 PRINT RESULT
310 END
```

User can input these codes into the console.The coderunner will store these lines of codes into the buffer.Codes are stored and sorted by their line number.The user can input "RUN" to run the program.Coderunner may ask the user to input the value of the variables to be assigned in the program.The program will input the GCD of A and B.
### Switching between Two Modes

User can switch between the two modes.When switching from interactive mode to storing mode,the initialized variable will be cleared only if the user input "RUN" to run the code stored in the buffer.

### Running Stored Programs

 After the user input "RUN" to run the program stored in the input,codes will just be interpreted by the interpreter and executed  one after another.If the codes are grammatically right,The execution will terminate with return value 0.If the interpreter meet with an error,The execution will terminate at once with a return value -1.If the user force the interpreter to stop running the program by inputting "\KILL",the interpreter will terminate the execution with a return value 1.

When running the program,the interpreter may pause and wait for user to input the value.But the interpreter will not remind user.So user need to know when to input the value.

## Design of Data Structures

1. Evaluate of Expressions:

   The interpreter accept a string as an expression.The interpreter use a stack of operators and a stack of operated numbers to analyse the priority of each step of calculation.The priority of operators is given in "evaluate_rule.h".The interpreter read characters in the expression string,figure out whether they are constant numbers,variables or operators.The interpreter also defines a struct of "Calcresult",whose components are a boolean variable showing whether the evaluation is valid,and an integer variable showing the value.If the evaluation is valid,The interpreter returns the value of the expression.If not,it will return a constant of Calcresult:Calcresult(false,INT_MIN),also defined as EVALUATE_ERROR by macro.It means the interpret meets an invalid expression.The execution of either one single line of code or program made up of many lines of code will be forced to terminate.

2. NamedVar:

   When meeting a viariable named such as "A","RESULT" etc. ,interpreter will create a NamedVar to describe it.A NamedVar includes name of variable as well as its value.

3. Linecode:

   A single line of BASIC code will be stored in a structure called Linecode.It can maintain the line number and The Linecode will analyse its type when its constructor is invoked.
   
4. Coderunner:

   ​    The design of Coderunner is enlightened by what we have learnt in Introduction to Computer Systems.
   ​    The coderunner has :
   A Vector of NamedVar to store the variables and their value. 
   
   A Vector of Linecode to store the code. A pointer to Console to interact with.
   
   A integer 'currentLine' to show the line number of the code to run,just like %rip.
   
   A interger 'currentVar' to show which of the named variables should be asigned the value input from the Console.
   
   An enum variable 'runnermode' to show whether the runner is waiting for code input or input for the value of a viariable.
   
   A stack of int 'Runtime_stack' to be used during function call.Because of the requirement of Project2,global namespace is enough.Therefore its use is limited——It can only used to store the "return address".
   
5. Console:

   Console is a subclass of QTestEdit.It has a Coderunner * variable pointing to the Coderuuner with which to interact.The Console can receive the user input,process it in a simple way and pass it to the coderunner to completely execute.Some message to be sent to the user will also be shown in the Console.

6. MainWindow:

   MainWindow is the carrier of Console.When user invoke the interpreter,a MainWindow will be generated.
   

## Summary

​    This project aims to give us a chance to find out how programming languages work.By working on the project,We learned about the job of an interpreter,improved our command of data structures and ability of engineering.The bonus task of "function call" also gives us a chance to put what we learn in ICS into practice,though a very naive one.This project also makes me become interested in Compilers:Principles and Technologies.I hope I will be lucky enough to take this class in the near future.And Thanks for instructor 's and teaching assistants' help!