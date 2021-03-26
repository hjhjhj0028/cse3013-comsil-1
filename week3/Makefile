CC = g++
CPPFLAGS = -g -c

OBJECTS = main.o stringClass.o
SRC = main.cpp stringClass.cpp

TARGET = main

$(TARGET): $(OBJECTS) stringClass.h
	$(CC) -o $(TARGET) $(OBJECTS)

clean: 
	rm $(OBJECTS) $(TARGET)
