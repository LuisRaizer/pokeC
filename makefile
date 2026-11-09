CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = pokec
OBJS = main.o interface.o entidade.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c interface.h entidade.h
	$(CC) $(CFLAGS) -c main.c

interface.o: interface.c interface.h
	$(CC) $(CFLAGS) -c interface.c

entidade.o: entidade.c entidade.h
	$(CC) $(CFLAGS) -c entidade.c

clean:
	rm -f *.o $(TARGET)