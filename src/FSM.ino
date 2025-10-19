#include <LiquidCrystal.h>

// --- Configuração dos Pinos e do LCD ---
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int pinoBotao = 7;
const int pinoLed = 8;

// --- Definição dos Estados da Máquina de Estados ---
enum Estado {
  MENU_PRINCIPAL,
  STATUS_SISTEMA,
  MONITORAMENTO
};

// --- Variáveis Globais ---
Estado estadoAtual = MENU_PRINCIPAL; // O sistema começa no estado do menu principal

// Variáveis para o controle do LED (não-bloqueante)
unsigned long tempoAnteriorLed = 0;
const long intervaloLed = 2000;
int estadoLed = LOW;

// Variáveis para o controle do botão (debounce)
unsigned long tempoAnteriorBotao = 0;
const long intervaloBotao = 50;
int estadoBotaoAnterior = LOW;

// Variáveis para a atualização do display (não-bloqueante)
unsigned long tempoAnteriorDisplay = 0;
const long intervaloDisplay = 250;

// --- Protótipos das Funções ---
// Adicionar protótipos também é uma boa prática em C/C++ para evitar erros de ordem.
void entrarNoEstado(Estado novoEstado);
void gerenciarLed();
void verificarBotao();
void atualizarDisplayMonitoramento();

// --- Função de configuração inicial do sistema ---
void setup() {
  lcd.begin(16, 2);
  pinMode(pinoBotao, INPUT);
  pinMode(pinoLed, OUTPUT);
  
  // Ao iniciar, preparamos a entrada no estado inicial
  entrarNoEstado(estadoAtual); 
}

// --- Função principal (loop) ---
void loop() {
  unsigned long tempoAtual = millis();
  // --- Tarefas que rodam em PARALELO e INDEPENDENTE do estado atual ---
  gerenciarLed(tempoAtual);
  verificarBotao(tempoAtual);
  
  // --- Ações que dependem do ESTADO ATUAL ---
  // Este switch é o coração da nossa Máquina de Estados
  switch (estadoAtual) {
    case MENU_PRINCIPAL:
      // Nenhuma ação contínua é necessária neste estado
      break;
      
    case STATUS_SISTEMA:
      // Nenhuma ação contínua é necessária neste estado
      break;
      
    case MONITORAMENTO:
      // A única ação contínua é atualizar o tempo no ecrã
      atualizarDisplayMonitoramento(tempoAtual);
      break;
  }
}

// --- Funções Auxiliares ---
void gerenciarLed(unsigned long tempoAtual) {
  if (tempoAtual - tempoAnteriorLed >= intervaloLed) {
    tempoAnteriorLed = tempoAtual;
    estadoLed = !estadoLed;
    digitalWrite(pinoLed, estadoLed);
  }
}

void verificarBotao(unsigned long tempoAtual) {
  // Verifica se o tempo de debounce passou
  if (tempoAtual - tempoAnteriorBotao >= intervaloBotao) {
    tempoAnteriorBotao = tempoAtual;
    
    // Verifica se o botão foi pressionado
    int estadoBotaoAtual = digitalRead(pinoBotao);
    if (estadoBotaoAtual == HIGH && estadoBotaoAnterior == LOW) {
      // Evento detectado: Botão foi pressionado!
      // Define a transição para o próximo estado
      Estado proximoEstado;
      switch (estadoAtual) {
        case MENU_PRINCIPAL:
          proximoEstado = STATUS_SISTEMA;
          break;
        case STATUS_SISTEMA:
          proximoEstado = MONITORAMENTO;
          break;
        case MONITORAMENTO:
          proximoEstado = MENU_PRINCIPAL;
          break;
      }
      
      // Atualiza o estado e executa a ação de entrada no novo estado
      estadoAtual = proximoEstado;
      entrarNoEstado(estadoAtual);
    }
    estadoBotaoAnterior = estadoBotaoAtual;
  }
}


void entrarNoEstado(Estado novoEstado) {
  lcd.clear();
  lcd.setCursor(0, 0);

  switch (novoEstado) {
    case MENU_PRINCIPAL:
      lcd.print("Menu Principal");
      lcd.setCursor(0, 1);
      lcd.print("Pressione...");
      break;
    case STATUS_SISTEMA:
      lcd.print("Status do Sistema");
      lcd.setCursor(0, 1);
      lcd.print("Tudo OK!");
      break;
    case MONITORAMENTO:
      lcd.print("Monitoramento");
      // A linha de tempo será gerida pela função 'atualizarDisplayMonitoramento()'
      break;
  }
}

void atualizarDisplayMonitoramento(unsigned long tempoAtual) {
  if (tempoAtual - tempoAnteriorDisplay >= intervaloDisplay) {
    tempoAnteriorDisplay = tempoAtual;
    lcd.setCursor(0, 1);
    lcd.print("Tempo: ");
    lcd.print(millis() / 1000);
    lcd.print("s   ");
  }
}