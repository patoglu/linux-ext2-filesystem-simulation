CC = gcc

DEPS = blocks.h
OBJ = makeFileSystem.o blocks.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

makeFileSystem: $(OBJ)
	gcc $(CFLAGS) -o $@ $^ -lm
.PHONY : clean
clean:
	-rm *.o $(objects)
