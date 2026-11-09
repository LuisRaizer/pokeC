#include <stdio.h>
#include <stdlib.h>
#include "interface.h"
#include "entidade.h"

int main(void) {
    int opcao;
    Criatura* lista_criaturas = NULL;
    
    imprimir_mensagem("Bem-vindo ao pokeC!");

    do {
        exibir_menu_inicial();
        opcao = ler_opcao(1, 4);
        
        switch(opcao) {
            case 1:
                imprimir_mensagem("Nova partida...");
                lista_criaturas = inicializar_criaturas(); 
                
                if (lista_criaturas != NULL) {
                    printf("Criatura 1: %s (HP: %d)\n", 
                           lista_criaturas[0].nome, 
                           lista_criaturas[0].hp_atual);
                    
                    liberar_criaturas(lista_criaturas);
                    lista_criaturas = NULL;
                }
                break;
                
            case 2:
                exibir_instrucoes();
                break;
                
            case 3:
                imprimir_mensagem("Em desenvolvimento...");
                break;
                
            case 4:
                imprimir_mensagem("Saindo...");
                break;
        }
    } while (opcao != 4);
    
    return 0;
}