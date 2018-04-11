MODULE_SRC= getMoves.cpp
MODULE_OBJ= getMoves.o
MODULE_TARGET= getMoves
MODULE_INCS= -I../include/
CCFLAGS= -ansi -pedantic -Wall -Wextra -std=c++14 -g -c
LDFLAGS= -g -o

$(MODULE_TARGET): $(MODULE_OBJ)
	g++ $(MODULE_OBJ) -lpgn $(LDFLAGS) ./$(MODULE_TARGET)

.cpp.o:
	g++ $(CCFLAGS) $< $(MODULE_INCS)

clean:
	rm -f *.o
	rm -f ./$(MODULE_TARGET)

