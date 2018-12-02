CFLAGS = -g

objects = main.o 

fileSystem : $(objects)
	cc -o fileSystem $(objects) $(CFLAGS)

main.o : 

.PHONY : clean
clean :
	rm fileSystem $(objects)
