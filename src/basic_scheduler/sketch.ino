// --- Estrutura para agendamento de tarefas ---
typedef struct {
  void (*funcao)();          // Ponteiro para a função da tarefa
  unsigned long intervalo;     // Intervalo de execução
  unsigned long ultimaExecucao; // Último tempo de execução 
} Tarefa;

// --- Pinos dos LEDs ---
const int pinoLed_A = 8;
const int pinoLed_B = 9;

// --- Estados dos LEDs ---
int estadoLed_A = LOW;
int estadoLed_B = LOW;

// --- Funções de protótipo das tarefas ---
void tarefaPiscaLed_A();
void tarefaPiscaLed_B();

// --- Array de tarefas ---
Tarefa tarefas[] = {
  {tarefaPiscaLed_A, 500},   // Tarefa A pisca a cada 500ms
  {tarefaPiscaLed_B, 1200}   // Tarefa B pisca a cada 1200ms
};
const int NUM_TAREFAS = sizeof(tarefas) / sizeof(tarefas[0]);

void setup() {
  pinMode(pinoLed_A, OUTPUT);
  pinMode(pinoLed_B, OUTPUT);
}

void loop() {
  // Nosso Agendador Round-Robin
  unsigned long tempoAtual = millis();
  
  for (int i = 0; i < NUM_TAREFAS; i++) {
    // Verifica se está na hora da tarefa ser executada 
    if (tempoAtual - tarefas[i].ultimaExecucao >= tarefas[i].intervalo) {
      tarefas[i].ultimaExecucao = tempoAtual; // Atualiza o tempo 
      tarefas[i].funcao();                    // Executa a função 
    }
  }
}

// --- Definição das Tarefas ---

void tarefaPiscaLed_A() {
  estadoLed_A = !estadoLed_A;
  digitalWrite(pinoLed_A, estadoLed_A);
}

void tarefaPiscaLed_B() {
  estadoLed_B = !estadoLed_B;
  digitalWrite(pinoLed_B, estadoLed_B);
}
