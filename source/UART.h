/*
 *  Filename: UART.h
 *  Created on: Nov 06, 2022
 *  Author: Shashank Chandrasekaran
 *  Description: This H file contains the declaration of UART0 functions
 *  Reference: https://github.com/alexander-g-dean/ESF/blob/master/NXP/Code/Chapter_8/Serial-Demo/inc/UART.h
 */

#ifndef UART_H_
#define UART_H_

#include <MKL25Z4.H>
#include <stdint.h>
#include "cbfifo.h"

#define BAUD_RATE  (38400) //38400 baud rate
#define USE_UART_INTERRUPTS  (0) //0 for polled UART communications, 1 for interrupt-driven
#define UART_OVERSAMPLE_RATE (16)
#define BUS_CLOCK (24e6)
#define SYS_CLOCK (24e6)
#define SBR_SHIFT  (8)
#define FAIL (-1)


/*
 * Function for initializing UART0
 * Parameter passed: Desired baud rate: baud_rate
 * Parameters returned: None
 */
void Init_UART0(uint32_t baud_rate);

/*
 * UART0 interrupt handler
 * Parameter passed: None
 * Parameters returned: None
 */
void UART0_IRQHandler(void);


/*
 * Inbuilt function replaced to redirect the characters from UART0 to the serial terminal.
 * This function is called by printf
 * Parameter passed: Handle :Writes bytes to stdout (Handle=1) or stderr (Handle=2)
 * buf: The string to be written, size: The bytes of data to be transmitted
 * Parameters returned: Status of operation i.e. 0 on success and -1 on failure
 */
int __sys_write(int handle, char *buf, int size);


/*
 * Inbuilt function replaced to redirect the characters from the serial terminal to the UART0.
 * This function is called by getchar
 * Parameter passed: None
 * Parameters returned: Dequeued byte on success and -1 on failure
 */
int __sys_readc(void);

#endif /* UART_H_ */
