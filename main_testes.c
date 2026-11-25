#include <stdio.h>
#include <stdlib.h>
#include "teste\teste_aux.h"

int main(void) {
    srand(42);
    limpar_arquivos_temporarios();
    silenciar_saida_padrao();

    fprintf(stderr, "=== Executando bateria de testes do pokeC ===\n");

    executar_testes_interface();
    executar_testes_batalha();
    executar_testes_arquivos();

    limpar_arquivos_temporarios();
    fprintf(stderr, "=== Todos os testes passaram com sucesso! ===\n");
    return 0;
}