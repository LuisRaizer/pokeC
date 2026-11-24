#ifndef BATALHA_H
#define BATALHA_H

#include "entidade.h"
#include "arquivos.h"

#define XP_POR_VITORIA 50
#define XP_POR_NIVEL 100
#define LIMIAR_CURA_PERCENT 30

#define RESULTADO_ERRO -1
#define RESULTADO_JOGADOR_VENCEU 1
#define RESULTADO_INIMIGO_VENCEU 2

int calcular_dano(const Criatura *atacante, const Criatura *defensor, int indice_habilidade);
void ganhar_xp(Criatura *criatura, int quantidade);
void preparar_nova_batalha(EstadoBatalha *estado, Criatura *lista);
int iniciar_batalha(EstadoBatalha *estado);
void registrar_progresso_da_batalha(const EstadoBatalha *estado, Criatura *lista);

#endif