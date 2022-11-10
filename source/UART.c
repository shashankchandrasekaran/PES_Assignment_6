/*
 *  Filename: UART.c
 *  Created on: Nov 06, 2022
 *  Author: Shashank Chandrasekaran
 *  Description: This C file contains the definitions of UART0 functions
 *  Reference: https:https://github.com/alexander-g-dean/ESF/blob/master/NXP/Code/Chapter_8/Serial-Demo/src/UART.c
 */

#include <stdio.h>
#include "UART.h"

Q_T TxQ, RxQ; //Transmit and receive buffers

/*****************************UART0 Initialization Function***********************************/
void Init_UART0(uint32_t baud_rate)
{
	uint16_t sbr;

	//Clock gating enabled for UART0 and Port A
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	//Transmitter and receiver are disabled before init
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

	//UART clock set to 48 MHz clock
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);

	//Pins set to UART0 Rx and Tx
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Rx
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Tx

	//Baud rate and oversampling ratio set
	sbr = (uint16_t)((SYS_CLOCK)/(baud_rate * UART_OVERSAMPLE_RATE));
	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr>>SBR_SHIFT);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE-1);

	//Interrupts for RX active edge and LIN break detect set, one stop bit selected
	//SBNS set to 1 for 2 stop bits
	UART0->BDH |= UART0_BDH_RXEDGIE(0) | UART0_BDH_SBNS(1) | UART0_BDH_LBKDIE(0);

	//Don't enable loopback mode, use 8 data bit mode, don't use parity
	UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(0) | UART0_C1_PE(0);
	//Don't invert transmit data, don't enable interrupts for errors
	UART0->C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(0)| UART0_C3_NEIE(0)
											| UART0_C3_FEIE(0) | UART0_C3_PEIE(0);

	//Clear error flags
	UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);

	//Send LSB first, do not invert received data
	UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);

	//Set NVIC priorities
	NVIC_SetPriority(UART0_IRQn, 2); // 0, 1, 2, or 3
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);

	//Enable receive interrupts but not transmit interrupts yet
	UART0->C2 |= UART_C2_RIE(1);

	//Enable UART receiver and transmitter
	UART0->C2 |= UART0_C2_RE(1) | UART0_C2_TE(1);

	//Clear the UART RDRF flag
	UART0->S1 &= ~UART0_S1_RDRF_MASK;

	//Initialize CBFIFO transmit and receive buffer
	cbfifo_init(&RxQ);
	cbfifo_init(&TxQ);
}

/*****************************UART0 Interrupt Handler******************************************/
void UART0_IRQHandler(void)
{
	uint8_t ch; //Variable to store or transmit the data

	//If interrupt due to error flags
	if (UART0->S1 & (UART_S1_OR_MASK |UART_S1_NF_MASK |
			UART_S1_FE_MASK | UART_S1_PF_MASK))
	{
		// clear the error flags
		UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK |
				UART0_S1_FE_MASK | UART0_S1_PF_MASK;
		// read the data register to clear RDRF
		ch = UART0->D;
	}

	//If interrupt due to receiving a character
	if (UART0->S1 & UART0_S1_RDRF_MASK)
	{
		ch = UART0->D; //received a character
		UART0->D = ch; //Echo character

		//Enqueue a byte successfully in read buffer and check if it enqueued
		if(cbfifo_enqueue(&ch,1,&RxQ) == 1)
		{
			;
		}
		else
		{
			// error - queue full.
			// discard character
		}
	}

	//If interrupt due to transmitting a character
	if ( (UART0->C2 & UART0_C2_TIE_MASK) && // transmitter interrupt enabled
			(UART0->S1 & UART0_S1_TDRE_MASK) )
	{
		// If transmit buffer empty
		if (!cbfifo_empty(&TxQ))
		{
			cbfifo_dequeue(&ch, 1, &TxQ); //Dequeue the transmit buffer
			UART0->D=ch; //Transmit dequeued byte serially
		}
		else
		{
			// queue is empty so disable transmitter interrupt
			UART0->C2 &= ~UART0_C2_TIE_MASK;
		}
	}
}

/**********************************Function called by printf***********************************/
int __sys_write(int handle, char *buf, int size)
{
	if(size<=0 || buf==NULL)
		return FAIL;

	while(size>(Q_MAX_SIZE-cbfifo_length(&TxQ))); // wait for space to open up

	if(cbfifo_enqueue(buf,size,&TxQ) != size)
		return FAIL;

	//start transmitter if it isn't already running
	if (!(UART0->C2 & UART0_C2_TIE_MASK))
	{
		UART0->C2 |= UART0_C2_TIE(1);
	}

	return 0; //Return on success
}

/*******************************Function called by getchar************************************/
int __sys_readc(void)
{
	char ch;
	while((cbfifo_length(&RxQ))==0); //Wait till the receive buffer is empty
	if(cbfifo_dequeue(&ch,1,&RxQ)==1) //Dequeue the byte
		return ch; //Return byte
	else
		return FAIL; //Dequeue operation failed
}

