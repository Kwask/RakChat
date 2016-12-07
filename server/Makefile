CXX = g++
INC = ../include/
CFLAGS = -std=c++11 -g -m64 -I$(INC)
LINK = -pthread
FILES = main.cpp ../include/raknet/*.cpp
BIN = server.out

OBJS = *.o

.PHONY: clean

linux:
	@echo "--------------------"
	@echo "Compiling server binary"
	@echo "--------------------"
	@$(CXX) $(CFLAGS) -c $(FILES)
	@echo "Object compilation complete"
	@$(CXX) $(CFLAGS) $(LINK) $(OBJS) -o $(BIN)
	@echo "Binary file generated"
clean:
	$(RM) -r *.o *~ *.gch *.out
