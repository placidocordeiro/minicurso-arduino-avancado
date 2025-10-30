#include <LiquidCrystal.h>

// --- Configuração dos Pinos e do LCD ---
LiquidCrystal lcd(12, 11, 6, 5, 4, 3);
const int pinoBotao = 2;
const int pinoLed = 8;

// --- Definição dos Estados da Máquina de Estados ---
enum Estado {
  MENU_PRINCIPAL,
  STATUS_SISTEMA,
  MONITORAMENTO
};

// --- Variáveis Globais ---
Estado estadoAtual = MENU_PRINCIPAL;

// Variáveis para o controle do LED (não-bloqueante)
unsigned long tempoAnteriorLed = 0;
const long intervaloLed = 2000;
int estadoLed = LOW;

// Variáveis para o controle do botão (debounce)
unsigned long tempoAnteriorBotao = 0;
const long intervaloBotao = 50;
int estadoBotaoAnterior = HIGH; // começa em HIGH porque há pull-up interno

// Variáveis para a atualização do display (não-bloqueante)
unsigned long tempoAnteriorDisplay = 0;
const long intervaloDisplay = 250;

// --- Protótipos das Funções ---
void entrarNoEstado(Estado novoEstado);
void gerenciarLed(unsigned long tempoAtual);
void verificarBotao(unsigned long tempoAtual);
void atualizarDisplayMonitoramento(unsigned long tempoAtual);

// --- Configuração inicial ---
void setup() {
  lcd.begin(16, 2);
  pinMode(pinoBotao, INPUT_PULLUP); // usa o pull-up interno
  pinMode(pinoLed, OUTPUT);
  
  entrarNoEstado(estadoAtual);
}

// --- Loop principal ---
void loop() {
  unsigned long tempoAtual = millis();

  gerenciarLed(tempoAtual);
  verificarBotao(tempoAtual);

  switch (estadoAtual) {
    case MENU_PRINCIPAL:
      break;
    case STATUS_SISTEMA:
      break;
    case MONITORAMENTO:
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
  if (tempoAtual - tempoAnteriorBotao >= intervaloBotao) {
    tempoAnteriorBotao = tempoAtual;
    
    int estadoBotaoAtual = digitalRead(pinoBotao);
    if (estadoBotaoAtual == LOW && estadoBotaoAnterior == HIGH) {
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
