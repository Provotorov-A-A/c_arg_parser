#ifndef _ARG_PARSER_H_
#define _ARG_PARSER_H_

//==================================================================================
// 									INCLUDES
//==================================================================================
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <windows.h>

//==================================================================================
//									TYPES DECLARATION
//==================================================================================
typedef struct 
{
	char* cmd;
	char* arg;
} cmd_arg_pair;

//----------------------------------------------------------------------------------------
typedef struct 
{
	char* cmd;
	char* valid_args_list;
} cmd_description;


//==================================================================================
//									VARIABLES DECLARATION
//==================================================================================


//==================================================================================
//									FUNCTIONS DECLARATION
//==================================================================================
int arg_parser_init(const cmd_description* cmd_descr_list, size_t command_count, char* argument_delimiter);
int arg_parser_parse(int argc_, char *argv_[], void** pResult, size_t* parsed_pairs_count);
void arg_parser_finish();

#endif /*_ARG_PARSER_H_*/
