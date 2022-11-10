/*
 *  Filename: Hexdump.c
 *  Created on: Nov 09, 2022
 *  Author: Shashank Chandrasekaran
 *  Description: This C file contains the definition of hexdump function
 *  Reference: None
 */

#include <stdio.h>
#include "hexdump.h"

#define UPPER_BYTES (0xFFFF0000)
#define LOWER_BYTES (0x0000FFFF)
#define DIV (16)


/*******************Function for displaying bytes of memory in hex******************************/
void hexdump(char *address, int len)
{
	int addr; //variable to print address
	int count=0; //Variable to keep track of number of bytes written
	while(len--) //Traverse the memory till len
	{
		if(count%DIV==0) //If 16 bytes of memory is read
		{
			addr=(int)address; //current address stored in variable
			printf("\r\n");
			//Separating the upper and lower 4 bytes to print in the hex format below
			printf("%04X_%04X ",(UPPER_BYTES & addr),(LOWER_BYTES & addr));
		}
		printf("%02X ",*address); //Byte printed in hex
		address++;
		count++;
	}
	printf("\r\n");
}

