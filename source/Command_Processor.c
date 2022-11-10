/*
 *  Filename: Command_Processor.c
 *  Created on: Nov 07, 2022
 *  Author: Shashank Chandrasekaran
 *  Description: This C file contains the definitions of functions related to command processor
 *  Reference: ECEN 5813 PES 21 Command Processing slide Page 23-28
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Command_Processor.h"
#include "hexdump.h"

#define LENGTH_MIN (0)
#define LENGTH_MAX (640)
#define HEX_BASE (16)
#define ADD_HEX_START ('0')
#define ADD_HEX_UP ('X')
#define ADD_HEX_LO ('x')
#define ARGV_MAX_SIZE (10)
#define SPACE (32)
#define BUFFER_SIZE (1000)
#define INIT_VAL (-1)

/****************************Function handler for Author command******************************/
void author(int argc, char *argv[])
{
	printf("\r\nShashank Chandrasekaran\r\n"); //Print author name
}

/****************************Function handler for Dump command*******************************/
void dump(int argc, char *argv[])
{
	int start_add=0, length=0;

	//Obtain the start address and convert to decimal from hex (in string)
	start_add = strtol(argv[1],NULL,HEX_BASE);

	//If the length is in hex format
	if((*(argv[2]) == ADD_HEX_START) && ((*(argv[2]+1) == ADD_HEX_LO)||(*(argv[2]+1) == ADD_HEX_UP)))
		length = strtol(argv[2],NULL,HEX_BASE); //Convert to decimal from string
	else
		length = atoi(argv[2]); //Convert to decimal from string if length in decimal format

	if(length>=LENGTH_MIN && length<=LENGTH_MAX) //Check if length is within the range
		hexdump((char*)start_add,length); //Call the hexdump function
	else
		printf("\r\nThe entered length %d is not in the specified range 0-640\r\n",length);
}

/****************************Function handler for Help command******************************/
void help(int argc, char *argv[])
{
	for (int i=0; i < num_commands; i++) //Run loop till number of commands
		printf("%s",commands[i].help_string); //Print the help string for each command
}

/**************Process Command Function for Lexical analysis and calling handlers***********/
void process_command(char *input)
{
	char *p = input;
	char *end;
	char store_prev=*input; //Use this variable to store the previous value in input string
	bool handler_status = false; //Used to check if a valid command is received or not
	char *argv[ARGV_MAX_SIZE]; //Array of character pointers to store address of arguments
	int argc = 0; //Variable to store number of arguments
	memset(argv, 0, sizeof(argv));

	for (end = input; *end != '\0'; end++); // find end of string

	//Lexical Analysis
	for (p = input; p<end; p++)
	{
		if(*p>SPACE) //If the value is above space in ascii table
		{
			//If the previous character is  first address or space or null
			if(p==input || store_prev==SPACE || store_prev=='\0')
			{
				argv[argc]=p; //Store the start address of argument
				argc++; //Increment number of argument
			}
		}
		//If current value is space and previous value not space or null
		if(*p==SPACE && (store_prev!=SPACE || store_prev!='\0'))
		{
			*p='\0'; //Force to null
		}
		store_prev=*p; //Store previous byte in string
	}
	argv[argc] = NULL;
	if (argc == 0) // no command
		return;

	//Call the handlers based on the commands received
	for (int i=0; i < num_commands; i++)
	{
		if (strcasecmp(argv[0], commands[i].name) == 0) //For valid commands
		{
			commands[i].handler(argc, argv); //Call the appropriate handler function
			handler_status=true;
			break;
		}
	}

	//If invalid command
	if(handler_status==false)
	{
		char *i;
		printf("\r\nUnknown Command:");
		for(i=argv[0];*i!='\0';i++) //To print the invalid command
			printf("%c",*i);
		printf("\r\n");
	}
}

/*****************************Main Command Processor function*********************************/
void command_processor(void)
{
	char buffer[BUFFER_SIZE]; //Used to store the string containing various arguments
	memset(buffer,0,BUFFER_SIZE);
	int i=INIT_VAL;
	printf("\rWelcome to BreakfastSerial!\n\r");
	while(1)
	{
		printf("? ");

		//Accumulating line
		while(buffer[i]!='\r')
		{
			i++;
			buffer[i]= getchar(); //Store the character received from user

			//To handle backspace
			if((buffer[i] == '\b') && (i > 0))
			{
				buffer[i] =' ';
				i--;
				buffer[i]=' ';
				i--;
				printf(" \b");
			}
		}
		buffer[i]='\0'; //Null terminate the string
		process_command(buffer); //Pass the string to the process command function
		//Reset the variables before processing new string
		memset(buffer,0,BUFFER_SIZE);
		i=INIT_VAL;
	}
}








