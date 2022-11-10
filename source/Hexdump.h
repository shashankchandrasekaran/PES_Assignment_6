/*
 *  Filename: Hexdump.h
 *  Created on: Nov 09, 2022
 *  Author: Shashank Chandrasekaran
 *  Description: This C file contains the declaration of hexdump function
 *  Reference: None
 */

#ifndef HEXDUMP_H_
#define HEXDUMP_H_

/*
 * Function for displaying memory address and bytes of data in memory 16 bytes per line
 * from a start address till the length specified by user
 * Parameter passed: Memory start address: address, bytes of data to be displayed: len
 * Parameters returned: None
 */
void hexdump(char *address, int len);


#endif /* HEXDUMP_H_ */
