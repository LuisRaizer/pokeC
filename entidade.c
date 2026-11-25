#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "entidade.h"

static void preencher_criatura(
    Criatura *c,
    const char *nome,
    int hp_max,
    int ataque,
    int defesa,
    const char *hab1,
    int poder1,
    const char *hab2,
    int poder2
) {
    if (c == NULL) {
        return;
    }

    snprintf(c->nome, MAX_NOME, "%s", nome);
    c->nivel = ENT_NIVEL_MIN;
    c->xp = ENT_XP_MIN;
    c->hp_max = hp_max;
    c->hp_atual = hp_max;
    c->ataque = ataque;
    c->defesa = defesa;

    snprintf(c->habilidades[0].nome, MAX_NOME, "%s", hab1);
    c->habilidades[0].poder = poder1;

    snprintf(c->habilidades[1].nome, MAX_NOME, "%s", hab2);
    c->habilidades[1].poder = poder2;
}

Criatura* inicializar_criaturas(void) {
    Criatura *lista = calloc(TOTAL_CRIATURAS, sizeof(Criatura));
    if (lista == NULL) {
        return NULL;
    }

    preencher_criatura(&lista[0], "Charmander",  95, 18, 12, "Brasa",            10, "Lanca-Chamas", 18);
    preencher_criatura(&lista[1], "Bulbassauro", 115, 14, 18, "Chicote de Vinha", 10, "Folha Navalha", 16);
    preencher_criatura(&lista[2], "Squirtle",    105, 15, 15, "Jato d'Agua",      11, "Hidro Pulso",   17);
    preencher_criatura(&lista[3], "Pikachu",     90, 19, 11, "Choque do Trovao",  12, "Raio",          19);

    return lista;
}

void liberar_criaturas(Criatura *lista) {
    free(lista);
}

int validar_indice_criatura(const Criatura *lista, int indice) {
    return (lista != NULL && indice >= 0 && indice < TOTAL_CRIATURAS);
}

Criatura* obter_criatura_por_indice(Criatura *lista, int indice) {
    if (!validar_indice_criatura(lista, indice)) {
        return NULL;
    }
    return &lista[indice];
}

const char* obter_nome(const Criatura *criatura) {
    return (criatura != NULL) ? criatura->nome : "Desconhecida";
}

int obter_nivel(const Criatura *criatura) {
    return (criatura != NULL) ? criatura->nivel : 0;
}

int obter_xp(const Criatura *criatura) {
    return (criatura != NULL) ? criatura->xp : 0;
}

int obter_hp_atual(const Criatura *criatura) {
    return (criatura != NULL) ? criatura->hp_atual : 0;
}

int obter_hp_max(const Criatura *criatura) {
    return (criatura != NULL) ? criatura->hp_max : 0;
}

int obter_ataque(const Criatura *criatura) {
    return (criatura != NULL) ? criatura->ataque : 0;
}

int obter_defesa(const Criatura *criatura) {
    return (criatura != NULL) ? criatura->defesa : 0;
}

void definir_nivel(Criatura *criatura, int nivel) {
    if (criatura == NULL || !validar_nivel_criatura(nivel)) {
        return;
    }
    criatura->nivel = nivel;
}

void definir_xp(Criatura *criatura, int xp) {
    if (criatura == NULL || !validar_xp_criatura(xp)) {
        return;
    }
    criatura->xp = xp;
}

int habilidade_pertence_criatura(const Criatura *criatura, int indice_habilidade) {
    return (criatura != NULL && indice_habilidade >= 0 && indice_habilidade < NUM_HABILIDADES);
}

const char* obter_nome_habilidade(const Criatura *criatura, int indice_habilidade) {
    if (!habilidade_pertence_criatura(criatura, indice_habilidade)) {
        return "Ataque Basico";
    }
    return criatura->habilidades[indice_habilidade].nome;
}

int obter_poder_habilidade(const Criatura *criatura, int indice_habilidade) {
    if (!habilidade_pertence_criatura(criatura, indice_habilidade)) {
        return 0;
    }
    return criatura->habilidades[indice_habilidade].poder;
}

