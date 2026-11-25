#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "interface.h"

void exibir_menu_inicial(void) {
    printf("\n==========================================\n");
    printf("                             //           \n");
    printf("  ____     ___     _  __   _____    ____  \n");
    printf(" |  _ \\   / _ \\   | |/ /  | ____|  / ___| \n");
    printf(" | |_) | | | | |  | ' /   |  _|   | |     \n");
    printf(" |  __/  | |_| |  | . \\   | |___  | |___  \n");
    printf(" |_|      \\___/   |_|\\_\\  |_____|  \\____| \n");
    printf("==========================================\n");
    printf("1. Nova partida\n");
    printf("2. Instrucoes\n");
    printf("3. Carregar partida\n");
    printf("4. Ver progresso das criaturas\n");
    printf("5. Sair\n");
    printf("==========================================\n");
    printf("Escolha uma opcao: ");
}

void exibir_instrucoes(void) {
    printf("\n========================================\n");
    printf("               INSTRUCOES               \n");
    printf("========================================\n");
    printf("- Escolha uma criatura para entrar em batalha.\n");
    printf("- A cada turno, escolha uma habilidade para atacar\n");
    printf("  ou digite 0 para salvar e voltar ao menu.\n");
    printf("- Ao vencer batalhas, sua criatura ganha XP e sobe\n");
    printf("  de nivel, ficando mais forte permanentemente.\n");
    printf("- Seu progresso e salvo automaticamente.\n");
    printf("========================================\n");
}

void imprimir_mensagem(const char *msg) {
    if (msg != NULL) {
        printf("\n%s\n", msg);
    }
}

int ler_opcao(int min, int max) {
    char buffer[64];

    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            imprimir_mensagem("Entrada encerrada. Voltando com a opcao padrao.");
            return min;
        }

        size_t len = strlen(buffer);
        int terminou_com_nova_linha = (len > 0 && buffer[len - 1] == '\n');

        if (!terminou_com_nova_linha && !feof(stdin)) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
            }
            printf("Entrada muito longa. Digite um numero entre %d e %d: ", min, max);
            continue;
        }

        if (terminou_com_nova_linha) {
            buffer[len - 1] = '\0';
        }

        errno = 0;
        char *fim = NULL;
        long valor = strtol(buffer, &fim, 10);

        while (*fim == ' ' || *fim == '\t' || *fim == '\r') {
            fim++;
        }

        if (errno == 0 && fim != buffer && *fim == '\0' &&
            valor >= (long)min && valor <= (long)max) {
            return (int)valor;
        }

        if (fim == buffer) {
            printf("Entrada invalida (nao e um numero). Digite um numero entre %d e %d: ", min, max);
        } else if (errno == ERANGE || valor < (long)min || valor > (long)max) {
            printf("Numero fora do intervalo. Digite um numero entre %d e %d: ", min, max);
        } else {
            printf("Entrada invalida. Digite apenas um numero entre %d e %d: ", min, max);
        }
    }
}

void exibir_catalogo_de_criaturas(const Criatura *lista, int total) {
    printf("\n============================================\n");
    printf("           CATALOGO DE CRIATURAS            \n");
    printf("============================================\n");

    if (lista == NULL || total <= 0) {
        printf("Nenhuma criatura disponivel.\n");
        return;
    }

    for (int i = 0; i < total; i++) {
        const Criatura *c = &lista[i];
        printf("%d. %-12s | Nv: %2d | HP: %3d | ATQ: %2d | DEF: %2d\n",
               i + 1,
               obter_nome(c),
               obter_nivel(c),
               obter_hp_max(c),
               obter_ataque(c),
               obter_defesa(c));
    }

    printf("============================================\n");
}

void exibir_resultado_da_batalha(const char *vencedor, int eh_aliado) {
    const char *lado = eh_aliado ? "aliado" : "inimigo";
    const char *nome = (vencedor != NULL) ? vencedor : "Desconhecido";

    printf("\n========================================\n");
    printf("             FIM DA BATALHA             \n");
    printf("========================================\n");
    printf("%s %s ganhou a batalha!\n", nome, lado);
    printf("========================================\n");
}

void exibir_status_batalha(const Criatura *jogador, const Criatura *inimigo, int turno) {
    printf("\n----------------------------------------\n");
    printf("                TURNO %d                \n", turno);
    printf("----------------------------------------\n");
    printf("Jogador : %-12s | Nv %2d | HP: %3d/%3d | XP: %d\n",
           obter_nome(jogador),
           obter_nivel(jogador),
           obter_hp_atual(jogador),
           obter_hp_max(jogador),
           obter_xp(jogador));
    printf("Inimigo : %-12s | Nv %2d | HP: %3d/%3d\n",
           obter_nome(inimigo),
           obter_nivel(inimigo),
           obter_hp_atual(inimigo),
           obter_hp_max(inimigo));
    printf("----------------------------------------\n");
}

