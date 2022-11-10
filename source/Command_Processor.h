/*
 *  Filename: Command_Processor.h
 *  Created on: Nov 07, 2022
 *  Author: Shashank Chandrasekaran
 *  Description: This C file contains the declaration of functions related to command processor
 *  Reference: ECEN 5813 PES 21 Command Processing slide Page 23-28
 */

#ifndef COMMAND_PROCESSOR_H_
#define COMMAND_PROCESSOR_H_

typedef void (*command_handler_t)(int, char *argv[]); //Function pointer


typedef struct {
	const char *name;
	command_handler_t handler;
	const char *help_string;
} command_table_t;


/*
 * Function handler when command entered by user is "Author". Prints the author's name
 * Parameter passed: The number of arguments: argc and address of the arguments: argv[]
 * Both these parameters aren't used inside the function
 * Parameters returned: None
 */
void author(int argc, char *argv[]);

/*
 * Function handler when command entered by user is "Dump". Calls the hexdump function
 * if the user entered length is in range.
 * Parameter passed: The number of arguments: argc and address of the arguments: argv[]
 * argv is used to fetch start address and length
 * Parameters returned: None
 */
void dump(int argc, char *argv[]);

/*
 * Function handler when command entered by user is "Help". Provides information about
 * each command
 * Parameter passed: The number of arguments: argc and address of the arguments: argv[]
 * Both these parameters aren't used inside the function
 * Parameters returned: None
 */
void help(int argc, char *argv[]);


/*
 * Function performs lexical analysis on the input string and calls the appropriate
 * handler based on the command received
 * Parameter passed: The input string: input
 * Parameters returned: None
 */
void process_command(char *input);


/*
 * Function is the main command processor which prepares the input string from the
 * characters entered by user and then passes that string to process command function
 * Parameter passed: None
 * Parameters returned: None
 */
void command_processor(void);


//Array of structures with each element corresponding to the command, handler and help string
static const command_table_t commands[] = {
		{"Author",author,"\r\nBreakFastSerial Command Processor\r\n"
		 "Author\r\n       Print the author of this code\r\n"},

		{"Dump",dump,"\r\nDump <loc> <len>\r\n       Output a hexdump "
		 "starting at location loc.\r\n       Always specify loc in hex; "
		 "specify len in hex (0xNN) or dec(NN)\r\n"},

		{"Help",help,"\r\nHelp\r\n       Print this help message\r\n"}
};


//Number of commands
static const int num_commands = sizeof(commands) / sizeof(command_table_t);

#endif /* COMMAND_PROCESSOR_H_ */
