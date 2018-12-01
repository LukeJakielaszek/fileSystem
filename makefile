CFLAGS = -g

objects = main.o llist.o utilities.o

fileSystem : $(objects)
	cc -o fileSystem $(objects) $(CFLAGS)

main.o : llist.h utilities.h

.PHONY : clean
clean :
	rm fileSystem $(objects)
