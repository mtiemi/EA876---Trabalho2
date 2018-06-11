

# Macros para teste
BASH = sh
TEST_SCRIPT = test.sh

# Macros para construcao do zip
ZIP = zip
USERNAME ?= $(USER)
ZIPFILE = $(USERNAME).zip
EXTENSIONS = *.c *.h *.in *.out *.sh
DIR = ./src/

.PHONY: depend clean

all:main_singlethread main_multithread main_multiprocess

main_singlethread: $(DIR)main_singlethread.c $(DIR)imageprocessing.c
	gcc -omain_singlethread $(DIR)main_singlethread.c $(DIR)imageprocessing.c -I./ -lfreeimage -g

main_multithread: $(DIR)main_multithread.c $(DIR)imageprocessing.c
	gcc -omain_multithread $(DIR)main_multithread.c $(DIR)imageprocessing.c -I./ -lfreeimage -lpthread -g

main_multiprocess: $(DIR)main_multiprocess.c $(DIR)imageprocessing.c
	gcc -omain_multiprocess $(DIR)main_multiprocess.c $(DIR)imageprocessing.c -I./ -lfreeimage -g

test:all
	$(BASH) $(TEST_SCRIPT) main_singlethread main_multithread main_multiprocess

clean:
	$(RM) ./main_singlethread
	$(RM) ./main_multithread
	$(RM) ./main_multiprocess

	$(RM) /*.o
