CXX = g++
CFLAGS = -std=c++11 -g -m64
LINK = -pthread 
FILES = main.cpp ../raknet/*.cpp ../_defines.cpp
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
