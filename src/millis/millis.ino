#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 6, 5, 4, 3);
const int pinoBotao = 2;
const int pinoLed = 8;

int telaAtual = 0;
const int numeroDeTelas = 3;

int estadoBotao = HIGH;          // estado estável atual
int leituraAnterior = HIGH;      // última leitura crua do pino
unsigned long ultimoDebounce = 0;
const unsigned long tempoDebounce = 50;

unsigned long tempoAnteriorLed = 0;
const long intervaloLed = 2000;
int estadoLed = LOW;

void setup() {
  lcd.begin(16, 2);
  pinMode(pinoBotao, INPUT_PULLUP);
  pinMode(pinoLed, OUTPUT);
  exibirMenu();
}

void loop() {
  unsigned long tempoAtual = millis();

  // --- LED ---
  if (tempoAtual - tempoAnteriorLed >= intervaloLed) {
    tempoAnteriorLed = tempoAtual;
    estadoLed = !estadoLed;
    digitalWrite(pinoLed, estadoLed);
  }

  // --- Leitura com debounce ---
  int leitura = digitalRead(pinoBotao);

  // se mudou, reinicia o temporizador
  if (leitura != leituraAnterior) {
    ultimoDebounce = tempoAtual;
  }

  // se ficou estável por tempoDebounce ms
  if ((tempoAtual - ultimoDebounce) > tempoDebounce) {
    // se o estado estável mudou
    if (leitura != estadoBotao) {
      estadoBotao = leitura;

      // se o botão foi realmente pressionado (LOW, pois há pull-up)
      if (estadoBotao == LOW) {
        telaAtual++;
        if (telaAtual >= numeroDeTelas) telaAtual = 0;
        exibirMenu();
      }
    }
  }

  leituraAnterior = leitura;

  // --- Atualização do display ---
  if (telaAtual == 2) {
    lcd.setCursor(0, 1);
    lcd.print("Tempo: ");
    lcd.print(tempoAtual / 1000);
    lcd.print("s ");
  }
}

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
