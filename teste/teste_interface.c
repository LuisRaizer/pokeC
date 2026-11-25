#include <stdio.h>
#include <assert.h>
#include "../interface.h"
#include "teste_aux.h"

static void testar_opcao_invalida_menu(void) {
    preparar_entrada_stdin("abc\n0\n6\n-3\n2abc\n4\n");
    int opcao = ler_opcao(1, 5);
    assert(opcao == 4);

    fprintf(stderr, "[OK] Interface: opcao invalida no menu rejeitada ate entrada valida\n");
}

void executar_testes_interface(void) {
    testar_opcao_invalida_menu();
}