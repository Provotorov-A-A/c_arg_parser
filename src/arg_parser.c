//==================================================================================
// 									INCLUDES
//==================================================================================
#include "arg_parser.h"
#include <ctype.h>

//==================================================================================
//									TYPES DECLARATION
//==================================================================================


//==================================================================================
//									VARIABLES DECLARATION
//==================================================================================
static int arg_parser_argc;
static char** arg_parser_argv;

static const cmd_description* arg_parser_cmd_descr;
static size_t valid_commands_count;
static char* arg_parser_delimiter;

static cmd_arg_pair* parsed_pairs_array;

//==================================================================================
//									FUNCTIONS DEFINITION
//==================================================================================
// Returns 0 if string is zero-length or it is all spaces symbols. In other case returns 1
static int is_valid_token(const char *str)
{
	if(0 == strlen(str))
	{
		return 0;
	}
	while(isspace((unsigned char)*str)) str++;
	if(*str == 0)  // All spaces?
	{
		return 0;
	}
	return 1;
}

//----------------------------------------------------------------------------------------
// Search command in commands description array and returns it's index. 
// If cmd is not valid command - function returns negative value
static int find_cmd(char* const cmd)
{
	size_t i;
	for (i = 0; i < valid_commands_count; ++i)
	{
		if ( 0 == strcmp(cmd, arg_parser_cmd_descr[i].cmd) )
		{
			return i;
		}
	}
	return -1;	
}

//----------------------------------------------------------------------------------------
// Returns 1 if string is in valid argument list for command whis that index in commands description array
static int is_valid_arg(const int index, const char* const arg_str)
{
	char* tmp = malloc(strlen(arg_parser_cmd_descr[index].valid_args_list));
	if (NULL == tmp)
	{
		return 0;
	}
	strcpy(tmp, arg_parser_cmd_descr[index].valid_args_list);
	char* token = strtok(tmp, arg_parser_delimiter);
	while(NULL != token)
	{
		if (0 == strcmp(token, arg_str))
		{
			return 1;	
		}
		token = strtok(NULL, arg_parser_delimiter);
	}
	free (tmp);
	return 0;
}

//----------------------------------------------------------------------------------------
// Allocate memory for valid parsed pairs cmd-arg and do some initialization operations
// IN cmd_descr_list - pointer to command description array (see readme file for format explanation)
// IN command_count - total count of valid commands (Number of cmd_description structs in command description array)
// IN argument_delimiter - pointer to string that contains delimiter that used in string with possible arguments for each command in command description array
// Return - 0 if all is OK, not 0 - initialization failed
int arg_parser_init(const cmd_description* cmd_descr_list, size_t command_count, char* argument_delimiter)
{
	size_t i;
	
	// Fill valid commands array
	valid_commands_count = command_count;

	arg_parser_cmd_descr = cmd_descr_list;
	
	// Allocate empty array of cmd_arg_pair items
	char* tmp = malloc(command_count * sizeof(cmd_arg_pair));
	if (NULL == tmp)
	{
		return -1;		
	}
	memset(tmp, 0x00, command_count * sizeof(cmd_arg_pair));
	parsed_pairs_array = (cmd_arg_pair*)tmp;
	
	if (NULL != argument_delimiter)
	{
		arg_parser_delimiter = 	argument_delimiter;
	}
	return 0;
}

//----------------------------------------------------------------------------------------
// Parse CLI and return parsed pairs of command-argument
// IN argc - the same as argc in main
// IN argv - the same as argv in main
// OUT pResult - pointer to allocated array of parsed pairs. Must be free after use (call arg_parse_finish for this)
// OUT parsed_pairs_count - returned count of valid parsed command-argument pairs (size of array pointed by pResult)
// Return - 0 if all is OK, index of argument in argv that cause a problem (index 0 is unused because it usually contents execution command)
int arg_parser_parse(int argc_, char *argv_[], void** pResult, size_t* parsed_pairs_count)
{
	size_t i, j, k;
	size_t cur_parsed_pair_index = 0;

	if ((argc_ < 2) || (NULL == argv_))	
	{
		return 1;	
	}
	arg_parser_argc = argc_;
	arg_parser_argv = argv_;
	
	for (i = 1; i < arg_parser_argc;)
	{
		char* next_token = NULL;
		// Check it is command
		if ('-' != arg_parser_argv[i][0])
		{
			return i;	
		}
		char* cmd = arg_parser_argv[i];
		int index = find_cmd(cmd);
		if ( index < 0 )
		{
			return i;	
		}
		parsed_pairs_array[cur_parsed_pair_index].cmd = cmd;
		
		// It's valid command with known index in arg_parser_cmd_descr
		// Check next token is valid argument
		int has_valid_token_next = 0;
		if (i+1 < arg_parser_argc)
		{
			next_token = arg_parser_argv[i+1];
			if ('-' != next_token[0])
			{
				if (is_valid_token(next_token))	
				{
					has_valid_token_next = 1;
				}
			}
		}
		// Check is argument required for this command
		int arg_required = 0;
		if ( is_valid_token(arg_parser_cmd_descr[index].valid_args_list) ) 
		{
			arg_required = 1;
		}
		
		if ( arg_required )
		{
			// Have invalid token insteed of argument
			if ( !has_valid_token_next )
			{
				return i;	
			}
			// Check token is valid argument for current command
			if ( !is_valid_arg(index, next_token) )
			{
				return i+1;	
			}
			parsed_pairs_array[cur_parsed_pair_index].arg = next_token;
			i+=2;
			cur_parsed_pair_index+=1;
			continue;
		}
		else
		{
			// Have token when it's not required
			if ( has_valid_token_next )
			{
				return i+1;	
			}
			else
			{
				i+=1;
				cur_parsed_pair_index+=1;
				continue;	
			}
		}
	}
	
	*parsed_pairs_count = cur_parsed_pair_index;
	*pResult = parsed_pairs_array;
	
	return 0;
}

//----------------------------------------------------------------------------------------
void arg_parser_finish()
{
	free(parsed_pairs_array);
}
