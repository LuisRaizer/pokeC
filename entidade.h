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
    Habilidade habilidades[NUM_HABILIDADES];
} Criatura;

Criatura* inicializar_criaturas(void);
Criatura* obter_criatura_por_indice(Criatura* lista, int indice);
void liberar_criaturas(Criatura* lista);

#endif