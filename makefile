CC = g++
CFLAGS = -g -Wall -std=c++11
TARGET = emu

all: $(TARGET)

$(TARGET): main.cpp
	$(CC) $(CFLAGS) -o $(TARGET) main.cpp

clean:
	$(RM) $(TARGET)
