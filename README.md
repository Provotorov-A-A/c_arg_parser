# c_arg_parser

## Overview
- Parser may be used to parse `main` function command line arguments argv and argc.

- Supported command formats is `-command` and `-command key`.

- As result you will get array of `command : key` pairs or invalid argument index.

### How to use:
1. Add files *arg_parser.h* and  *arg_parser.c* to your project. Include *arg_parser.h* to main.c.

2. Create command description array (CDA). Each element of CDA is a string. 
First string is command name begining with symbol `'-'`, i.e `"-device"`. 
Second string is valid arguments list for command in the first string separated by specified delimiter string, i.e. `"avr, cortex-m3, risc-v"`.

3. Define two variables: 
- pointer for parse result array that parser will return;
- integer variable to get parsed *command-argument* pairs count.

4. Call `arg_parser_init` function with you CDA, CDA items count and delimiter string for command valid arguments list string as arguments.

5. Call `arg_parser_parse` function with you main's argc, argv and your variables for result from p.2 as arguments.

6. Check if `arg_parser_parse` function returnes zero that means all is OK. You have filled array of `command : key` pairs known size now!
if `arg_parser_parse` returned code is integer more than 0, it means that there is an error was occured in parsing process. Returned code is an index of argument in `argv` array that was produced this error. You can use it to print error message.

7. Free allocated for result array memory when you program will be finished. You can do it with `arg_parser_finish` subroutine or manually free the returned by `arg_parser_parse` function pointer.

### Notes:
1. It is required use ONLY delimiter string to separate arguments from each other. Do not use extra spaces or other symbols.
2. Parser does not analyze repeated commands and does not overwrite it's values. 

### Test
To build and run test project use *make* utility.
There are 4 targets:
- all: compile project and get arg_parser.exe executable file;
- clean: delete compiled arg_parser.exe executable file;
- run_invalid: run arg_parser.exe with some arguments. One of them is invalid;
- run_valid: run arg_parser.exe with some arguments. All of them are valid.

Clean project with command `make clean`.
Compile project with command `make all`.
Run demonstrating program with command `make run_valid` or `make run_invalid`.

### Most common use example
```
#define VALID_ARGUMENTS_DELIMITER	", "

// NOTE!!! ARGUMENTS LIST ITEMS MUST BE SEPARATED ONLY BY SPECIFIED DELIMITER STRING WITH NO EXTRA SPACES OR OTHER SYMBOLS
static const cmd_description cmd_descr[] = 
{
	{"-read", "all, page"},
	{"-write", "memory, register"},
	{"-erase", "full, sector"}
};

int main(int argc, char *argv[]) 
{
	cmd_arg_pair* parsed;
	size_t parsed_count;
	
	arg_parser_init(cli_struct, sizeof(cli_struct)/sizeof(cmd_description), VALID_ARGUMENTS_DELIMITER);	
	int r = arg_parser_parse(argc_dbg, argv_dbg, (void**)&parsed, &parsed_count);
	if (r)
	{
		printf("### Argument parser error: argument[%d], \"%s\" \n", r, argv_dbg[r]);
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
```
