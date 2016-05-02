# minimalist makefile
.SUFFIXES:
#
.SUFFIXES: .cpp .o .c .h
ifeq ($(DEBUG),1)
CFLAGS = -fPIC  -std=c99 -ggdb -march=native -Wall -Wextra -Wshadow -fsanitize=undefined  -fno-omit-frame-pointer -fsanitize=address
else
CFLAGS = -fPIC -std=c99 -O3  -march=native -Wall -Wextra -Wshadow
endif # debug
OBJECTS=bitset.o
all:  $(OBJECTS)
HEADERS=./include/bitset.h 

bitset.o: ./src/bitset.c $(HEADERS)
	$(CC) $(CFLAGS) -c ./src/bitset.c -Iinclude
clean:
	rm -f  *.o 
