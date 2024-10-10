CC = gcc
CFLAGS  = -g -Wall
TARGET = rand_generator

all: $(TARGET)
 
build: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)