CFLAGS = -g

objects = main.o utilities.o

fileSystem : $(objects)
	cc -o fileSystem $(objects) $(CFLAGS)

main.o : utilities.h

.PHONY : clean
clean :
	rm fileSystem $(objects)
