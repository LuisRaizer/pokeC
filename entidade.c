#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entidade.h"

Criatura* inicializar_criaturas(void) {
    Criatura* lista = (Criatura*) malloc(TOTAL_CRIATURAS * sizeof(Criatura));

    if (lista == NULL) {
        printf("Erro: Não foi possível alocar memória\n");
        return NULL;
    }

    strcpy(lista[0].nome, "Charmander");
    lista[0].nivel = 1;
    lista[0].xp = 0;
    lista[0].hp_max = 45;
    lista[0].hp_atual = 45;
    strcpy(lista[0].habilidades[0].nome, "Brasa");
    lista[0].habilidades[0].poder = 30;
    strcpy(lista[0].habilidades[1].nome, "Arranhão");
    lista[0].habilidades[1].poder = 15;

    strcpy(lista[1].nome, "Squirtle");
    lista[1].nivel = 1;
    lista[1].xp = 0;
    lista[1].hp_max = 45;
    lista[1].hp_atual = 45;
    strcpy(lista[1].habilidades[0].nome, "Bolhas");
    lista[1].habilidades[0].poder = 30;
    strcpy(lista[1].habilidades[1].nome, "Arranhão");
    lista[1].habilidades[1].poder = 15;

    strcpy(lista[2].nome, "Bulbasaur");
    lista[2].nivel = 1;
    lista[2].xp = 0;
    lista[2].hp_max = 45;
    lista[2].hp_atual = 45;
    strcpy(lista[2].habilidades[0].nome, "Chicote de Cipó");
    lista[2].habilidades[0].poder = 30;
    strcpy(lista[2].habilidades[1].nome, "Arranhão");
    lista[2].habilidades[1].poder = 15;

    strcpy(lista[3].nome, "Pikachu");
    lista[3].nivel = 1;
    lista[3].xp = 0;
    lista[3].hp_max = 40;
    lista[3].hp_atual = 40;
    strcpy(lista[3].habilidades[0].nome, "Choque do Trovão");
    lista[3].habilidades[0].poder = 30;
    strcpy(lista[3].habilidades[1].nome, "Ataque Rápido");
    lista[3].habilidades[1].poder = 15;

    return lista;
}

void liberar_criaturas(Criatura* lista) {
    if (lista != NULL) {
        free(lista);
    }
}

Criatura* obter_criatura_por_indice(Criatura* lista, int indice) {
    if (lista == NULL || indice < 0 || indice >= TOTAL_CRIATURAS) {
        printf("Erro: Índice %d inválido ou lista nula\n", indice);
        return NULL;
    }

    return &lista[indice];
}