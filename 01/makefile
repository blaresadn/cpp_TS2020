CC=g++
CFLAGS=--std=c++17 -c -Wall -O2
LDFLAGS=
SOURCES=tests.cpp 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=tests
all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
clean: 
	rm -f *.o $(EXECUTABLE)
test:
	./$(EXECUTABLE)
