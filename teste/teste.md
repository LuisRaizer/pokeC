## Relatório de Testes – pokeC

### Testes realizados

1. **Opção inválida no menu:** Ao digitar uma opção inexistente, o jogo informou que a escolha era inválida e solicitou uma nova entrada.

2. **Escolha inválida de habilidade:** Ao informar uma habilidade inexistente durante o turno, o jogo avisou que a opção era inválida e pediu outra escolha.

3. **Cálculo de dano:** Ataques foram executados para verificar os danos aplicados, confirmando variações de valor entre tentativas.

4. **Vitória e gravação do progresso:** Após vencer uma batalha, o jogador recebeu 50 pontos de `XP` e o progresso foi gravado corretamente no arquivo.

5. **Derrota:** O jogo identificou quando o `HP` da criatura do jogador chegou a zero e encerrou a batalha normalmente.

6. **Arquivo corrompido:** Foi testado o comportamento do jogo diante de um arquivo de salvamento inválido, verificando que o programa seguiu em execução sem encerrar inesperadamente.

Além desses cenários, também foram verificados o acúmulo de experiência e a subida de nível das criaturas.

### Resultado

No geral, os testes obtiveram bons resultados e as funcionalidades avaliadas se comportaram conforme o esperado, ajudando a identificar ajustes necessários e confirmar o funcionamento das principais partes do jogo.