# PES_Assignment_6
PES Assignment 6
PES Assignment 6 Created by: Shashank Chandrasekaran Decription: SerialBreakfast

Following are the .c and .h files created: 

cbfifo.c : Contains the definitions of CBFIFO functions like init, enqueue, dequeue, etc. Assignment 2 code was leveraged and updated to pass structure as a parameter
instead of using a global array. Dean's code was referred to add the primary mask portion to disable and enable interrupts during enqueue and dequeue operations
cbfifo.h : Contains the declaration of CBFIFO functions

cbfifo_test.c : Contains around 600 test cases for CBFIFO called by a test function. Assignment 2a test code was leveraged and updated to add the structure
cbfifo_test.h : Contains the function declaration of the CBFIFO test function

UART.c : Defines the functions related to UART0 like initialization and interrupt handler. Inbuilt function __syswrite and __sysread updated to redirect the data to the UART0 instead of semihost when printf and getchar are called respectively
UART.h: Declares the UART0 and inbuilt functions mentioned in UART.c

hexdump.c : Defines the hexdump fuction that prints the address and 16 bytes of data per line based on the start address and length of bytes entered by user
hexdump.h : Declares the hexdump function

Command_Processor.c : This .C file contains the definitions for command processor functions which performing operations: Accumulating characters ti a string, perform
lexical analysis and call the appropriate function handler based on the command. These functions provide the interface to the user
Command_Processor.h : This .H file conatins the declaration of the command processor functions mentioned above. It also declares the function pointer, structure and array of structures

main.c : Contains the main subroutine that calls the functions

Note: sysclock.c and sysclock.h were not updated

Each section was tested separately before integrating everything together. There was an issue I faced where after doing hexdump for a large length, random values were displayed at the end of hexdum and after ?. After lot of debugging, it was found that in cbfifo, while enqueuing and dequeuing, the interrupt wasn't disabled due to which in between enqueue/dequeue operation, it might go to the interrupt. Hence disabled the interrupts as soon as entering the function and enabled it just before return statements. This solved the problem.

References were taken from Assignemnt 2, 2a and Dean's github link which are mentioned in the source files.

The screenshots for terminal parameters and interactive terminal session are added to the git folder. 
