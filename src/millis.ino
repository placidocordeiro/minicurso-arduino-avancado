#include <LiquidCrystal.h>

// --- Configuração do Hardware ---
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int pinoBotao = 7;
const int pinoLed = 8;

// --- Variáveis de Controle do Menu ---
int telaAtual = 0;
const int numeroDeTelas = 3;

// --- Variáveis para Controle do Botão (Debounce) ---
int estadoBotaoAnterior = LOW;

// --- Variáveis para o Pisca-LED (sem delay) ---
unsigned long tempoAnteriorLed = 0;
const long intervaloLed = 2000;
int estadoLed = LOW;

// --- Função de configuração inicial do sistema ---
void setup() {
  lcd.begin(16, 2);
  pinMode(pinoBotao, INPUT);
  pinMode(pinoLed, OUTPUT);
  
  exibirMenu();
}

// --- Função principal (loop) ---
void loop() {
  // Pega o tempo atual no início de cada loop. Usamos isso para tudo.
  unsigned long tempoAtual = millis();

  // --- Seção de Lógica do LED (Não bloqueante) ---
  // Verifica se já se passaram 2 segundos desde a última mudança
  if (tempoAtual - tempoAnteriorLed >= intervaloLed) {
    // Se sim, "reseta" o cronômetro do LED salvando o tempo atual
    tempoAnteriorLed = tempoAtual;

    // Inverte o estado do LED
    if (estadoLed == LOW) {
      estadoLed = HIGH;
    } else {
      estadoLed = LOW;
    }

    // Aplica o novo estado ao pino do LED
    digitalWrite(pinoLed, estadoLed);
  }

  // --- Seção de Lógica do Botão (Não bloqueante) ---
  int estadoBotaoAtual = digitalRead(pinoBotao);
  if (estadoBotaoAtual == HIGH && estadoBotaoAnterior == LOW) {
    telaAtual++;
    if (telaAtual >= numeroDeTelas) {
      telaAtual = 0;
    }
    exibirMenu();
    delay(50); // Um pequeno delay aqui é aceitável para o debounce do botão
  }
  estadoBotaoAnterior = estadoBotaoAtual;

  // --- Seção de Lógica do cronômetro do Display (Não bloqueante) ---
  if (telaAtual == 2) {
    lcd.setCursor(0, 1);
    lcd.print("Tempo: ");
    lcd.print(tempoAtual / 1000); // Usamos o tempoAtual que já lemos
    lcd.print("s ");
  }
}

// --- Função para desenhar o menu no LCD ---
void exibirMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);

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
      break;
  }
}