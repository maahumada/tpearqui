include Makefile.inc

SHELL_DATA=0001-shellDataModule.bin

all: shellCodeModule shellDataModule

shellCodeModule:
	cd shellCodeModule; make

shellDataModule:
	printf "This is shell data." >> $(SHELL_DATA) && dd if=/dev/zero bs=1 count=1 >> $(SHELL_DATA)

clean:
	cd shellCodeModule; make clean
	rm -rf *.bin


.PHONY: shellCodeModule all clean
