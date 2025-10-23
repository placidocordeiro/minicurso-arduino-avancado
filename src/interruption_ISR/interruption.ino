#include <LiquidCrystal.h>
#include <avr/interrupt.h>

LiquidCrystal lcd(12, 11, 6, 5, 4, 3);
const int pinoBotao = 2;
const int pinoLed = 8;

enum Estado {
  MENU_PRINCIPAL,
  STATUS_SISTEMA,
  MONITORAMENTO
};
Estado estadoAtual = MENU_PRINCIPAL;

volatile bool g_botaoPressionado = false;
volatile unsigned long g_tempoUltimaInterrupcao = 0;
volatile bool g_estadoAnteriorBotao = HIGH;  // puxa para alto com resistor interno

unsigned long tempoAnteriorDisplay = 0;
const long intervaloDisplay = 250;

void entrarNoEstado(Estado novoEstado);
void atualizarDisplayMonitoramento(unsigned long tempoAtual);

void isrBotao() {
  unsigned long tempoAtual = millis();
  bool estadoAtualBotao = digitalRead(pinoBotao);

  // debounce simples
  if (tempoAtual - g_tempoUltimaInterrupcao > 100) {
    // detecta transição de ALTO -> BAIXO (apertar)
    if (g_estadoAnteriorBotao == HIGH && estadoAtualBotao == LOW) {
      g_botaoPressionado = true;
    }
    g_tempoUltimaInterrupcao = tempoAtual;
  }

  g_estadoAnteriorBotao = estadoAtualBotao;
}

ISR(TIMER1_COMPA_vect) {
  PINB |= (1 << PB0);
}

void setup() {
  lcd.begin(16, 2);
  pinMode(pinoBotao, INPUT_PULLUP);  // usa resistor interno
  pinMode(pinoLed, OUTPUT);

  entrarNoEstado(estadoAtual);

  attachInterrupt(digitalPinToInterrupt(pinoBotao), isrBotao, CHANGE);

  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 31249;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);
  sei();
}

void loop() {
  unsigned long tempoAtual = millis();

  if (g_botaoPressionado) {
    g_botaoPressionado = false;

    switch (estadoAtual) {
      case MENU_PRINCIPAL: estadoAtual = STATUS_SISTEMA; break;
      case STATUS_SISTEMA: estadoAtual = MONITORAMENTO; break;
      case MONITORAMENTO: estadoAtual = MENU_PRINCIPAL; break;
    }
    entrarNoEstado(estadoAtual);
  }

  if (estadoAtual == MONITORAMENTO)
    atualizarDisplayMonitoramento(tempoAtual);
}

void entrarNoEstado(Estado novoEstado) {
  lcd.clear();
  lcd.setCursor(0, 0);
  switch (novoEstado) {
    case MENU_PRINCIPAL: lcd.print("Menu Principal"); lcd.setCursor(0, 1); lcd.print("Pressione..."); break;
    case STATUS_SISTEMA: lcd.print("Status do Sistema"); lcd.setCursor(0, 1); lcd.print("Tudo OK!"); break;
    case MONITORAMENTO: lcd.print("Monitoramento"); break;
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
