#ifndef TESTE_AUX_H
#define TESTE_AUX_H

void preparar_entrada_stdin(const char *conteudo);
void silenciar_saida_padrao(void);
void gravar_arquivo_bruto(const char *caminho, const char *conteudo);
void limpar_arquivos_temporarios(void);

void executar_testes_interface(void);
void executar_testes_batalha(void);
void executar_testes_arquivos(void);

#endif