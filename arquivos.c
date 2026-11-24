#include "arquivos.h"
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_LINHA 256

static int ler_linha(FILE *fp, char *buf, size_t tam) {
    if (!fgets(buf, (int)tam, fp)) {
        return 0;
    }
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
        if (len > 1 && buf[len - 2] == '\r') {
            buf[len - 2] = '\0';
        }
        return 1;
    }
    return feof(fp) ? 1 : -1;
}

static int dividir_csv(char *linha, char **campos, int esperado) {
    int qtd = 0;
    char *inicio = linha;
    for (char *p = linha;; p++) {
        if (*p == ',' || *p == '\0') {
            if (qtd >= esperado) {
                return 0;
            }
            campos[qtd++] = inicio;
            if (*p == '\0') {
                break;
            }
            *p = '\0';
            inicio = p + 1;
        }
    }
    return qtd == esperado;
}

static int parse_int_faixa(const char *texto, int min, int max, int *destino) {
    if (!texto || *texto == '\0' || isspace((unsigned char)*texto)) {
        return 0;
    }
    char *fim = NULL;
    errno = 0;
    long valor = strtol(texto, &fim, 10);
    if (errno != 0 || fim == texto || *fim != '\0') {
        return 0;
    }
    if (valor < min || valor > max) {
        return 0;
    }
    *destino = (int)valor;
    return 1;
}

static int validar_nome(const char *nome) {
    if (!nome) {
        return 0;
    }
    size_t len = strlen(nome);
    if (len == 0 || len >= MAX_NOME) {
        return 0;
    }
    int tem_caractere = 0;
    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)nome[i];
        if (!isprint(c) || c == ',') {
            return 0;
        }
        if (!isspace(c)) {
            tem_caractere = 1;
        }
    }
    return tem_caractere;
}

static int validar_criatura_completa(const Criatura *c) {
    if (!c || !validar_nome(c->nome)) {
        return 0;
    }
    if (c->nivel < NIVEL_MIN || c->nivel > NIVEL_MAX ||
        c->xp < XP_MIN || c->xp > XP_MAX ||
        c->hp_max < 1 || c->hp_max > HP_MAX_LIMITE ||
        c->hp_atual < 0 || c->hp_atual > c->hp_max ||
        c->ataque < ATR_MIN || c->ataque > ATR_MAX ||
        c->defesa < ATR_MIN || c->defesa > ATR_MAX) {
        return 0;
    }
    for (int i = 0; i < NUM_HABILIDADES; i++) {
        if (!validar_nome(c->habilidades[i].nome) ||
            c->habilidades[i].poder < PODER_MIN ||
            c->habilidades[i].poder > PODER_MAX) {
            return 0;
        }
    }
    return 1;
}

static void aplicar_progresso_padrao(Criatura *lista, int total) {
    Criatura *padrao = inicializar_criaturas();
    if (!padrao) {
        return;
    }
    int limite = total < TOTAL_CRIATURAS ? total : TOTAL_CRIATURAS;
    for (int i = 0; i < limite; i++) {
        lista[i] = padrao[i];
    }
    liberar_criaturas(padrao);
}

int salvar_progresso(const char *caminho, const Criatura *lista, int total) {
    if (!caminho || !lista || total != TOTAL_CRIATURAS) {
        return ARQ_ERRO;
    }

    for (int i = 0; i < total; i++) {
        if (!validar_nome(lista[i].nome) ||
            lista[i].nivel < NIVEL_MIN || lista[i].nivel > NIVEL_MAX ||
            lista[i].xp < XP_MIN || lista[i].xp > XP_MAX) {
            printf("Erro: Dados invalidos ao salvar progresso da criatura %d\n", i);
            return ARQ_ERRO;
        }
    }

    FILE *fp = fopen(caminho, "w");
    if (!fp) {
        printf("Erro: Nao foi possivel abrir %s para escrita\n", caminho);
        return ARQ_ERRO;
    }

    for (int i = 0; i < total; i++) {
        if (fprintf(fp, "%s,%d,%d\n", lista[i].nome, lista[i].nivel, lista[i].xp) < 0) {
            fclose(fp);
            return ARQ_ERRO;
        }
    }

    if (fclose(fp) != 0) {
        return ARQ_ERRO;
    }
    return ARQ_OK;
}