void exibir_menu_habilidades(const Criatura *criatura) {
    printf("\nEscolha uma acao para %s:\n", obter_nome(criatura));
    for (int i = 0; i < NUM_HABILIDADES; i++) {
        printf("%d. %-18s (Poder: %d)\n",
               i + 1,
               obter_nome_habilidade(criatura, i),
               obter_poder_habilidade(criatura, i));
    }
    printf("0. Salvar partida e voltar ao menu\n");
    printf("Sua escolha: ");
}

void exibir_ataque(const char *atacante, const char *habilidade, const char *alvo, int dano) {
    printf("\n%s usou %s em %s e causou %d de dano!\n",
           (atacante != NULL) ? atacante : "Criatura",
           (habilidade != NULL) ? habilidade : "Ataque",
           (alvo != NULL) ? alvo : "Alvo",
           dano);
}

void exibir_fim_de_turno(int turno) {
    printf("\n--- Fim do turno %d ---\n", turno);
}

void exibir_subida_de_nivel(const char *nome, int novo_nivel) {
    printf("\n*** %s subiu para o nivel %d! Atributos aumentados! ***\n",
           (nome != NULL) ? nome : "Criatura",
           novo_nivel);
}

void exibir_erro_arquivo(int codigo_status, const char *acao) {
    const char *operacao = (acao != NULL) ? acao : "acessar os dados do jogo";

    if (codigo_status == ARQ_PADRAO) {
        printf("\n[Aviso] Nenhum registro anterior foi encontrado ao %s.\n", operacao);
        printf("O jogo continuara utilizando os dados iniciais.\n");
    } else if (codigo_status == ARQ_ERRO) {
        printf("\n[Erro] Nao foi possivel %s.\n", operacao);
        printf("O arquivo pode estar danificado, incompleto ou sem permissao de acesso.\n");
    }
}

void exibir_tela_carregar_partida(int status_carregamento, const EstadoBatalha *estado) {
    printf("\n========================================\n");
    printf("            CARREGAR PARTIDA            \n");
    printf("========================================\n");

    if (status_carregamento == ARQ_PADRAO) {
        printf("Nenhuma partida salva foi encontrada.\n");
        printf("Inicie uma nova partida e salve durante a batalha para continuar depois.\n");
        printf("========================================\n");
        return;
    }

    if (status_carregamento != ARQ_OK || estado == NULL) {
        printf("Nao foi possivel carregar a partida salva.\n");
        printf("O arquivo de salvamento esta danificado ou incompleto.\n");
        printf("========================================\n");
        return;
    }

    printf("Partida carregada com sucesso!\n\n");
    printf("Turno atual : %d\n", estado->turno);
    printf("Placar      : Jogador %d x %d Inimigo\n\n",
           estado->placar_jogador, estado->placar_inimigo);

    printf("Sua criatura:\n");
    printf("  %s | Nivel %d | HP: %d/%d | XP: %d\n",
           obter_nome(&estado->jogador),
           obter_nivel(&estado->jogador),
           obter_hp_atual(&estado->jogador),
           obter_hp_max(&estado->jogador),
           obter_xp(&estado->jogador));

    printf("Oponente:\n");
    printf("  %s | Nivel %d | HP: %d/%d\n",
           obter_nome(&estado->inimigo),
           obter_nivel(&estado->inimigo),
           obter_hp_atual(&estado->inimigo),
           obter_hp_max(&estado->inimigo));

    printf("========================================\n");
}

void exibir_tela_progresso(const Criatura *lista, int total) {
    printf("\n========================================\n");
    printf("         PROGRESSO DAS CRIATURAS        \n");
    printf("========================================\n");

    if (lista == NULL || total <= 0) {
        printf("Nenhuma criatura disponivel para exibicao.\n");
        printf("========================================\n");
        return;
    }

    for (int i = 0; i < total; i++) {
        const Criatura *c = &lista[i];
        printf("%d. %-12s | Nivel: %3d | XP: %d/100\n",
               i + 1,
               obter_nome(c),
               obter_nivel(c),
               obter_xp(c));
    }

    printf("========================================\n");
}

int confirmar_descarte_save(void) {
    printf("\n[Aviso] Existe uma partida salva em andamento.\n");
    printf("Iniciar uma nova partida ira descartar esse salvamento.\n");
    printf("Deseja continuar mesmo assim? (1 - Sim | 2 - Nao): ");
    return (ler_opcao(1, 2) == 1);
}