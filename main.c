#include <stdio.h>
#include <stdlib.h>
#include "interface.h"
#include "entidade.h"
#include "arquivos.h"
#include "batalha.h"

int main(void) {
    int opcao;
    Criatura* lista_criaturas = inicializar_criaturas();
    EstadoBatalha estado_batalha = {0};

    if (lista_criaturas == NULL) {
        return 1;
    }

    int status_prog = carregar_progresso("progresso.txt", lista_criaturas, TOTAL_CRIATURAS);
    if (status_prog == ARQ_PADRAO) {
        salvar_progresso("progresso.txt", lista_criaturas, TOTAL_CRIATURAS);
    }

    imprimir_mensagem("Bem-vindo ao pokeC!");

    do {
        exibir_menu_inicial();
        opcao = ler_opcao(1, 4);

        switch (opcao) {
            case 1:
                imprimir_mensagem("Nova partida...");
                preparar_nova_batalha(&estado_batalha, lista_criaturas);
                if (iniciar_batalha(&estado_batalha) == RESULTADO_JOGADOR_VENCEU) {
                    registrar_progresso_da_batalha(&estado_batalha, lista_criaturas);
                    if (salvar_progresso("progresso.txt", lista_criaturas, TOTAL_CRIATURAS) != ARQ_OK) {
                        imprimir_mensagem("Nao foi possivel salvar o progresso.");
                    }
                }
                break;

            case 2:
                exibir_instrucoes();
                break;

            case 3: {
                int status_save = carregar_batalha("save.txt", &estado_batalha, lista_criaturas);
                if (status_save == ARQ_OK) {
                    imprimir_mensagem("Jogo carregado com sucesso!");
                    printf("Turno %d | Placar %d x %d | %s (HP: %d/%d)\n",
                           estado_batalha.turno,
                           estado_batalha.placar_jogador,
                           estado_batalha.placar_inimigo,
                           estado_batalha.jogador.nome,
                           estado_batalha.jogador.hp_atual,
                           estado_batalha.jogador.hp_max);
                } else if (status_save == ARQ_PADRAO) {
                    imprimir_mensagem("Nenhum save encontrado. Estado padrao iniciado.");
                } else {
                    imprimir_mensagem("Falha ao carregar save.txt (arquivo corrompido).");
                }
                break;
            }

            case 4:
                salvar_progresso("progresso.txt", lista_criaturas, TOTAL_CRIATURAS);
                imprimir_mensagem("Saindo...");
                break;
        }
    } while (opcao != 4);

    liberar_criaturas(lista_criaturas);
    return 0;
}