int carregar_progresso(const char *caminho, Criatura *lista, int total) {
    if (!caminho || !lista || total != TOTAL_CRIATURAS) {
        return ARQ_ERRO;
    }

    errno = 0;
    FILE *fp = fopen(caminho, "r");
    if (!fp) {
        if (errno == ENOENT) {
            aplicar_progresso_padrao(lista, total);
            return ARQ_PADRAO;
        }
        printf("Erro: Falha ao abrir %s\n", caminho);
        return ARQ_ERRO;
    }

    Criatura temp[TOTAL_CRIATURAS];
    for (int i = 0; i < total; i++) {
        temp[i] = lista[i];
    }

    char linha[TAM_LINHA];
    int count = 0;

    while (1) {
        int status = ler_linha(fp, linha, sizeof(linha));
        if (status == 0) {
            break;
        }
        if (status < 0 || linha[0] == '\0' || count >= total) {
            printf("Erro: Arquivo %s corrompido ou fora de formato\n", caminho);
            fclose(fp);
            return ARQ_ERRO;
        }

        char *campos[3];
        int nivel = 0;
        int xp = 0;

        if (!dividir_csv(linha, campos, 3) ||
            !validar_nome(campos[0]) ||
            !parse_int_faixa(campos[1], NIVEL_MIN, NIVEL_MAX, &nivel) ||
            !parse_int_faixa(campos[2], XP_MIN, XP_MAX, &xp)) {
            printf("Erro: Dados invalidos na linha %d de %s\n", count + 1, caminho);
            fclose(fp);
            return ARQ_ERRO;
        }

        strncpy(temp[count].nome, campos[0], MAX_NOME - 1);
        temp[count].nome[MAX_NOME - 1] = '\0';
        definir_nivel(&temp[count], nivel);
        definir_xp(&temp[count], xp);
        count++;
    }

    fclose(fp);

    if (count != total) {
        printf("Erro: Quantidade de registros invalida em %s\n", caminho);
        return ARQ_ERRO;
    }

    for (int i = 0; i < total; i++) {
        lista[i] = temp[i];
    }
    return ARQ_OK;
}

void inicializar_estado_batalha_padrao(EstadoBatalha *estado, Criatura *lista) {
    if (!estado || !lista) {
        return;
    }
    estado->indice_jogador = 0;
    estado->indice_inimigo = 1;
    estado->jogador = lista[0];
    estado->inimigo = lista[1];
    estado->turno = 1;
    estado->placar_jogador = 0;
    estado->placar_inimigo = 0;
}

static int escrever_criatura_batalha(FILE *fp, const char *tag, int indice, const Criatura *c) {
    return fprintf(fp, "%s,%d,%s,%d,%d,%d,%d,%d,%d,%s,%d,%s,%d\n",
                   tag,
                   indice,
                   c->nome,
                   c->nivel,
                   c->xp,
                   c->hp_max,
                   c->hp_atual,
                   c->ataque,
                   c->defesa,
                   c->habilidades[0].nome,
                   c->habilidades[0].poder,
                   c->habilidades[1].nome,
                   c->habilidades[1].poder) >= 0;
}

static int ler_criatura_batalha(char *linha, const char *tag_esperada, Criatura *lista, int *indice_out, Criatura *criatura_out) {
    char *campos[13];
    if (!dividir_csv(linha, campos, 13) || strcmp(campos[0], tag_esperada) != 0) {
        return 0;
    }

    int indice = 0;
    Criatura c;
    memset(&c, 0, sizeof(Criatura));

    if (!parse_int_faixa(campos[1], 0, TOTAL_CRIATURAS - 1, &indice) ||
        !validar_indice_criatura(lista, indice) ||
        !validar_nome(campos[2]) ||
        !parse_int_faixa(campos[3], NIVEL_MIN, NIVEL_MAX, &c.nivel) ||
        !parse_int_faixa(campos[4], XP_MIN, XP_MAX, &c.xp) ||
        !parse_int_faixa(campos[5], 1, HP_MAX_LIMITE, &c.hp_max) ||
        !parse_int_faixa(campos[6], 0, HP_MAX_LIMITE, &c.hp_atual) ||
        !parse_int_faixa(campos[7], ATR_MIN, ATR_MAX, &c.ataque) ||
        !parse_int_faixa(campos[8], ATR_MIN, ATR_MAX, &c.defesa) ||
        !validar_nome(campos[9]) ||
        !parse_int_faixa(campos[10], PODER_MIN, PODER_MAX, &c.habilidades[0].poder) ||
        !validar_nome(campos[11]) ||
        !parse_int_faixa(campos[12], PODER_MIN, PODER_MAX, &c.habilidades[1].poder)) {
        return 0;
    }

    strncpy(c.nome, campos[2], MAX_NOME - 1);
    c.nome[MAX_NOME - 1] = '\0';
    strncpy(c.habilidades[0].nome, campos[9], MAX_NOME - 1);
    c.habilidades[0].nome[MAX_NOME - 1] = '\0';
    strncpy(c.habilidades[1].nome, campos[11], MAX_NOME - 1);
    c.habilidades[1].nome[MAX_NOME - 1] = '\0';

    if (!validar_criatura_completa(&c)) {
        return 0;
    }

    *indice_out = indice;
    *criatura_out = c;
    return 1;
}

