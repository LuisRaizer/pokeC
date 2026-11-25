#include <stdio.h>
#include <assert.h>
#include "teste_aux.h"

#define ARQ_STDIN_TMP   "teste_stdin_tmp.txt"
#define ARQ_STDOUT_TMP  "teste_stdout_tmp.txt"

void preparar_entrada_stdin(const char *conteudo) {
    FILE *f = fopen(ARQ_STDIN_TMP, "w");
    assert(f != NULL);
    fputs(conteudo, f);
    fclose(f);

    FILE *redirecionado = freopen(ARQ_STDIN_TMP, "r", stdin);
    assert(redirecionado != NULL);
}

void silenciar_saida_padrao(void) {
    FILE *silencio = freopen(ARQ_STDOUT_TMP, "w", stdout);
    assert(silencio != NULL);
}

void gravar_arquivo_bruto(const char *caminho, const char *conteudo) {
    FILE *f = fopen(caminho, "w");
    assert(f != NULL);
    fputs(conteudo, f);
    fclose(f);
}

void limpar_arquivos_temporarios(void) {
    remove(ARQ_STDIN_TMP);
    remove(ARQ_STDOUT_TMP);
    remove("teste_save_tmp.txt");
    remove("teste_progresso_tmp.txt");
    remove("teste_arquivo_inexistente_999.txt");
}