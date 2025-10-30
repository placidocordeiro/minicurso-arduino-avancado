#include <LiquidCrystal.h>
#include <avr/interrupt.h>

// pinos do LCD (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 6, 5, 4, 3);

const int pinoBotao = 2; // botão usado com INPUT_PULLUP
const int pinoLed = 8;   // LED ligado em PB0 (digital 8)

enum Estado {
  MENU_PRINCIPAL,
  STATUS_SISTEMA,
  MONITORAMENTO
};
Estado estadoAtual = MENU_PRINCIPAL;

// variáveis atualizadas em ISRs devem ser 'volatile'
volatile bool g_botaoPressionado = false;
volatile unsigned long g_tempoUltimaInterrupcao = 0;
volatile bool g_estadoAnteriorBotao = HIGH;  // botão puxa para alto com resistor interno

unsigned long tempoAnteriorDisplay = 0;
const long intervaloDisplay = 250;

void entrarNoEstado(Estado novoEstado);
void atualizarDisplayMonitoramento(unsigned long tempoAtual);

// ISR associado ao botão: faz debounce simples e detecta borda de descida
void isrBotao() {
  unsigned long tempoAtual = millis();
  bool estadoAtualBotao = digitalRead(pinoBotao);

  // debounce simples (ignorar pulsações rápidas)
  if (tempoAtual - g_tempoUltimaInterrupcao > 100) {
    // detecta transição de HIGH -> LOW (botão pressionado com INPUT_PULLUP)
    if (g_estadoAnteriorBotao == HIGH && estadoAtualBotao == LOW) {
      g_botaoPressionado = true; // sinaliza para o loop principal
    }
    g_tempoUltimaInterrupcao = tempoAtual;
  }

  g_estadoAnteriorBotao = estadoAtualBotao;
}

// Interrupção do Timer1: usa escrita em PINB para inverter o bit PB0
// (isso alterna o estado do pino 8 sem alterar os demais pinos do PORTB)
ISR(TIMER1_COMPA_vect) {
  PINB |= (1 << PB0); // toggle PB0 (digital 8)
}

void setup() {
  lcd.begin(16, 2);
  pinMode(pinoBotao, INPUT_PULLUP);  // usa resistor interno para manter HIGH quando solto
  pinMode(pinoLed, OUTPUT);

  entrarNoEstado(estadoAtual);

  // interrupção externa no pino do botão (CHANGE para detectar ambos os estados)
  attachInterrupt(digitalPinToInterrupt(pinoBotao), isrBotao, CHANGE);

  // configura Timer1 para gerar interrupção periódica
  cli();            // desabilita interrupções globais durante a configuração
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 31249;    // com prescaler 1024 em 16MHz -> interrupção a cada ~2s (compare match)
  TCCR1B |= (1 << WGM12);                     // modo CTC
  TCCR1B |= (1 << CS12) | (1 << CS10);        // prescaler 1024
  TIMSK1 |= (1 << OCIE1A);                    // habilita interrupção de compare A
  sei();                                      // reabilita interrupções globais
}

void loop() {
  unsigned long tempoAtual = millis();

  // se o botão foi pressionado (sinalizado pela ISR), muda o estado
  if (g_botaoPressionado) {
    g_botaoPressionado = false;

    switch (estadoAtual) {
      case MENU_PRINCIPAL: estadoAtual = STATUS_SISTEMA; break;
      case STATUS_SISTEMA: estadoAtual = MONITORAMENTO; break;
      case MONITORAMENTO: estadoAtual = MENU_PRINCIPAL; break;
    }
    entrarNoEstado(estadoAtual);
  }

  // atualização periódica do display no modo de monitoramento
  if (estadoAtual == MONITORAMENTO)
    atualizarDisplayMonitoramento(tempoAtual);
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

// atualiza o segundo linha do LCD a cada intervaloDisplay 
void atualizarDisplayMonitoramento(unsigned long tempoAtual) {
  if (tempoAtual - tempoAnteriorDisplay >= intervaloDisplay) {
    tempoAnteriorDisplay = tempoAtual;
    lcd.setCursor(0, 1);
    lcd.print("Tempo: ");
    lcd.print(millis() / 1000);
    lcd.print("s   ");
  }
}
