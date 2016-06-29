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
all: unit benchmark $(OBJECTS)
HEADERS=./include/bitset.h

bitset.o: ./src/bitset.c $(HEADERS)
	$(CC) $(CFLAGS) -c ./src/bitset.c -Iinclude

unit: bitset.o ./tests/unit.c $(HEADERS)
	$(CC) $(CFLAGS) -o unit ./tests/unit.c bitset.o -Iinclude


benchmark: bitset.o ./benchmarks/benchmark.c $(HEADERS)
	$(CC) $(CFLAGS) -o benchmark ./benchmarks/benchmark.c bitset.o -Iinclude
clean:
	rm -f  *.o unit benchmark
