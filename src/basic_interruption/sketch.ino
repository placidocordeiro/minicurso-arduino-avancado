#include <avr/interrupt.h>

const int pinoBotao = 2;
const int pinoLedTimer = 8;  // Este pino é o PB0
const int pinoLedBotao = 9;

// Flag volátil para a interrupção do botão 
volatile bool g_botaoPressionado = false;
volatile unsigned long g_tempoUltimaInterrupcao = 0;
// --- ISR do Botão (INT0) ---
// Usando a lógica de borda de descida (FALLING)
void isrBotao() {
  // Debounce simples dentro da ISR
  if (millis() - g_tempoUltimaInterrupcao > 200) {
    g_botaoPressionado = true;
    g_tempoUltimaInterrupcao = millis();
  }
}

// --- ISR do Timer1 (Pisca LED) ---
ISR(TIMER1_COMPA_vect) {
  // Inverte o pino 8 (PB0) diretamente
  PINB |= (1 << PB0);
}

void setup() {
  pinMode(pinoLedTimer, OUTPUT);
  pinMode(pinoLedBotao, OUTPUT);
  pinMode(pinoBotao, INPUT_PULLUP); 
  
  // Configura a interrupção do botão
  attachInterrupt(digitalPinToInterrupt(pinoBotao), isrBotao, FALLING);

  // --- Configuração da Interrupção do Timer1 (Pisca 1s) ---
  cli(); // Desliga interrupções
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  // OCR1A = 31249; // Para 2 segundos (do seu código)
  OCR1A = 15624; // Para 1 segundo (16MHz / 1024 * 1s - 1)
  TCCR1B |= (1 << WGM12); // Modo CTC
  TCCR1B |= (1 << CS12) | (1 << CS10); // Prescaler 1024 
  TIMSK1 |= (1 << OCIE1A); // Ativa a interrupção de comparação
  sei(); // Liga interrupções
}

void loop() {
  // 1. O LED do Timer (pino 8) está a piscar sozinho via hardware.
  // O loop() não faz NADA para que ele funcione.

  // 2. O loop() apenas processa a flag da interrupção do botão
  if (g_botaoPressionado) {
    g_botaoPressionado = false; // "Baixa" a flag
    
    // Ação: Inverte o LED do botão
    digitalWrite(pinoLedBotao, !digitalRead(pinoLedBotao));
  }
  
  // O loop principal está livre para fazer outras coisas...
  // (Neste caso, não faz nada).
}
