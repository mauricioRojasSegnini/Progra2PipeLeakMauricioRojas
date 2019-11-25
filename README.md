# Progra2PipeLeakMauricioRojas
### **Estudiante:** MauricioRojas
### **carne:** B86981
### **Estudiante:** 
### **carne:** 
this is a repository for a programming project of the course of Programation 2.

The player must build or rebuild a pipe through which a fluid, usually water, will pass through. **The pipe is built on a matrix area**, where fluid arises from one or more sources and must reach one or more destinations.
The program receives the status of a game in progress by standard input or in files. The content in the standard input or in the files could be text or binary. The program **must validate, resolve, or convert the level** it receives according to the user's choice.

The program indicates if there is a leak in the pipe, if nothing is reported, the pipe is not leaking.
The program interprets the entry as textual if the "-it" argument is provided. This argument is considered by default, and therefore, the user could skip it. If a file name is not provided in the command line arguments, the program must assume the state of the game will be provided in the standard input.

The water sources and destinations are always at the edge of the level.
The program evaluates the pipe and indicates if there are leaks.
For each leak detected, a “leak R C D” line must be reported, where R is the row, “C” the column, and “D” the address or cardinal point of the cell where the leak occurs.

Solving a level consists in finding the state of the pipe that allows the fluid to move from the sources to the destinations without leaks. The program must resolve a level when the "solve" action is indicated in the first argument on the command line
