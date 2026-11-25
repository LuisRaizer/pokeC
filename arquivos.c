#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "arquivos.h"

#define TAM_LINHA_ARQ 512

static int ler_linha_completa(FILE *f, char *buffer, size_t tam) {
    if (f == NULL || buffer == NULL || tam == 0) {
        return 0;
    }

    if (fgets(buffer, (int)tam, f) == NULL) {
        return 0;
    }

    // Rejeita linha truncada (maior que o buffer suportado)
    if (strchr(buffer, '\n') == NULL && !feof(f)) {
        return 0;
    }

    return 1;
}

static int arquivo_tem_lixo_restante(FILE *f) {
    char extra[TAM_LINHA_ARQ];
    while (fgets(extra, sizeof(extra), f) != NULL) {
        for (size_t i = 0; extra[i] != '\0'; i++) {
            if (extra[i] != '\n' && extra[i] != '\r' && extra[i] != ' ' && extra[i] != '\t') {
                return 1;
            }
        }
    }
    return 0;
}

static int parse_cabecalho_batalha(
    const char *linha,
    int *indice_jogador,
    int *indice_inimigo,
    int *turno,
    int *placar_jogador,
    int *placar_inimigo
) {
    if (linha == NULL) {
        return 0;
    }

    int valores[5];
    const char *cursor = linha;

    for (int i = 0; i < 5; i++) {
        const char *sep = strchr(cursor, ';');
        int eh_ultimo = (i == 4);

        if (!eh_ultimo && sep == NULL) {
            return 0;
        }
        if (eh_ultimo && sep != NULL) {
            return 0;
        }

        size_t len = (sep != NULL) ? (size_t)(sep - cursor) : strlen(cursor);
        while (eh_ultimo && len > 0 && (cursor[len - 1] == '\n' || cursor[len - 1] == '\r')) {
            len--;
        }

        if (len == 0 || len >= 32 || cursor[0] == ' ' || cursor[0] == '\t') {
            return 0;
        }

        char campo[32];
        memcpy(campo, cursor, len);
        campo[len] = '\0';

        errno = 0;
        char *fim = NULL;
        long num = strtol(campo, &fim, 10);
        if (errno != 0 || fim == campo || *fim != '\0') {
            return 0;
        }

        if (num < -1000000L || num > 1000000L) {
            return 0;
        }

        valores[i] = (int)num;
        cursor = (sep != NULL) ? (sep + 1) : (cursor + len);
    }

    if (valores[0] < 0 || valores[0] >= TOTAL_CRIATURAS ||
        valores[1] < 0 || valores[1] >= TOTAL_CRIATURAS ||
        valores[2] < 1 || valores[2] > 9999 ||
        valores[3] < 0 || valores[3] > 999999 ||
        valores[4] < 0 || valores[4] > 999999) {
        return 0;
    }

    *indice_jogador = valores[0];
    *indice_inimigo = valores[1];
    *turno = valores[2];
    *placar_jogador = valores[3];
    *placar_inimigo = valores[4];
    return 1;
}

int salvar_progresso(const char *caminho, const Criatura *lista, int total) {
    if (caminho == NULL || lista == NULL || total <= 0) {
        return ARQ_ERRO;
    }

    FILE *f = fopen(caminho, "w");
    if (f == NULL) {
        return ARQ_ERRO;
    }

    char linha[TAM_LINHA_ARQ];
    for (int i = 0; i < total; i++) {
        if (!criatura_para_texto(&lista[i], linha, sizeof(linha))) {
            fclose(f);
            return ARQ_ERRO;
        }
        if (fprintf(f, "%s\n", linha) < 0) {
            fclose(f);
            return ARQ_ERRO;
        }
    }

    if (fclose(f) != 0) {
        return ARQ_ERRO;
    }

    return ARQ_OK;
}

int carregar_progresso(const char *caminho, Criatura *lista, int total) {
    if (caminho == NULL || lista == NULL || total <= 0) {
        return ARQ_ERRO;
    }

    FILE *f = fopen(caminho, "r");
    if (f == NULL) {
        return (errno == ENOENT) ? ARQ_PADRAO : ARQ_ERRO;
    }

    Criatura *temp = calloc((size_t)total, sizeof(Criatura));
    if (temp == NULL) {
        fclose(f);
        return ARQ_ERRO;
    }

    char linha[TAM_LINHA_ARQ];
    for (int i = 0; i < total; i++) {
        if (!ler_linha_completa(f, linha, sizeof(linha)) ||
            !texto_para_criatura(linha, &temp[i])) {
            free(temp);
            fclose(f);
            return ARQ_ERRO;
        }
    }

    if (arquivo_tem_lixo_restante(f)) {
        free(temp);
        fclose(f);
        return ARQ_ERRO;
    }

    memcpy(lista, temp, (size_t)total * sizeof(Criatura));
    free(temp);
    fclose(f);
    return ARQ_OK;
}