void receber_dano(Criatura *criatura, int dano) {
    if (criatura == NULL || dano <= 0) {
        return;
    }

    criatura->hp_atual -= dano;
    if (criatura->hp_atual < 0) {
        criatura->hp_atual = 0;
    }
}

void curar_criatura(Criatura *criatura, int quantidade) {
    if (criatura == NULL || quantidade <= 0) {
        return;
    }

    criatura->hp_atual += quantidade;
    if (criatura->hp_atual > criatura->hp_max) {
        criatura->hp_atual = criatura->hp_max;
    }
}

void restaurar_hp_total(Criatura *criatura) {
    if (criatura == NULL) {
        return;
    }
    criatura->hp_atual = criatura->hp_max;
}

void aumentar_atributos_nivel(Criatura *criatura, int bonus_hp, int bonus_ataque, int bonus_defesa) {
    if (criatura == NULL) {
        return;
    }

    if (bonus_hp > 0) {
        criatura->hp_max += bonus_hp;
        if (criatura->hp_max > ENT_HP_MAX_LIMITE) {
            criatura->hp_max = ENT_HP_MAX_LIMITE;
        }
        curar_criatura(criatura, bonus_hp);
    }

    if (bonus_ataque > 0) {
        criatura->ataque += bonus_ataque;
        if (criatura->ataque > ENT_ATR_MAX) {
            criatura->ataque = ENT_ATR_MAX;
        }
    }

    if (bonus_defesa > 0) {
        criatura->defesa += bonus_defesa;
        if (criatura->defesa > ENT_ATR_MAX) {
            criatura->defesa = ENT_ATR_MAX;
        }
    }
}

int criatura_derrotada(const Criatura *criatura) {
    if (criatura == NULL) {
        return 1;
    }
    return (criatura->hp_atual <= 0);
}

int validar_nivel_criatura(int nivel) {
    return (nivel >= ENT_NIVEL_MIN && nivel <= ENT_NIVEL_MAX);
}

int validar_xp_criatura(int xp) {
    return (xp >= ENT_XP_MIN && xp <= ENT_XP_MAX);
}

static int extrair_campo(const char **cursor, char *dest, size_t tam_dest, int eh_ultimo) {
    if (cursor == NULL || *cursor == NULL || dest == NULL || tam_dest == 0) {
        return 0;
    }

    const char *inicio = *cursor;
    const char *sep = strchr(inicio, ';');

    if (!eh_ultimo && sep == NULL) {
        return 0;
    }
    if (eh_ultimo && sep != NULL) {
        return 0;
    }

    size_t len = (sep != NULL) ? (size_t)(sep - inicio) : strlen(inicio);

    while (eh_ultimo && len > 0 && (inicio[len - 1] == '\n' || inicio[len - 1] == '\r')) {
        len--;
    }

    if (len == 0 || len >= tam_dest) {
        return 0;
    }

    for (size_t i = 0; i < len; i++) {
        if (inicio[i] == '\n' || inicio[i] == '\r') {
            return 0;
        }
    }

    memcpy(dest, inicio, len);
    dest[len] = '\0';

    *cursor = (sep != NULL) ? (sep + 1) : (inicio + len);
    return 1;
}

static int ler_inteiro_na_faixa(const char *texto, int min, int max, int *saida) {
    if (texto == NULL || *texto == '\0' || saida == NULL) {
        return 0;
    }

    if (texto[0] == ' ' || texto[0] == '\t') {
        return 0;
    }

    errno = 0;
    char *fim = NULL;
    long valor = strtol(texto, &fim, 10);

    if (errno != 0 || fim == texto || *fim != '\0') {
        return 0;
    }
    if (valor < (long)min || valor > (long)max) {
        return 0;
    }

    *saida = (int)valor;
    return 1;
}

