	

# Macros para teste
BASH = sh
TEST_SCRIPT = test.sh

# Macros para construcao do zip
ZIP = zip
USERNAME ?= $(USER)
ZIPFILE = $(USERNAME).zip
EXTENSIONS = *.c *.h *.in *.out *.sh

.PHONY: depend clean

all:main main2 main3

main:
	gcc -omain main.c imageprocessing.c -I./ -lfreeimage 

main2:
	gcc -omain2 main2.c imageprocessing.c -I./ -lfreeimage -lpthread
    	
test:all
	$(BASH) $(TEST_SCRIPT) main

clean:
	$(RM) ./main
	$(RM) ./main2
	$(RM) /*.o

