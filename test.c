//========================================================================================
// 									INCLUDES
//========================================================================================
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "arg_parser.h"

//========================================================================================
//									DEFINES
//========================================================================================
#define VALID_ARGUMENTS_DELIMITER	", "

//========================================================================================
//									TYPES DECLARATION
//========================================================================================


//========================================================================================
//									VARIABLES DECLARATION
//========================================================================================
// NOTE!!! ARGUMENTS LIST ITEMS MUST BE SEPARATED ONLY BY SPECIFIED DELIMITER STRING WITH NO EXTRA SPACES OR OTHER SYMBOLS
static const cmd_description cmd_descr[] = 
{
	{"-read", "all, page"},
	{"-write", "memory, register"},
	{"-erase", "full, sector"}
};

//========================================================================================
//											MAIN
//========================================================================================
int main(int argc, char *argv[]) 
{
	cmd_arg_pair* parsed;
	size_t parsed_count;
	/*
	const int argc_dbg = 7;
	char* argv_dbg[argc_dbg];
	argv_dbg[0] = "$PWD";
	argv_dbg[1] = "-write";
	argv_dbg[2] = "yes";
	argv_dbg[3] = "-erase";
	argv_dbg[4] = "sector";
	argv_dbg[5] = "-read";
	argv_dbg[6] = "all";
	*/
	arg_parser_init(cmd_descr, sizeof(cmd_descr)/sizeof(cmd_description), VALID_ARGUMENTS_DELIMITER);	
	//int r = arg_parser_parse(argc_dbg, argv_dbg, (void**)&parsed, &parsed_count);
	int r = arg_parser_parse(argc, argv, (void**)&parsed, &parsed_count);
	
	if (r)
	{
		printf("### Argument parser error: argument[%d], \"%s\" \n", r, argv[r]);
		return -1;
	}
	else
	{
		printf("%d parsed commands:\n", parsed_count);
		size_t i;
		for (i = 0; i < parsed_count; ++i)
		{
			printf("%s : %s\n", parsed[i].cmd, parsed[i].arg);
		}
	}	
	return 0;
}