int criatura_para_texto(const Criatura *criatura, char *buffer, size_t tamanho_buffer) {
    if (criatura == NULL || buffer == NULL || tamanho_buffer == 0) {
        return 0;
    }

    if (criatura->nome[0] == '\0' ||
        !validar_nivel_criatura(criatura->nivel) ||
        !validar_xp_criatura(criatura->xp) ||
        criatura->hp_max < 1 || criatura->hp_max > ENT_HP_MAX_LIMITE ||
        criatura->hp_atual < 0 || criatura->hp_atual > criatura->hp_max ||
        criatura->ataque < ENT_ATR_MIN || criatura->ataque > ENT_ATR_MAX ||
        criatura->defesa < ENT_ATR_MIN || criatura->defesa > ENT_ATR_MAX) {
        return 0;
    }

    int escrito = snprintf(
        buffer, tamanho_buffer,
        "%s;%d;%d;%d;%d;%d;%d",
        criatura->nome,
        criatura->nivel,
        criatura->xp,
        criatura->hp_max,
        criatura->hp_atual,
        criatura->ataque,
        criatura->defesa
    );

    if (escrito < 0 || (size_t)escrito >= tamanho_buffer) {
        return 0;
    }

    size_t usado = (size_t)escrito;
    for (int i = 0; i < NUM_HABILIDADES; i++) {
        if (criatura->habilidades[i].nome[0] == '\0' ||
            criatura->habilidades[i].poder < ENT_PODER_MIN ||
            criatura->habilidades[i].poder > ENT_PODER_MAX) {
            return 0;
        }

        int n = snprintf(
            buffer + usado, tamanho_buffer - usado,
            ";%s;%d",
            criatura->habilidades[i].nome,
            criatura->habilidades[i].poder
        );
        if (n < 0 || (size_t)n >= (tamanho_buffer - usado)) {
            return 0;
        }
        usado += (size_t)n;
    }

    return 1;
}

int texto_para_criatura(const char *linha, Criatura *destino) {
    if (linha == NULL || destino == NULL) {
        return 0;
    }

    Criatura tmp;
    memset(&tmp, 0, sizeof(Criatura));

    const char *cursor = linha;
    char campo[64];

    if (!extrair_campo(&cursor, tmp.nome, MAX_NOME, 0)) {
        return 0;
    }

    if (!extrair_campo(&cursor, campo, sizeof(campo), 0) ||
        !ler_inteiro_na_faixa(campo, ENT_NIVEL_MIN, ENT_NIVEL_MAX, &tmp.nivel) ||
        !validar_nivel_criatura(tmp.nivel)) {
        return 0;
    }

    if (!extrair_campo(&cursor, campo, sizeof(campo), 0) ||
        !ler_inteiro_na_faixa(campo, ENT_XP_MIN, ENT_XP_MAX, &tmp.xp) ||
        !validar_xp_criatura(tmp.xp)) {
        return 0;
    }

    if (!extrair_campo(&cursor, campo, sizeof(campo), 0) ||
        !ler_inteiro_na_faixa(campo, 1, ENT_HP_MAX_LIMITE, &tmp.hp_max)) {
        return 0;
    }

    if (!extrair_campo(&cursor, campo, sizeof(campo), 0) ||
        !ler_inteiro_na_faixa(campo, 0, tmp.hp_max, &tmp.hp_atual)) {
        return 0;
    }

    if (!extrair_campo(&cursor, campo, sizeof(campo), 0) ||
        !ler_inteiro_na_faixa(campo, ENT_ATR_MIN, ENT_ATR_MAX, &tmp.ataque)) {
        return 0;
    }

    if (!extrair_campo(&cursor, campo, sizeof(campo), 0) ||
        !ler_inteiro_na_faixa(campo, ENT_ATR_MIN, ENT_ATR_MAX, &tmp.defesa)) {
        return 0;
    }

    for (int i = 0; i < NUM_HABILIDADES; i++) {
        int eh_ultimo_campo = (i == NUM_HABILIDADES - 1);

        if (!extrair_campo(&cursor, tmp.habilidades[i].nome, MAX_NOME, 0)) {
            return 0;
        }

        if (!extrair_campo(&cursor, campo, sizeof(campo), eh_ultimo_campo) ||
            !ler_inteiro_na_faixa(campo, ENT_PODER_MIN, ENT_PODER_MAX, &tmp.habilidades[i].poder)) {
            return 0;
        }
    }

    *destino = tmp;
    return 1;
}