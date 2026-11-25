#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "batalha.h"
#include "interface.h"
#include "arquivos.h"

void preparar_nova_batalha(EstadoBatalha *estado, Criatura *lista) {
    if (estado == NULL || lista == NULL) {
        return;
    }

    exibir_catalogo_de_criaturas(lista, TOTAL_CRIATURAS);
    printf("Escolha sua criatura (1 a %d): ", TOTAL_CRIATURAS);
    int escolha_jogador = ler_opcao(1, TOTAL_CRIATURAS) - 1;

    if (!validar_indice_criatura(lista, escolha_jogador)) {
        escolha_jogador = 0;
    }

    // Sorteia livremente entre todas as criaturas (0 a TOTAL_CRIATURAS - 1),
    // permitindo inclusive batalha contra o mesmo Pokemon do jogador
    int escolha_inimigo = rand() % TOTAL_CRIATURAS;
    if (!validar_indice_criatura(lista, escolha_inimigo)) {
        escolha_inimigo = 0;
    }

    Criatura *jog = obter_criatura_por_indice(lista, escolha_jogador);
    Criatura *ini = obter_criatura_por_indice(lista, escolha_inimigo);
    if (jog == NULL || ini == NULL) {
        return;
    }

    estado->indice_jogador = escolha_jogador;
    estado->indice_inimigo = escolha_inimigo;
    estado->jogador = *jog;
    estado->inimigo = *ini;
    restaurar_hp_total(&estado->jogador);
    restaurar_hp_total(&estado->inimigo);
    estado->turno = 1;

    (void)remover_batalha_salva(ARQ_CAMINHO_BATALHA);

    printf("O adversario sorteou: %s!\n", obter_nome(&estado->inimigo));
}

int calcular_dano(const Criatura *atacante, const Criatura *alvo, int poder_habilidade) {
    if (atacante == NULL || alvo == NULL) {
        return 1;
    }

    int base = (obter_ataque(atacante) + poder_habilidade) - (obter_defesa(alvo) / 2);
    int variacao = (rand() % 5) - 2; // variacao entre -2 e +2
    int dano = base + variacao;

    if (dano < 1) {
        dano = 1;
    }

    return dano;
}

int executar_turno_jogador(EstadoBatalha *estado) {
    if (estado == NULL) {
        return 0;
    }

    exibir_menu_habilidades(&estado->jogador);
    int opcao = ler_opcao(0, NUM_HABILIDADES);

    if (opcao == 0) {
        int status = salvar_batalha(ARQ_CAMINHO_BATALHA, estado);
        if (status == ARQ_OK) {
            imprimir_mensagem("Partida salva com sucesso! Voltando ao menu principal.");
        } else {
            exibir_erro_arquivo(status, "salvar a partida");
        }
        return 1; // indica pausa/saida
    }

    int escolha = opcao - 1;
    if (!habilidade_pertence_criatura(&estado->jogador, escolha)) {
        escolha = 0;
    }

    int poder = obter_poder_habilidade(&estado->jogador, escolha);
    int dano = calcular_dano(&estado->jogador, &estado->inimigo, poder);
    receber_dano(&estado->inimigo, dano);

    exibir_ataque(
        obter_nome(&estado->jogador),
        obter_nome_habilidade(&estado->jogador, escolha),
        obter_nome(&estado->inimigo),
        dano
    );

    return 0;
}

void executar_turno_inimigo(EstadoBatalha *estado) {
    if (estado == NULL) {
        return;
    }

    Criatura *ia = &estado->inimigo;

    // Sorteia uma habilidade valida da criatura da IA para atacar
    int escolha = rand() % NUM_HABILIDADES;
    if (!habilidade_pertence_criatura(ia, escolha)) {
        escolha = 0;
    }

    int poder = obter_poder_habilidade(ia, escolha);
    int dano = calcular_dano(ia, &estado->jogador, poder);
    receber_dano(&estado->jogador, dano);

    exibir_ataque(
        obter_nome(ia),
        obter_nome_habilidade(ia, escolha),
        obter_nome(&estado->jogador),
        dano
    );
}

int verificar_fim_batalha(EstadoBatalha *estado) {
    if (estado == NULL) {
        return RESULTADO_EM_ANDAMENTO;
    }

    if (criatura_derrotada(&estado->inimigo)) {
        estado->placar_jogador++;
        exibir_resultado_da_batalha(obter_nome(&estado->jogador));
        ganhar_xp(&estado->jogador, XP_POR_VITORIA);
        return RESULTADO_JOGADOR_VENCEU;
    }

    if (criatura_derrotada(&estado->jogador)) {
        estado->placar_inimigo++;
        exibir_resultado_da_batalha(obter_nome(&estado->inimigo));
        return RESULTADO_INIMIGO_VENCEU;
    }

    return RESULTADO_EM_ANDAMENTO;
}

void ganhar_xp(Criatura *criatura, int xp_ganho) {
    if (criatura == NULL || xp_ganho <= 0) {
        return;
    }

    int xp_total = obter_xp(criatura) + xp_ganho;
    int nivel_atual = obter_nivel(criatura);

    while (xp_total >= XP_POR_NIVEL && nivel_atual < NIVEL_MAX) {
        xp_total -= XP_POR_NIVEL;
        nivel_atual++;

        definir_nivel(criatura, nivel_atual);
        aumentar_atributos_nivel(criatura, BONUS_HP_NIVEL, BONUS_ATAQUE_NIVEL, BONUS_DEFESA_NIVEL);
        exibir_subida_de_nivel(obter_nome(criatura), nivel_atual);
    }

    if (xp_total > XP_MAX) {
        xp_total = XP_MAX;
    }
    definir_xp(criatura, xp_total);
}

int iniciar_batalha(EstadoBatalha *estado) {
    if (estado == NULL) {
        return RESULTADO_EM_ANDAMENTO;
    }

    int resultado = verificar_fim_batalha(estado);

    while (resultado == RESULTADO_EM_ANDAMENTO) {
        exibir_status_batalha(&estado->jogador, &estado->inimigo, estado->turno);
        printf("Placar da sessao: Jogador %d x %d Inimigo\n",
               estado->placar_jogador, estado->placar_inimigo);

        if (executar_turno_jogador(estado)) {
            return RESULTADO_PAUSADA;
        }

        resultado = verificar_fim_batalha(estado);
        if (resultado != RESULTADO_EM_ANDAMENTO) {
            break;
        }

        executar_turno_inimigo(estado);
        resultado = verificar_fim_batalha(estado);
        if (resultado != RESULTADO_EM_ANDAMENTO) {
            break;
        }

        exibir_fim_de_turno(estado->turno);
        estado->turno++;
    }

   if (resultado == RESULTADO_JOGADOR_VENCEU || resultado == RESULTADO_INIMIGO_VENCEU) {
        (void)remover_batalha_salva(ARQ_CAMINHO_BATALHA);
    }

    return resultado;
}

void registrar_progresso_da_batalha(const EstadoBatalha *estado, Criatura *lista) {
    if (estado == NULL || lista == NULL) {
        return;
    }

    Criatura *original = obter_criatura_por_indice(lista, estado->indice_jogador);
    if (original == NULL) {
        return;
    }

    int nivel_atual = obter_nivel(original);
    int nivel_batalha = obter_nivel(&estado->jogador);
    int progrediu = (nivel_batalha > nivel_atual) || (nivel_batalha == nivel_atual && obter_xp(&estado->jogador) >= obter_xp(original));
    if (progrediu) {
        *original = estado->jogador;
        restaurar_hp_total(original);
    }
}