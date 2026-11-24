#ifndef ENTIDADES_H
#define ENTIDADES_H

#define MAX_NOME 30
#define NUM_HABILIDADES 2
#define TOTAL_CRIATURAS 4

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
Criatura* obter_criatura_por_indice(Criatura* lista, int indice);
void liberar_criaturas(Criatura* lista);

int validar_indice_criatura(Criatura* lista, int indice);
int habilidade_pertence_criatura(const Criatura* criatura, int indice_habilidade);
const Habilidade* obter_habilidade_por_indice(const Criatura* criatura, int indice_habilidade);

const char* obter_nome(const Criatura* criatura);
const char* obter_nome_habilidade(const Criatura* criatura, int indice_habilidade);
int obter_poder_habilidade(const Criatura* criatura, int indice_habilidade);
int obter_hp_max(const Criatura* criatura);
int obter_hp_atual(const Criatura* criatura);
int obter_ataque(const Criatura* criatura);
int obter_defesa(const Criatura* criatura);
int obter_nivel(const Criatura* criatura);
void definir_nivel(Criatura* criatura, int nivel);
int obter_xp(const Criatura* criatura);
void definir_xp(Criatura* criatura, int xp);

void curar_criatura(Criatura* criatura, int quantidade);
void receber_dano(Criatura* criatura, int quantidade);
int criatura_derrotada(const Criatura* criatura);

#endif