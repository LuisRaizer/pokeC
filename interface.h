#ifndef INTERFACE_H
#define INTERFACE_H

#include "entidade.h"
#include "arquivos.h"

void exibir_menu_inicial(void);
void exibir_instrucoes(void);
void imprimir_mensagem(const char *msg);
int ler_opcao(int min, int max);

void exibir_catalogo_de_criaturas(const Criatura *lista, int total);
void exibir_resultado_da_batalha(const char *vencedor, int eh_aliado);

void exibir_status_batalha(const Criatura *jogador, const Criatura *inimigo, int turno);
void exibir_menu_habilidades(const Criatura *criatura);
void exibir_ataque(const char *atacante, const char *habilidade, const char *alvo, int dano);
void exibir_fim_de_turno(int turno);
void exibir_subida_de_nivel(const char *nome, int novo_nivel);

void exibir_erro_arquivo(int codigo_status, const char *acao);
void exibir_tela_carregar_partida(int status_carregamento, const EstadoBatalha *estado);
void exibir_tela_progresso(const Criatura *lista, int total);
int confirmar_descarte_save(void);

#endif