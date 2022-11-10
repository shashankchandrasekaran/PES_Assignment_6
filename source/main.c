/*
 *  Filename: main.c
 *  Created on: Nov 06, 2022
 *  Author: Shashank Chandrasekaran
 *  Description: This .C fie contains the main subroutine which calls the command processor
 *  Reference: None
 */

#include "sysclock.h"
#include "UART.h"
#include "Command_Processor.h"
# include "test_cbfifo.h"



int main(void)
{
  sysclock_init(); //Initializes Clock

  Init_UART0(BAUD_RATE); //Initializes UART

  test_cbfifo(); //Tests CBFIFO

  command_processor(); //Calls Main command processor function

  return 0 ;
}