int salvar_batalha(const char *caminho, const EstadoBatalha *estado) {
    if (!caminho || !estado ||
        estado->indice_jogador < 0 || estado->indice_jogador >= TOTAL_CRIATURAS ||
        estado->indice_inimigo < 0 || estado->indice_inimigo >= TOTAL_CRIATURAS ||
        estado->turno < 1 ||
        estado->placar_jogador < 0 || estado->placar_inimigo < 0 ||
        !validar_criatura_completa(&estado->jogador) ||
        !validar_criatura_completa(&estado->inimigo)) {
        printf("Erro: Estado de batalha invalido para salvar\n");
        return ARQ_ERRO;
    }

    FILE *fp = fopen(caminho, "w");
    if (!fp) {
        printf("Erro: Nao foi possivel abrir %s para escrita\n", caminho);
        return ARQ_ERRO;
    }

    int ok = fprintf(fp, "ESTADO,%d,%d,%d\n",
                     estado->turno,
                     estado->placar_jogador,
                     estado->placar_inimigo) >= 0 &&
             escrever_criatura_batalha(fp, "JOGADOR", estado->indice_jogador, &estado->jogador) &&
             escrever_criatura_batalha(fp, "INIMIGO", estado->indice_inimigo, &estado->inimigo);

    if (fclose(fp) != 0 || !ok) {
        return ARQ_ERRO;
    }
    return ARQ_OK;
}

int carregar_batalha(const char *caminho, EstadoBatalha *estado, Criatura *lista) {
    if (!caminho || !estado || !lista) {
        return ARQ_ERRO;
    }

    errno = 0;
    FILE *fp = fopen(caminho, "r");
    if (!fp) {
        if (errno == ENOENT) {
            inicializar_estado_batalha_padrao(estado, lista);
            return ARQ_PADRAO;
        }
        printf("Erro: Falha ao abrir %s\n", caminho);
        return ARQ_ERRO;
    }

    EstadoBatalha temp;
    memset(&temp, 0, sizeof(EstadoBatalha));
    char linha[TAM_LINHA];
    char *campos[4];

    if (ler_linha(fp, linha, sizeof(linha)) <= 0 ||
        !dividir_csv(linha, campos, 4) ||
        strcmp(campos[0], "ESTADO") != 0 ||
        !parse_int_faixa(campos[1], 1, 999999, &temp.turno) ||
        !parse_int_faixa(campos[2], 0, 999999, &temp.placar_jogador) ||
        !parse_int_faixa(campos[3], 0, 999999, &temp.placar_inimigo)) {
        printf("Erro: Cabecalho de save corrompido em %s\n", caminho);
        fclose(fp);
        return ARQ_ERRO;
    }

    if (ler_linha(fp, linha, sizeof(linha)) <= 0 ||
        !ler_criatura_batalha(linha, "JOGADOR", lista, &temp.indice_jogador, &temp.jogador)) {
        printf("Erro: Dados do jogador invalidos em %s\n", caminho);
        fclose(fp);
        return ARQ_ERRO;
    }

    if (ler_linha(fp, linha, sizeof(linha)) <= 0 ||
        !ler_criatura_batalha(linha, "INIMIGO", lista, &temp.indice_inimigo, &temp.inimigo)) {
        printf("Erro: Dados do inimigo invalidos em %s\n", caminho);
        fclose(fp);
        return ARQ_ERRO;
    }

    if (ler_linha(fp, linha, sizeof(linha)) != 0) {
        printf("Erro: Conteudo extra inesperado em %s\n", caminho);
        fclose(fp);
        return ARQ_ERRO;
    }

    fclose(fp);
    *estado = temp;
    return ARQ_OK;
}