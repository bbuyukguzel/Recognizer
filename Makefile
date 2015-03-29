CC=g++
CFLAGS=-c -Wall

all: tron

tron: GPIO.o SEN10736.o
	$(CC) GPIO.o SEN10736.o -o tron

GPIO.o: GPIO.cpp
	$(CC) $(CFLAGS) GPIO.cpp

SEN10736.o: SEN10736.cpp
	$(CC) $(CFLAGS) SEN10736.cpp

clean:
	rm *o tron

