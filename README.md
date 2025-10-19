# 🚀 Minicurso: Tópicos Avançados de Programação com Arduino

Bem-vindo ao repositório do minicurso de Arduino Avançado! Este material foi criado para explorar técnicas de programação que vão além do básico, permitindo que você crie projetos mais eficientes, responsivos e complexos.

## 🎯 Visão Geral do Curso

O objetivo deste curso é demonstrar, de forma prática, diferentes maneiras de gerenciar tarefas e o fluxo de execução em um microcontrolador. Vamos sair do tradicional `delay()` e mergulhar em abordagens que evitam o bloqueio do processador, como o uso de `millis()`, máquinas de estado e um agendador de tarefas simples.

Todos os exemplos utilizam um menu de navegação em um display LCD, controlado por um botão, e um LED que pisca a cada 2 segundos independentemente, para ilustrar como as diferentes técnicas afetam a interatividade do sistema.

## 🔧 Pré-requisitos

Antes de começar, certifique-se de que você possui:

**Software:**
* [Arduino IDE](https://www.arduino.cc/en/software) ou [Tinkercad](https://www.tinkercad.com/) (para simulação)

**Hardware:**
* 1x Placa Arduino Uno
* 1x Display LCD 16x2
* 1x Push button (botão de pressão)
* 1x Potenciômetro de 10kΩ (para contraste do display)
* 1x Resistor de 10kΩ (para pull-down)
* 2x Resistor de 220Ω (para LED e pino do display)
* Protoboard e jumpers

## 📂 Estrutura do Repositório

Todo o código-fonte dos exemplos pode ser encontrado no diretório `src/`.

```
.
└── src/
    ├── delay.ino      # Exemplo 1: Abordagem bloqueante com delay()
    ├── millis.ino     # Exemplo 2: Abordagem não-bloqueante com millis()
    ├── FSM.ino        # Exemplo 3: Organização com Máquina de Estados Finitos (FSM)
    └── scheduler.ino  # Exemplo 4: Gerenciamento com Agendador de Tarefas
```

## 💡 Os Exemplos

Cada arquivo `.ino` representa uma evolução na forma de controlar o tempo e as tarefas no Arduino.

---

### 1. Sistema com `delay()`

-   **Arquivo:** [`delay.ino`](src/delay.ino)

Este é o ponto de partida clássico. O código utiliza a função `delay()` para criar pausas no programa.

-   **O que observar:** Note como a chamada `delay(1000)` **trava completamente** o processador. Enquanto o programa espera, ele não consegue ler o botão, atualizar o display ou realizar qualquer outra tarefa. Isso torna a interação com o menu lenta e pouco responsiva.
-   **Conceito-chave:** Programação **bloqueante**.

---

### 2. Sistema com `millis()`

-   **Arquivo:** [`millis.ino`](src/millis.ino)

Aqui, abandonamos o `delay()` e adotamos uma abordagem baseada no tempo, utilizando a função `millis()`. Ela retorna o número de milissegundos desde que a placa foi ligada.

-   **O que observar:** O `loop()` agora executa livremente, sem pausas. O código constantemente verifica se já passou tempo suficiente para executar uma tarefa (como piscar um LED ou atualizar o LCD). A leitura do botão se torna instantânea, resultando em um menu totalmente responsivo.
-   **Conceito-chave:** Programação **não-bloqueante**.

---

### 3. Máquina de Estados Finitos (FSM - Finite State Machine)

-   **Arquivo:** [`FSM.ino`](src/FSM.ino)

Esta abordagem organiza o código em "estados" distintos (ex: `MENU_PRINCIPAL`, `STATUS_SISTEMA`, `MONITORAMENTO`). O programa sabe exatamente em qual estado está e quais ações deve executar, tornando a lógica mais clara e fácil de expandir.

-   **O que observar:** A estrutura do código é muito mais organizada. Em vez de uma série de `if/else` complexos, temos um `switch/case` que gerencia o estado atual e as transições. Isso é ideal para projetos que possuem um fluxo complexo de operações.
-   **Conceito-chave:** Organização de código e controle de fluxo.

---

### 4. Agendador de Tarefas (Scheduler)

-   **Arquivo:** [`scheduler.ino`](src/scheduler.ino)

Este é o exemplo mais avançado. Ele implementa um "agendador" cooperativo simples, que é uma estrutura de software responsável por gerenciar e executar múltiplas tarefas independentes em intervalos de tempo específicos.

-   **O que observar:** O código define diferentes tarefas (piscar LED, ler botão, atualizar tela) e o agendador cuida de executá-las quando for a hora certa. Essa abordagem é a base dos Sistemas Operacionais de Tempo Real (RTOS) e é extremamente poderosa para gerenciar projetos complexos de forma organizada e eficiente.
-   **Conceito-chave:** Multitarefa cooperativa.

## 🚀 Como Usar

1.  **Clone o repositório:**
    ```bash
    git clone [https://github.com/seu-usuario/seu-repositorio.git](https://github.com/seu-usuario/seu-repositorio.git)
    ```
2.  **Monte o circuito** conectando o display LCD e o botão ao Arduino.
3.  **Abra um dos arquivos `.ino`** na sua Arduino IDE.
4.  **Compile e envie** o código para a sua placa.
5.  **Teste e modifique!** A melhor forma de aprender é experimentar.

---

Espero que este minicurso seja útil para a sua jornada no mundo dos sistemas embarcados!
