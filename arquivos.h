#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include "entidade.h"

#define ARQ_OK 0
#define ARQ_PADRAO 1
#define ARQ_ERRO -1

#define ARQ_CAMINHO_BATALHA   "save.txt"
#define ARQ_CAMINHO_PROGRESSO "progresso.txt"

typedef struct {
    int indice_jogador;
    int indice_inimigo;
    Criatura jogador;
    Criatura inimigo;
    int turno;
    int placar_jogador;
    int placar_inimigo;
} EstadoBatalha;

int salvar_progresso(const char *caminho, const Criatura *lista, int total);
int carregar_progresso(const char *caminho, Criatura *lista, int total);

void inicializar_estado_batalha_padrao(EstadoBatalha *estado, Criatura *lista);
int salvar_batalha(const char *caminho, const EstadoBatalha *estado);
int carregar_batalha(const char *caminho, EstadoBatalha *estado, Criatura *lista);
int remover_batalha_salva(const char *caminho);
int existe_batalha_salva(const char *caminho);

#endif