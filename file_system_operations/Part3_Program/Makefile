CC = gcc

DEPS = blocks.h
OBJ = fileSystemOper.o blocks.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

fileSystemOper: $(OBJ)
	gcc $(CFLAGS) -o $@ $^ -lm
.PHONY : clean
clean:
	-rm *.o $(objects)
