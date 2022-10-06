## Projeto 1 - Multithreaded Game of Life and High Life

Este projeto tem como objetivo analisar o ganho de desempenho ao se utilizar o paralelismo em um programa que necessita de muito processamento. Foram utilizadas as bibliotecas *OpenMP* e *pthread* para a criação das threads.

<br>

![screen-gif](./results/game%20of%20life%20gif.gif)

<br>

### Game of Life

O Jogo da Vida, criado por John H. Conway, utiliza um autômato celular para simular gerações sucessivas de uma sociedade de organismos vivos.
É composto por um tabuleiro bi-dimensional, infinito em qualquer direção, de células quadradas idênticas. Cada célula tem exatamente oito células vizinhas (todas as células que compartilham, com a célula original, uma aresta ou um vértice). Cada célula está em um de dois estados: viva ou morta (correspondentes aos valores 1 ou 0). Uma geração da sociedade é representada pelo conjunto dos estados das células do tabuleiro.
Sociedades evoluem de uma geração para a próxima aplicando simultaneamente, a todas as células do tabuleiro, regras que estabelecem o próximo estado de cada célula. As regras são:

1) Qualquer célula viva com 2 (dois) ou 3 (três) vizinhos deve sobreviver;
2) Qualquer célula morta com 3 (três) vizinhos torna-se viva;
3) Qualquer outro caso, células vivas devem morrer e células já mortas devem continuar mortas.


### High Life

Alternativamente ao Jogo do Vida existe uma versão do mesmo conhecida por HighLife, que tem uma única diferença na regra 2, conforme se vê em destaque a seguir:

1) Qualquer célula viva com 2 (dois) ou 3 (três) vizinhos deve sobreviver;
2) Qualquer célula morta com 3 (três) ou 6 (seis) vizinhos torna-se viva;
3) Qualquer outro caso, células vivas devem morrer e células já mortas devem continuar mortas.

## Resultados

Os algoritmos foram rodados em um computador com um processador [AMD Ryzen 5 5600X](https://www.amd.com/pt/products/cpu/amd-ryzen-5-5600x), que possui 6 núcleos físicos, com capacidade para multithreading, resultando em 12 núcleos lógicos.

Foi utilizado um tabuleiro de dimensões 2048x2048, com dois objetos iniciais e simuladas 2000 gerações.

Os testes foram executados 5 vezes para cada algoritmo. O melhor e pior resultados foram descartados, e o tempo final de execução é a média das outras 3 execuções restantes.

<br>


#### Game of Life

![Game of Life Results](results/Game%20of%20Life.png "Game of Life Results")

![Game of Life speedup and efficiency](results/Game%20of%20Life%20table.png "Game of Life speedup and efficiency")

![Game of Life OpenMP](results/Game%20of%20Life%20-%20OpenMP.png "Game of Life OpenMP")

![Game of Life pthread](results/Game%20of%20Life%20-%20pthread.png "Game of Life pthread")

![Game of Life OpenMP vs pthread](results/Game%20of%20Life%20-%20OpenMP%20vs%20pthread%20.png "Game of Life OpenMP vs pthread")

<br>

#### High Life

![High Life Results](results/High%20Life.png "High Life Results")

![High Life speedup and efficiency](results/High%20Life%20table.png "High Life speedup and efficiency")

![High Life OpenMP](results/HighLife%20-%20OpenMP.png "High Life OpenMP")

![High Life pthread](results/High%20Life%20-%20pthread.png "High Life pthread")

![High Life OpenMP vs pthread](results/High%20Life%20-%20OpenMP%20vs%20pthread%20.png "High Life OpenMP vs pthread")

<br>

### OpenMP

![OpenMP](results/OpenMP%20-%20Game%20of%20Life%20vs%20High%20Life.png "OpenMP")

<br>

### pthread

![pthread](results/pthread%20-%20Game%20of%20Life%20vs%20High%20Life.png "pthread")


