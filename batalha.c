#include "batalha.h"
#include "interface.h"

int calcular_dano(const Criatura *atacante, const Criatura *defensor, int indice_habilidade) {
    if (!atacante || !defensor || !habilidade_pertence_criatura(atacante, indice_habilidade)) {
        return 0;
    }

    const Habilidade *hab = obter_habilidade_por_indice(atacante, indice_habilidade);
    int dano = hab->poder + atacante->ataque - (defensor->defesa / 2);

    return dano > 0 ? dano : 1;
}

void ganhar_xp(Criatura *criatura, int quantidade) {
    if (!criatura || quantidade <= 0) {
        return;
    }

    int nivel = obter_nivel(criatura);
    long xp = (long)obter_xp(criatura) + quantidade;

    while (xp >= XP_POR_NIVEL && nivel < NIVEL_MAX) {
        xp -= XP_POR_NIVEL;
        nivel++;
        definir_nivel(criatura, nivel);
        exibir_subida_de_nivel(obter_nome(criatura), nivel);
    }

    if (nivel >= NIVEL_MAX) {
        xp = 0;
    }
    definir_xp(criatura, (int)xp);
}

/* ---- Oponente ---- */

static int deve_curar(const Criatura *criatura) {
    return obter_hp_atual(criatura) * 100 < obter_hp_max(criatura) * LIMIAR_CURA_PERCENT;
}

static int quantidade_de_cura(const Criatura *criatura) {
    int q = obter_hp_max(criatura) / 3;
    return q > 0 ? q : 1;
}

static int habilidade_mais_forte(const Criatura *criatura) {
    int melhor = -1;
    for (int i = 0; i < NUM_HABILIDADES; i++) {
        if (!habilidade_pertence_criatura(criatura, i)) {
            continue;
        }
        if (melhor < 0 ||
            obter_poder_habilidade(criatura, i) > obter_poder_habilidade(criatura, melhor)) {
            melhor = i;
        }
    }
    return melhor;
}

/* ---- Turnos ---- */

static void executar_ataque(Criatura *atacante, Criatura *alvo, int indice_habilidade) {
    int dano = calcular_dano(atacante, alvo, indice_habilidade);
    receber_dano(alvo, dano);
    exibir_ataque(obter_nome(atacante),
                  obter_nome_habilidade(atacante, indice_habilidade),
                  obter_nome(alvo),
                  dano);
}

static void turno_do_jogador(EstadoBatalha *estado) {
    int escolha;
    do {
        exibir_menu_habilidades(&estado->jogador);
        escolha = ler_opcao(1, NUM_HABILIDADES) - 1;
    } while (!habilidade_pertence_criatura(&estado->jogador, escolha));

    executar_ataque(&estado->jogador, &estado->inimigo, escolha);
}

static void turno_do_inimigo(EstadoBatalha *estado) {
    Criatura *inimigo = &estado->inimigo;

    if (deve_curar(inimigo)) {
        int antes = obter_hp_atual(inimigo);
        curar_criatura(inimigo, quantidade_de_cura(inimigo));
        exibir_cura(obter_nome(inimigo), obter_hp_atual(inimigo) - antes);
        return;
    }

    int habilidade = habilidade_mais_forte(inimigo);
    if (habilidade >= 0) {
        executar_ataque(inimigo, &estado->jogador, habilidade);
    }
}

/* ---- Batalha ---- */

void preparar_nova_batalha(EstadoBatalha *estado, Criatura *lista) {
    if (!estado || !lista) {
        return;
    }

    int placar_jogador = estado->placar_jogador;
    int placar_inimigo = estado->placar_inimigo;

    inicializar_estado_batalha_padrao(estado, lista);
    estado->jogador.hp_atual = estado->jogador.hp_max;
    estado->inimigo.hp_atual = estado->inimigo.hp_max;

    estado->placar_jogador = placar_jogador;
    estado->placar_inimigo = placar_inimigo;
}

int iniciar_batalha(EstadoBatalha *estado) {
    if (!estado) {
        return RESULTADO_ERRO;
    }

    while (!criatura_derrotada(&estado->jogador) && !criatura_derrotada(&estado->inimigo)) {
        exibir_status_batalha(&estado->jogador, &estado->inimigo, estado->turno);

        turno_do_jogador(estado);
        if (!criatura_derrotada(&estado->inimigo)) {
            turno_do_inimigo(estado);
        }

        if (!criatura_derrotada(&estado->jogador) && !criatura_derrotada(&estado->inimigo)) {
            exibir_fim_de_turno(estado->turno);
            estado->turno++;
        }
    }

    if (criatura_derrotada(&estado->jogador)) {
        estado->placar_inimigo++;
        exibir_resultado_da_batalha(obter_nome(&estado->inimigo));
        return RESULTADO_INIMIGO_VENCEU;
    }

    estado->placar_jogador++;
    exibir_resultado_da_batalha(obter_nome(&estado->jogador));
    ganhar_xp(&estado->jogador, XP_POR_VITORIA);
    return RESULTADO_JOGADOR_VENCEU;
}

void registrar_progresso_da_batalha(const EstadoBatalha *estado, Criatura *lista) {
    if (!estado || !lista) {
        return;
    }
    Criatura *destino = obter_criatura_por_indice(lista, estado->indice_jogador);
    if (!destino) {
        return;
    }
    definir_nivel(destino, obter_nivel(&estado->jogador));
    definir_xp(destino, obter_xp(&estado->jogador));
}