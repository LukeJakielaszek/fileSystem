CFLAGS = -g

objects = main.o llist.o

fileSystem : $(objects)
	cc -o fileSystem $(objects) $(CFLAGS)

main.o : llist.h

.PHONY : clean
clean :
	rm fileSystem $(objects)
