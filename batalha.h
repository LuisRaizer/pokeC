#ifndef BATALHA_H
#define BATALHA_H

#include "entidade.h"
#include "arquivos.h"

#define RESULTADO_EM_ANDAMENTO   0
#define RESULTADO_JOGADOR_VENCEU 1
#define RESULTADO_INIMIGO_VENCEU 2
#define RESULTADO_PAUSADA        3

#define XP_POR_VITORIA     50
#define XP_POR_NIVEL       100
#define BONUS_HP_NIVEL     5
#define BONUS_ATAQUE_NIVEL 2
#define BONUS_DEFESA_NIVEL 2

void preparar_nova_batalha(EstadoBatalha *estado, Criatura *lista);
int calcular_dano(const Criatura *atacante, const Criatura *alvo, int poder_habilidade);
int executar_turno_jogador(EstadoBatalha *estado);
void executar_turno_inimigo(EstadoBatalha *estado);
int verificar_fim_batalha(EstadoBatalha *estado);
void ganhar_xp(Criatura *criatura, int xp_ganho);
int iniciar_batalha(EstadoBatalha *estado);
void registrar_progresso_da_batalha(const EstadoBatalha *estado, Criatura *lista);

#endif