#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include "entidade.h"

#define ARQ_OK 0
#define ARQ_PADRAO 1
#define ARQ_ERRO -1

#define NIVEL_MIN 1
#define NIVEL_MAX 100
#define XP_MIN 0
#define XP_MAX 1000000
#define HP_MAX_LIMITE 9999
#define ATR_MIN 1
#define ATR_MAX 9999
#define PODER_MIN 0
#define PODER_MAX 9999

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

#endif