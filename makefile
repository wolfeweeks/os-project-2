
GCC = gcc#an unnecessary pattern
.SUFFIXES: #none for this project (no object files)

all: master slave

slave: slave.c
	$(GCC) slave.c -o slave

master: master.c
	$(GCC) master.c -o master

clean:
	rm -rf *.o ./master ./slave