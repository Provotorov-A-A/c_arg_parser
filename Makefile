PRJ_DIR = C:/Private/Projects/c_arg_parser
EXE = $(PRJ_DIR)/arg_parser.exe

SRC = $(PRJ_DIR)/test.c
SRC += $(PRJ_DIR)/src/arg_parser.c

all: $(SRC)
	gcc $(SRC) -I$(PRJ_DIR)/src -o $(EXE)

clean:
	@rm -f $(EXE)
	
run_invalid:
	$(EXE) -erase full -get_id XXX -write register -read page 

run_valid:
	$(EXE) -erase full -get_id -write_id 0xDEADBEAF -write register -read page
