

# Macros para teste
BASH = sh
TEST_SCRIPT = test.sh

# Macros para construcao do zip
ZIP = zip
USERNAME ?= $(USER)
ZIPFILE = $(USERNAME).zip
EXTENSIONS = *.c *.h *.in *.out *.sh

.PHONY: depend clean

all:main_singlethread main_multithread main_multiprocess

main_singlethread:
	gcc -omain_singlethread main_singlethread.c imageprocessing.c -I./ -lfreeimage

main_multithread:
	gcc -omain_multithread main_multithread.c imageprocessing.c -I./ -lfreeimage -lpthread

main_multiprocess:
	gcc -omain_multiprocess main_multiprocess.c imageprocessing.c -I./ -lfreeimage

test:all
	$(BASH) $(TEST_SCRIPT) main_singlethread main_multithread main_multiprocess

clean:
	$(RM) ./main_singlethread
	$(RM) ./main_multithread
	$(RM) ./main_multiprocess

	$(RM) /*.o
