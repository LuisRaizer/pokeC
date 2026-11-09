#include <stdio.h>
#include <stdlib.h>
#include "interface.h"

void imprimir_mensagem(const char* msg) {
    printf("\n========================================\n");
    printf(" %s\n", msg);
    printf("========================================\n\n");
}

void exibir_menu_inicial(void) {
    printf("\n");
    printf("              _        ____  \n");
    printf("  _ __   ___ | | _____/ ___| \n");
    printf(" | '_ \\ / _ \\| |/ / _ \\ |    \n");
    printf(" | |_) | (_) |   <  __/ |___ \n");
    printf(" | .__/ \\___/|_|\\_\\___|\\____|\n");
    printf(" |_|                         \n");
    printf("\n");
    printf("1. Nova Partida\n");
    printf("2. Instrucoes\n");
    printf("3. Carregar Jogo\n");
    printf("4. Sair\n");
}

void exibir_instrucoes(void) {
    imprimir_mensagem("INSTRUCOES");
    printf("- Escolha sua criatura e lute contra o PC.\n");
    printf("- Cada criatura tem duas habilidades.\n");
    printf("- O objetivo e zerar o HP do adversario.\n");
    printf("\nPressione ENTER para voltar...\n");
    getchar();
}

int ler_opcao(int min, int max) {
    int opcao, lido;
    do {
        printf("Escolha uma opcao (%d-%d): ", min, max);
        lido = scanf("%d", &opcao);
        
        while (getchar() != '\n'); 
        
        if (lido != 1 || opcao < min || opcao > max) {
            imprimir_mensagem("Opcao invalida!");
        }
    } while (lido != 1 || opcao < min || opcao > max);
    
    return opcao;
}