#ifndef ENTIDADE_H
#define ENTIDADE_H

#include <stddef.h>

#define MAX_NOME        50
#define NUM_HABILIDADES 2
#define TOTAL_CRIATURAS 4

#define ENT_NIVEL_MIN     1
#define ENT_NIVEL_MAX     100
#define ENT_XP_MIN        0
#define ENT_XP_MAX        1000000
#define ENT_HP_MAX_LIMITE 9999
#define ENT_ATR_MIN       1
#define ENT_ATR_MAX       9999
#define ENT_PODER_MIN     0
#define ENT_PODER_MAX     9999

typedef struct {
    char nome[MAX_NOME];
    int poder;
} Habilidade;

typedef struct {
    char nome[MAX_NOME];
    int nivel;
    int xp;
    int hp_max;
    int hp_atual;
    int ataque;
    int defesa;
    Habilidade habilidades[NUM_HABILIDADES];
} Criatura;

Criatura* inicializar_criaturas(void);
void liberar_criaturas(Criatura *lista);

int validar_indice_criatura(const Criatura *lista, int indice);
Criatura* obter_criatura_por_indice(Criatura *lista, int indice);

const char* obter_nome(const Criatura *criatura);
int obter_nivel(const Criatura *criatura);
int obter_xp(const Criatura *criatura);
int obter_hp_atual(const Criatura *criatura);
int obter_hp_max(const Criatura *criatura);
int obter_ataque(const Criatura *criatura);
int obter_defesa(const Criatura *criatura);

void definir_nivel(Criatura *criatura, int nivel);
void definir_xp(Criatura *criatura, int xp);

int habilidade_pertence_criatura(const Criatura *criatura, int indice_habilidade);
const char* obter_nome_habilidade(const Criatura *criatura, int indice_habilidade);
int obter_poder_habilidade(const Criatura *criatura, int indice_habilidade);

void receber_dano(Criatura *criatura, int dano);
void curar_criatura(Criatura *criatura, int quantidade);
void restaurar_hp_total(Criatura *criatura);
void aumentar_atributos_nivel(Criatura *criatura, int bonus_hp, int bonus_ataque, int bonus_defesa);
int criatura_derrotada(const Criatura *criatura);

int validar_nivel_criatura(int nivel);
int validar_xp_criatura(int xp);
int criatura_para_texto(const Criatura *criatura, char *buffer, size_t tamanho_buffer);
int texto_para_criatura(const char *linha, Criatura *destino);

#endif