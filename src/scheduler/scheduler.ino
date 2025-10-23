#include <LiquidCrystal.h>

// --- Configuração dos Pinos e do LCD ---
LiquidCrystal lcd(12, 11, 6, 5, 4, 3);
const int pinoBotao = 2;
const int pinoLed = 8;

// --- Estrutura para agendamento de tarefas ---
typedef struct {
  void (*funcao)();             // Ponteiro para a função da tarefa
  unsigned long intervalo;      // Intervalo de execução em milissegundos
  unsigned long ultimaExecucao; // Última vez que a tarefa foi executada
} Tarefa;

// --- Definição de funções para mitigar erros no escopo do código ---
void tarefaLed();
void tarefaBotao();
void tarefaDisplay();

// --- Variáveis Globais ---
int telaAtual = 0;
int estadoBotaoAnterior = LOW;
int estadoLed = LOW;

// --- Array de tarefas ---
Tarefa tarefas[] = {
  {tarefaLed, 2000},        // Executa a cada 2 segundos
  {tarefaBotao, 50},        // Verifica o botão a cada 50ms (debounce)
  {tarefaDisplay, 250},     // Atualiza o display a cada 250ms (mais que suficiente)
};

// Calcula o número de tarefas automaticamente
const int NUM_TAREFAS = sizeof(tarefas) / sizeof(tarefas[0]);

// --- Função de configuração inicial do sistema ---
void setup() {
  lcd.begin(16, 2);
  pinMode(pinoBotao, INPUT_PULLUP);
  pinMode(pinoLed, OUTPUT);
  
  // Exibe o menu inicial uma vez ao ligar
  exibirMenu(); 
}

void loop() {
  // Nosso Agendador Round-Robin
  unsigned long tempoAtual = millis();
  
  // Percorre a lista de tarefas para ver qual precisa ser executada
  for (int i = 0; i < NUM_TAREFAS; i++) {
    // Verifica se está na hora da tarefa ser executada
    if (tempoAtual - tarefas[i].ultimaExecucao >= tarefas[i].intervalo) {
      tarefas[i].ultimaExecucao = tempoAtual; // Atualiza o tempo da última execução
      tarefas[i].funcao();                    // Executa a função da tarefa
    }
  }
}

// --- Definição das Tarefas ---
void tarefaLed() {
  estadoLed = !estadoLed;
  digitalWrite(pinoLed, estadoLed);
}

void tarefaBotao() {
  int estadoBotaoAtual = digitalRead(pinoBotao);
  
  // Deteta a borda de subida (momento exato em que o botão é pressionado)
  if (estadoBotaoAtual == LOW && estadoBotaoAnterior == HIGH) {
    telaAtual = (telaAtual + 1) % 3; // Circula entre 0, 1 e 2
    
    // Chamamos a função de exibir o menu diretamente daqui!
    // Este é o único momento em que o menu precisa ser completamente redesenhado.
    exibirMenu();
  }
  estadoBotaoAnterior = estadoBotaoAtual;
}

void tarefaDisplay() {
  // Esta tarefa só faz algo se estivermos na tela de monitoramento
  if (telaAtual == 2) {
    lcd.setCursor(0, 1);
    lcd.print("Tempo: ");
    lcd.print(millis() / 1000);
    lcd.print("s   "); // Adiciona espaços para limpar caracteres antigos
  }
}

void exibirMenu() {
  lcd.clear();          // Limpa o ecrã completamente
  lcd.setCursor(0, 0);  // Posiciona o cursor no início

  switch (telaAtual) {
    case 0:
      lcd.print("Menu Principal");
      lcd.setCursor(0, 1);
      lcd.print("Pressione...");
      break;
    case 1:
      lcd.print("Status do Sistema");
      lcd.setCursor(0, 1);
      lcd.print("Tudo OK!");
      break;
    case 2:
      lcd.print("Monitoramento");
      // A linha de baixo (tempo) será escrita pela tarefaDisplay
      break;
  }
}