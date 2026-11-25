CC = gcc
CFLAGS = -Wall -Wextra -g

TARGET = pokec
TEST_TARGET = teste/rodar_testes

CORE_OBJS = interface.o entidade.o arquivos.o batalha.o
OBJS = main.o $(CORE_OBJS)
TEST_OBJS = main_testes.o teste/teste_aux.o teste/teste_arquivos.o teste/teste_batalha.o teste/teste_interface.o

.PHONY: all teste clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

$(TEST_TARGET): $(TEST_OBJS) $(CORE_OBJS)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_OBJS) $(CORE_OBJS)

teste: $(TEST_TARGET)
	./$(TEST_TARGET)

main.o: main.c interface.h entidade.h arquivos.h batalha.h
	$(CC) $(CFLAGS) -c main.c -o main.o

interface.o: interface.c interface.h entidade.h arquivos.h
	$(CC) $(CFLAGS) -c interface.c -o interface.o

entidade.o: entidade.c entidade.h
	$(CC) $(CFLAGS) -c entidade.c -o entidade.o

arquivos.o: arquivos.c arquivos.h entidade.h
	$(CC) $(CFLAGS) -c arquivos.c -o arquivos.o

batalha.o: batalha.c batalha.h entidade.h arquivos.h interface.h
	$(CC) $(CFLAGS) -c batalha.c -o batalha.o

main_testes.o: main_testes.c teste/teste_aux.h
	$(CC) $(CFLAGS) -c main_testes.c -o main_testes.o

teste/teste_aux.o: teste/teste_aux.c teste/teste_aux.h
	$(CC) $(CFLAGS) -c teste/teste_aux.c -o teste/teste_aux.o

teste/teste_arquivos.o: teste/teste_arquivos.c entidade.h arquivos.h teste/teste_aux.h
	$(CC) $(CFLAGS) -c teste/teste_arquivos.c -o teste/teste_arquivos.o

teste/teste_batalha.o: teste/teste_batalha.c entidade.h batalha.h arquivos.h teste/teste_aux.h
	$(CC) $(CFLAGS) -c teste/teste_batalha.c -o teste/teste_batalha.o

teste/teste_interface.o: teste/teste_interface.c interface.h teste/teste_aux.h
	$(CC) $(CFLAGS) -c teste/teste_interface.c -o teste/teste_interface.o

clean:
	rm -f *.o teste/*.o $(TARGET) $(TEST_TARGET)