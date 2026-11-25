CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = pokec
OBJS = main.o interface.o entidade.o arquivos.o batalha.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c interface.h entidade.h arquivos.h batalha.h
	$(CC) $(CFLAGS) -c main.c

interface.o: interface.c interface.h entidade.h arquivos.h
	$(CC) $(CFLAGS) -c interface.c

entidade.o: entidade.c entidade.h
	$(CC) $(CFLAGS) -c entidade.c

arquivos.o: arquivos.c arquivos.h entidade.h
	$(CC) $(CFLAGS) -c arquivos.c

batalha.o: batalha.c batalha.h entidade.h arquivos.h interface.h
	$(CC) $(CFLAGS) -c batalha.c

clean:
	rm -f *.o $(TARGET)