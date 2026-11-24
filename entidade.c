#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entidade.h"

Criatura* inicializar_criaturas(void) {
    Criatura* lista = (Criatura*) malloc(TOTAL_CRIATURAS * sizeof(Criatura));
    if (lista == NULL) {
        printf("Erro: Nao foi possivel alocar memoria\n");
        return NULL;
    }

    strcpy(lista[0].nome, "Charmander");
    lista[0].nivel = 1;
    lista[0].xp = 0;
    lista[0].hp_max = 45;
    lista[0].hp_atual = 45;
    lista[0].ataque = 12;
    lista[0].defesa = 8;
    strcpy(lista[0].habilidades[0].nome, "Brasa");
    lista[0].habilidades[0].poder = 30;
    strcpy(lista[0].habilidades[1].nome, "Arranhao");
    lista[0].habilidades[1].poder = 15;

    strcpy(lista[1].nome, "Squirtle");
    lista[1].nivel = 1;
    lista[1].xp = 0;
    lista[1].hp_max = 45;
    lista[1].hp_atual = 45;
    lista[1].ataque = 10;
    lista[1].defesa = 11;
    strcpy(lista[1].habilidades[0].nome, "Bolhas");
    lista[1].habilidades[0].poder = 30;
    strcpy(lista[1].habilidades[1].nome, "Arranhao");
    lista[1].habilidades[1].poder = 15;

    strcpy(lista[2].nome, "Bulbasaur");
    lista[2].nivel = 1;
    lista[2].xp = 0;
    lista[2].hp_max = 45;
    lista[2].hp_atual = 45;
    lista[2].ataque = 11;
    lista[2].defesa = 10;
    strcpy(lista[2].habilidades[0].nome, "Chicote de Cipo");
    lista[2].habilidades[0].poder = 30;
    strcpy(lista[2].habilidades[1].nome, "Arranhao");
    lista[2].habilidades[1].poder = 15;

    strcpy(lista[3].nome, "Pikachu");
    lista[3].nivel = 1;
    lista[3].xp = 0;
    lista[3].hp_max = 40;
    lista[3].hp_atual = 40;
    lista[3].ataque = 13;
    lista[3].defesa = 7;
    strcpy(lista[3].habilidades[0].nome, "Choque do Trovao");
    lista[3].habilidades[0].poder = 30;
    strcpy(lista[3].habilidades[1].nome, "Ataque Rapido");
    lista[3].habilidades[1].poder = 15;

    return lista;
}

void liberar_criaturas(Criatura* lista) {
    if (lista != NULL) {
        free(lista);
    }
}

int validar_indice_criatura(Criatura* lista, int indice) {
    if (lista == NULL || indice < 0 || indice >= TOTAL_CRIATURAS) {
        return 0;
    }
    return 1;
}

Criatura* obter_criatura_por_indice(Criatura* lista, int indice) {
    if (!validar_indice_criatura(lista, indice)) {
        printf("Erro: Indice %d invalido ou lista nula\n", indice);
        return NULL;
    }
    return &lista[indice];
}

int habilidade_pertence_criatura(const Criatura* criatura, int indice_habilidade) {
    if (criatura == NULL || indice_habilidade < 0 || indice_habilidade >= NUM_HABILIDADES) {
        return 0;
    }
    if (criatura->habilidades[indice_habilidade].nome[0] == '\0') {
        return 0;
    }
    return 1;
}

const Habilidade* obter_habilidade_por_indice(const Criatura* criatura, int indice_habilidade) {
    if (!habilidade_pertence_criatura(criatura, indice_habilidade)) {
        printf("Erro: Habilidade %d invalida\n", indice_habilidade);
        return NULL;
    }
    return &criatura->habilidades[indice_habilidade];
}

const char* obter_nome(const Criatura* criatura) {
    if (criatura == NULL) {
        printf("Erro: Criatura nula\n");
        return "";
    }
    return criatura->nome;
}

const char* obter_nome_habilidade(const Criatura* criatura, int indice_habilidade) {
    const Habilidade* hab = obter_habilidade_por_indice(criatura, indice_habilidade);
    return hab != NULL ? hab->nome : "";
}

int obter_poder_habilidade(const Criatura* criatura, int indice_habilidade) {
    const Habilidade* hab = obter_habilidade_por_indice(criatura, indice_habilidade);
    return hab != NULL ? hab->poder : -1;
}

int obter_hp_max(const Criatura* criatura) {
    if (criatura == NULL) {
        printf("Erro: Criatura nula\n");
        return -1;
    }
    return criatura->hp_max;
}

int obter_hp_atual(const Criatura* criatura) {
    if (criatura == NULL) {
        printf("Erro: Criatura nula\n");
        return -1;
    }
    return criatura->hp_atual;
}

int obter_ataque(const Criatura* criatura) {
    if (criatura == NULL) {
        printf("Erro: Criatura nula\n");
        return -1;
    }
    return criatura->ataque;
}

int obter_defesa(const Criatura* criatura) {
    if (criatura == NULL) {
        printf("Erro: Criatura nula\n");
        return -1;
    }
    return criatura->defesa;
}

int obter_nivel(const Criatura* criatura) {
    if (criatura == NULL) {
        printf("Erro: Criatura nula\n");
        return -1;
    }
    return criatura->nivel;
}

void definir_nivel(Criatura* criatura, int nivel) {
    if (criatura == NULL) {
        printf("Erro: Criatura nula\n");
        return;
    }
    if (nivel > 0) {
        criatura->nivel = nivel;
    }
}

int obter_xp(const Criatura* criatura) {
    if (criatura == NULL) {
        printf("Erro: Criatura nula\n");
        return -1;
    }
    return criatura->xp;
}

void definir_xp(Criatura* criatura, int xp) {
    if (criatura == NULL) {
        printf("Erro: Criatura nula\n");
        return;
    }
    if (xp >= 0) {
        criatura->xp = xp;
    }
}

void curar_criatura(Criatura* criatura, int quantidade) {
    if (criatura == NULL) {
        printf("Erro: Criatura nula\n");
        return;
    }
    if (quantidade <= 0) {
        return;
    }

    criatura->hp_atual += quantidade;
    if (criatura->hp_atual > criatura->hp_max) {
        criatura->hp_atual = criatura->hp_max;
    }
}

void receber_dano(Criatura* criatura, int quantidade) {
    if (criatura == NULL) {
        printf("Erro: Criatura nula\n");
        return;
    }
    if (quantidade <= 0) {
        return;
    }

    criatura->hp_atual -= quantidade;
    if (criatura->hp_atual < 0) {
        criatura->hp_atual = 0;
    }
}

int criatura_derrotada(const Criatura* criatura) {
    return criatura == NULL || criatura->hp_atual <= 0;
}