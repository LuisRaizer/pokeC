#include <stdio.h>
#include <assert.h>
#include "../entidade.h"
#include "../batalha.h"
#include "../arquivos.h"
#include "teste_aux.h"

#define ARQ_PROG_TMP "teste_progresso_tmp.txt"

static void testar_escolha_invalida_habilidade(void) {
    Criatura *lista = inicializar_criaturas();
    assert(lista != NULL);

    EstadoBatalha estado;
    inicializar_estado_batalha_padrao(&estado, lista);

    assert(habilidade_pertence_criatura(&estado.jogador, -1) == 0);
    assert(habilidade_pertence_criatura(&estado.jogador, NUM_HABILIDADES) == 0);
    assert(habilidade_pertence_criatura(&estado.jogador, 0) == 1);

    int hp_inicial_inimigo = obter_hp_atual(&estado.inimigo);
    preparar_entrada_stdin("xyz\n-1\n3\n99\n2\n");
    int pausou = executar_turno_jogador(&estado);

    assert(pausou == 0);
    assert(obter_hp_atual(&estado.inimigo) < hp_inicial_inimigo);

    liberar_criaturas(lista);
    fprintf(stderr, "[OK] Batalha: escolha invalida de habilidade tratada sem falha\n");
}

static void testar_formula_dano_e_variacao(void) {
    Criatura *lista = inicializar_criaturas();
    assert(lista != NULL);

    Criatura *charmander = obter_criatura_por_indice(lista, 0);
    Criatura *bulbassauro = obter_criatura_por_indice(lista, 1);
    assert(charmander != NULL && bulbassauro != NULL);

    int min_obs = 9999;
    int max_obs = -9999;
    for (int i = 0; i < 200; i++) {
        int dano = calcular_dano(charmander, bulbassauro, obter_poder_habilidade(charmander, 0));
        assert(dano >= 17 && dano <= 21);
        if (dano < min_obs) min_obs = dano;
        if (dano > max_obs) max_obs = dano;
    }
    assert(min_obs < max_obs);

    Criatura fraco = *charmander;
    Criatura tanque = *bulbassauro;
    fraco.ataque = 1;
    tanque.defesa = 60;
    for (int i = 0; i < 50; i++) {
        assert(calcular_dano(&fraco, &tanque, 0) == 1);
    }

    receber_dano(&tanque, 9999);
    assert(obter_hp_atual(&tanque) == 0);
    assert(criatura_derrotada(&tanque) == 1);

    liberar_criaturas(lista);
    fprintf(stderr, "[OK] Batalha: formula de dano, variacao [-2..+2] e piso minimo 1 validados\n");
}

static void testar_vitoria_e_gravacao_progresso(void) {
    Criatura *lista = inicializar_criaturas();
    assert(lista != NULL);

    EstadoBatalha estado;
    inicializar_estado_batalha_padrao(&estado, lista);

    estado.inimigo.hp_atual = 1;
    receber_dano(&estado.inimigo, 10);

    int res = verificar_fim_batalha(&estado);
    assert(res == RESULTADO_JOGADOR_VENCEU);
    assert(estado.placar_jogador == 1);
    assert(obter_xp(&estado.jogador) == XP_POR_VITORIA);

    registrar_progresso_da_batalha(&estado, lista);
    assert(obter_xp(&lista[0]) == XP_POR_VITORIA);
    assert(obter_hp_atual(&lista[0]) == obter_hp_max(&lista[0]));

    assert(salvar_progresso(ARQ_PROG_TMP, lista, TOTAL_CRIATURAS) == ARQ_OK);

    Criatura *recarregada = inicializar_criaturas();
    assert(recarregada != NULL);
    assert(carregar_progresso(ARQ_PROG_TMP, recarregada, TOTAL_CRIATURAS) == ARQ_OK);
    assert(obter_xp(&recarregada[0]) == XP_POR_VITORIA);
    assert(obter_nivel(&recarregada[0]) == 1);

    liberar_criaturas(lista);
    liberar_criaturas(recarregada);
    fprintf(stderr, "[OK] Batalha: vitoria com ganho de XP e gravacao de progresso validada\n");
}

static void testar_derrota(void) {
    Criatura *lista = inicializar_criaturas();
    assert(lista != NULL);

    EstadoBatalha estado;
    inicializar_estado_batalha_padrao(&estado, lista);

    estado.jogador.hp_atual = 0;
    int res = verificar_fim_batalha(&estado);

    assert(res == RESULTADO_INIMIGO_VENCEU);
    assert(estado.placar_inimigo == 1);
    assert(obter_xp(&estado.jogador) == 0);

    registrar_progresso_da_batalha(&estado, lista);
    assert(obter_xp(&lista[0]) == 0);
    assert(obter_nivel(&lista[0]) == 1);

    liberar_criaturas(lista);
    fprintf(stderr, "[OK] Batalha: derrota contabilizada sem alterar progresso salvo\n");
}

static void testar_xp_acumulado_e_subida_nivel_modesta(void) {
    Criatura *lista = inicializar_criaturas();
    assert(lista != NULL);

    Criatura c = lista[0];

    ganhar_xp(&c, 50);
    assert(obter_nivel(&c) == 1);
    assert(obter_xp(&c) == 50);
    assert(obter_hp_max(&c) == 95);

    ganhar_xp(&c, 50);
    assert(obter_nivel(&c) == 2);
    assert(obter_xp(&c) == 0);
    assert(obter_hp_max(&c) == 95 + BONUS_HP_NIVEL);
    assert(obter_ataque(&c) == 18 + BONUS_ATAQUE_NIVEL);
    assert(obter_defesa(&c) == 12 + BONUS_DEFESA_NIVEL);

    ganhar_xp(&c, 235);
    assert(obter_nivel(&c) == 4);
    assert(obter_xp(&c) == 35);
    assert(obter_hp_max(&c) == 95 + (3 * BONUS_HP_NIVEL));
    assert(obter_ataque(&c) == 18 + (3 * BONUS_ATAQUE_NIVEL));
    assert(obter_defesa(&c) == 12 + (3 * BONUS_DEFESA_NIVEL));

    liberar_criaturas(lista);
    fprintf(stderr, "[OK] Batalha: ganho de XP acumulado e subida de nivel com bonus modestos validados\n");
}

void executar_testes_batalha(void) {
    testar_escolha_invalida_habilidade();
    testar_formula_dano_e_variacao();
    testar_vitoria_e_gravacao_progresso();
    testar_derrota();
    testar_xp_acumulado_e_subida_nivel_modesta();
}