void inicializar_estado_batalha_padrao(EstadoBatalha *estado, Criatura *lista) {
    if (estado == NULL || lista == NULL) {
        return;
    }

    memset(estado, 0, sizeof(EstadoBatalha));

    int idx_jog = 0;
    int idx_ini = (TOTAL_CRIATURAS > 1) ? 1 : 0;

    Criatura *jog = obter_criatura_por_indice(lista, idx_jog);
    Criatura *ini = obter_criatura_por_indice(lista, idx_ini);
    if (jog == NULL || ini == NULL) {
        return;
    }

    estado->indice_jogador = idx_jog;
    estado->indice_inimigo = idx_ini;
    estado->jogador = *jog;
    estado->inimigo = *ini;
    restaurar_hp_total(&estado->jogador);
    restaurar_hp_total(&estado->inimigo);
    estado->turno = 1;
    estado->placar_jogador = 0;
    estado->placar_inimigo = 0;
}

int salvar_batalha(const char *caminho, const EstadoBatalha *estado) {
    if (caminho == NULL || estado == NULL) {
        return ARQ_ERRO;
    }

    char linha_jog[TAM_LINHA_ARQ];
    char linha_ini[TAM_LINHA_ARQ];

    if (!criatura_para_texto(&estado->jogador, linha_jog, sizeof(linha_jog)) ||
        !criatura_para_texto(&estado->inimigo, linha_ini, sizeof(linha_ini))) {
        return ARQ_ERRO;
    }

    FILE *f = fopen(caminho, "w");
    if (f == NULL) {
        return ARQ_ERRO;
    }

    if (fprintf(f, "%d;%d;%d;%d;%d\n%s\n%s\n",
                estado->indice_jogador,
                estado->indice_inimigo,
                estado->turno,
                estado->placar_jogador,
                estado->placar_inimigo,
                linha_jog,
                linha_ini) < 0) {
        fclose(f);
        return ARQ_ERRO;
    }

    if (fclose(f) != 0) {
        return ARQ_ERRO;
    }

    return ARQ_OK;
}

int carregar_batalha(const char *caminho, EstadoBatalha *estado, Criatura *lista) {
    if (caminho == NULL || estado == NULL || lista == NULL) {
        return ARQ_ERRO;
    }

    FILE *f = fopen(caminho, "r");
    if (f == NULL) {
        if (errno == ENOENT) {
            inicializar_estado_batalha_padrao(estado, lista);
            return ARQ_PADRAO;
        }
        return ARQ_ERRO;
    }

    EstadoBatalha temp;
    memset(&temp, 0, sizeof(EstadoBatalha));

    char linha[TAM_LINHA_ARQ];

    if (!ler_linha_completa(f, linha, sizeof(linha)) ||
        !parse_cabecalho_batalha(
            linha,
            &temp.indice_jogador,
            &temp.indice_inimigo,
            &temp.turno,
            &temp.placar_jogador,
            &temp.placar_inimigo)) {
        fclose(f);
        return ARQ_ERRO;
    }

    if (!ler_linha_completa(f, linha, sizeof(linha)) ||
        !texto_para_criatura(linha, &temp.jogador)) {
        fclose(f);
        return ARQ_ERRO;
    }

    if (!ler_linha_completa(f, linha, sizeof(linha)) ||
        !texto_para_criatura(linha, &temp.inimigo)) {
        fclose(f);
        return ARQ_ERRO;
    }

    if (criatura_derrotada(&temp.jogador) || criatura_derrotada(&temp.inimigo)) {
        fclose(f);
        return ARQ_ERRO;
    }

    if (arquivo_tem_lixo_restante(f)) {
        fclose(f);
        return ARQ_ERRO;
    }

    fclose(f);
    *estado = temp;
    return ARQ_OK;
}

int remover_batalha_salva(const char *caminho) {
    if (caminho == NULL) {
        return ARQ_ERRO;
    }
    if (remove(caminho) != 0) {
        return (errno == ENOENT) ? ARQ_PADRAO : ARQ_ERRO;
    }
    return ARQ_OK;
}

int existe_batalha_salva(const char *caminho) {
    if (caminho == NULL) {
        return 0;
    }

    FILE *f = fopen(caminho, "r");
    if (f == NULL) {
        return 0;
    }

    fclose(f);
    return 1;
}