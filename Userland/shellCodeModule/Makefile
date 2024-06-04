include ../Makefile.inc

MODULE=0000-shell.bin
SOURCES=$(wildcard [^_]*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)

all: $(MODULE)

$(MODULE): $(SOURCES)
	$(GCC) $(GCCFLAGS) -I./include -T shell.ld _loader.c $(SOURCES) $(OBJECTS_ASM) -o ../$(MODULE)
	$(GCC) $(GCCFLAGS) -I./include -T shell.ld -Wl,--oformat=elf64-x86-64 _loader.c $(SOURCES) $(OBJECTS_ASM) -o ../shell.elf


clean:
	rm -rf *.o

.PHONY: all clean print