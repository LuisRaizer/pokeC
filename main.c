#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "interface.h"
#include "entidade.h"
#include "arquivos.h"
#include "batalha.h"

int main(void) {
    srand((unsigned int)time(NULL));

    Criatura *lista_criaturas = inicializar_criaturas();
    if (lista_criaturas == NULL) {
        imprimir_mensagem("Nao foi possivel preparar o jogo por falta de memoria.");
        return 1;
    }

    EstadoBatalha estado_batalha;
    inicializar_estado_batalha_padrao(&estado_batalha, lista_criaturas);

    int status_prog = carregar_progresso("progresso.txt", lista_criaturas, TOTAL_CRIATURAS);
    if (status_prog == ARQ_PADRAO) {
        if (salvar_progresso("progresso.txt", lista_criaturas, TOTAL_CRIATURAS) != ARQ_OK) {
            exibir_erro_arquivo(ARQ_ERRO, "criar o arquivo de progresso");
        }
    } else if (status_prog == ARQ_ERRO) {
        exibir_erro_arquivo(status_prog, "carregar o progresso das criaturas");
    }

    imprimir_mensagem("Bem-vindo ao pokeC!");

    int opcao = 0;
    do {
        exibir_menu_inicial();
        opcao = ler_opcao(1, 5);

        switch (opcao) {
            case 1:
                imprimir_mensagem("Nova partida...");
                preparar_nova_batalha(&estado_batalha, lista_criaturas);
                if (iniciar_batalha(&estado_batalha) == RESULTADO_JOGADOR_VENCEU) {
                    registrar_progresso_da_batalha(&estado_batalha, lista_criaturas);
                    int st = salvar_progresso("progresso.txt", lista_criaturas, TOTAL_CRIATURAS);
                    if (st != ARQ_OK) {
                        exibir_erro_arquivo(st, "salvar o progresso");
                    }
                }
                break;

            case 2:
                exibir_instrucoes();
                break;

            case 3: {
                int status_save = carregar_batalha("save.txt", &estado_batalha, lista_criaturas);
                exibir_tela_carregar_partida(status_save, &estado_batalha);

                if (status_save == ARQ_OK) {
                    printf("Deseja retomar esta batalha agora? (1 - Sim | 2 - Voltar ao menu): ");
                    int retomar = ler_opcao(1, 2);
                    if (retomar == 1) {
                        if (iniciar_batalha(&estado_batalha) == RESULTADO_JOGADOR_VENCEU) {
                            registrar_progresso_da_batalha(&estado_batalha, lista_criaturas);
                            int st = salvar_progresso("progresso.txt", lista_criaturas, TOTAL_CRIATURAS);
                            if (st != ARQ_OK) {
                                exibir_erro_arquivo(st, "salvar o progresso");
                            }
                        }
                    }
                }
                break;
            }

            case 4:
                exibir_tela_progresso(lista_criaturas, TOTAL_CRIATURAS);
                break;

            case 5: {
                int st = salvar_progresso("progresso.txt", lista_criaturas, TOTAL_CRIATURAS);
                if (st != ARQ_OK) {
                    exibir_erro_arquivo(st, "salvar o progresso ao sair");
                }
                imprimir_mensagem("Saindo do pokeC. Ate a proxima!");
                break;
            }
        }
    } while (opcao != 5);

    liberar_criaturas(lista_criaturas);
    return 0;
}