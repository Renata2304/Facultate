CC = gcc
CFLAGS  = -g -Wall
TARGET1 = AVL
TARGET2 = Hash

all: $(TARGET1) $(TARGET2)
 
build: $(TARGET1) $(TARGET2)

$(TARGET1): $(TARGET1).c
	$(CC) $(CFLAGS) -o $(TARGET1) $(TARGET1).c

$(TARGET2): $(TARGET2).c
	$(CC) $(CFLAGS) -o $(TARGET2) $(TARGET2).c


run-p1: $(TARGET1)
	./$(TARGET1)
 
run-p2: $(TARGET2)
	./$(TARGET2) 

run-best: $(TARGET2)
	./$(TARGET1)
 

clean:
	$(RM) $(TARGET1) $(TARGET2) test.out