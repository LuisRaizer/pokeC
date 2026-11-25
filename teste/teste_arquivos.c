#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../entidade.h"
#include "../arquivos.h"
#include "teste_aux.h"

#define ARQ_SAVE_TMP    "teste_save_tmp.txt"
#define ARQ_PROG_TMP    "teste_progresso_tmp.txt"
#define ARQ_INEXISTENTE "teste_arquivo_inexistente_999.txt"

static void testar_arquivo_inexistente(void) {
    remove(ARQ_INEXISTENTE);

    Criatura *lista = inicializar_criaturas();
    assert(lista != NULL);

    EstadoBatalha estado;
    assert(existe_batalha_salva(ARQ_INEXISTENTE) == 0);
    assert(remover_batalha_salva(ARQ_INEXISTENTE) == ARQ_PADRAO);
    assert(carregar_progresso(ARQ_INEXISTENTE, lista, TOTAL_CRIATURAS) == ARQ_PADRAO);
    assert(carregar_batalha(ARQ_INEXISTENTE, &estado, lista) == ARQ_PADRAO);
    assert(estado.turno == 1);
    assert(obter_hp_atual(&estado.jogador) > 0);

    liberar_criaturas(lista);
    fprintf(stderr, "[OK] Arquivos: arquivos inexistentes tratados com fallback ARQ_PADRAO\n");
}

static void testar_salvar_carregar_e_arquivo_corrompido(void) {
    Criatura *lista = inicializar_criaturas();
    assert(lista != NULL);

    EstadoBatalha original;
    inicializar_estado_batalha_padrao(&original, lista);
    original.turno = 4;
    original.placar_jogador = 2;
    original.placar_inimigo = 1;
    original.jogador.hp_atual = 63;
    original.inimigo.hp_atual = 41;

    assert(salvar_batalha(ARQ_SAVE_TMP, &original) == ARQ_OK);
    assert(existe_batalha_salva(ARQ_SAVE_TMP) == 1);

    EstadoBatalha carregado;
    memset(&carregado, 0, sizeof(EstadoBatalha));
    assert(carregar_batalha(ARQ_SAVE_TMP, &carregado, lista) == ARQ_OK);
    assert(carregado.turno == 4);
    assert(carregado.placar_jogador == 2);
    assert(carregado.placar_inimigo == 1);
    assert(obter_hp_atual(&carregado.jogador) == 63);
    assert(obter_hp_atual(&carregado.inimigo) == 41);

    /* Arquivo vazio */
    gravar_arquivo_bruto(ARQ_SAVE_TMP, "");
    assert(carregar_batalha(ARQ_SAVE_TMP, &carregado, lista) == ARQ_ERRO);
    assert(carregado.turno == 4);

    /* Cabecalho corrompido */
    gravar_arquivo_bruto(ARQ_SAVE_TMP,
        "99;1;3;0;0\n"
        "Charmander;1;0;95;95;18;12;Brasa;10;Lanca-Chamas;18\n"
        "Bulbassauro;1;0;115;115;14;18;Chicote de Vinha;10;Folha Navalha;16\n");
    assert(carregar_batalha(ARQ_SAVE_TMP, &carregado, lista) == ARQ_ERRO);

    /* Arquivo truncado */
    gravar_arquivo_bruto(ARQ_SAVE_TMP,
        "0;1;3;0;0\n"
        "Charmander;1;0;95;95;18;12;Brasa;10;Lanca-Chamas;18\n");
    assert(carregar_batalha(ARQ_SAVE_TMP, &carregado, lista) == ARQ_ERRO);

    /* HP atual maior que HP maximo */
    gravar_arquivo_bruto(ARQ_SAVE_TMP,
        "0;1;3;0;0\n"
        "Charmander;1;0;95;500;18;12;Brasa;10;Lanca-Chamas;18\n"
        "Bulbassauro;1;0;115;115;14;18;Chicote de Vinha;10;Folha Navalha;16\n");
    assert(carregar_batalha(ARQ_SAVE_TMP, &carregado, lista) == ARQ_ERRO);

    /* Criatura ja derrotada no save */
    gravar_arquivo_bruto(ARQ_SAVE_TMP,
        "0;1;3;0;0\n"
        "Charmander;1;0;95;0;18;12;Brasa;10;Lanca-Chamas;18\n"
        "Bulbassauro;1;0;115;115;14;18;Chicote de Vinha;10;Folha Navalha;16\n");
    assert(carregar_batalha(ARQ_SAVE_TMP, &carregado, lista) == ARQ_ERRO);

    /* Lixo extra no arquivo de progresso */
    gravar_arquivo_bruto(ARQ_PROG_TMP,
        "Charmander;1;0;95;95;18;12;Brasa;10;Lanca-Chamas;18\n"
        "Bulbassauro;1;0;115;115;14;18;Chicote de Vinha;10;Folha Navalha;16\n"
        "Squirtle;1;0;105;105;15;15;Jato d'Agua;11;Hidro Pulso;17\n"
        "Pikachu;1;0;90;90;19;11;Choque do Trovao;12;Raio;19\n"
        "LINHA_CORROMPIDA_EXTRA\n");
    assert(carregar_progresso(ARQ_PROG_TMP, lista, TOTAL_CRIATURAS) == ARQ_ERRO);
    assert(carregado.turno == 4);

    liberar_criaturas(lista);
    fprintf(stderr, "[OK] Arquivos: salvar/carregar e 6 cenarios de arquivo corrompido tratados sem crash\n");
}

void executar_testes_arquivos(void) {
    testar_arquivo_inexistente();
    testar_salvar_carregar_e_arquivo_corrompido();
}