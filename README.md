# pokeC

Jogo de batalha por turnos entre criaturas em linha de comando, inspirado em Pokémon. O projeto é executado no terminal e conta com criaturas que possuem atributos próprios, habilidades, pontos de vida, experiência e níveis, permitindo enfrentar um adversário controlado pelo computador.

## Como jogar

Ao iniciar o jogo, você verá o menu principal e poderá escolher a opção desejada digitando o número correspondente. Ao começar uma nova partida, você entra em batalha contra o computador:

* **Criaturas e atributos:** Cada criatura possui pontos de vida (`HP`), ataque, defesa e duas habilidades diferentes, cada uma com um valor de poder próprio que influencia o dano causado.

* **Dinâmica dos turnos:** Durante a batalha, você escolhe qual habilidade utilizar. Em seguida, o adversário realiza sua jogada automaticamente, podendo atacar ou recuperar parte da própria vida quando estiver com pouco `HP`.

* **Cálculo de dano e objetivo:** O dano considera o poder da habilidade utilizada, o ataque da criatura atacante e a defesa do alvo. O objetivo é reduzir o `HP` da criatura inimiga a zero antes que a sua seja derrotada.

* **Experiência e níveis:** Ao vencer uma batalha, você recebe **50 pontos de experiência (`XP`)**. Ao acumular **100 `XP`**, a criatura sobe de nível.

## Salvamento do progresso

O progresso das criaturas é armazenado no arquivo `progresso.txt`, que registra os níveis e os pontos de experiência obtidos. Assim, os dados permanecem salvos mesmo após fechar e abrir o jogo novamente.

## Compilação e execução

Para compilar e executar o projeto, é necessário ter o **GCC** instalado no computador.

1. Abra o terminal na pasta onde estão os arquivos do projeto e compile com:

   ```bash
   gcc -Wall -Wextra -g main.c interface.c entidade.c arquivos.c batalha.c -o pokec
   ```

2. Após a compilação, execute o jogo com:

   ```bash
   ./pokec
   ```

O menu principal será exibido no terminal para você começar a jogar.