
CFLAGS := -std=c++11 #-Wall
CC := g++
TARGET := MAIN
SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SRCS))
all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(TARGET) *.o
