#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

const int pinoBotao = 2;
const int pinoLed = 8;
// --- Flags de Interrupção --- 
volatile bool g_acordarPeloBotao = false;
volatile bool g_acordarPeloWDT = false;

// --- ISR do Botão (INT0) ---
void isrBotao() {
  // Apenas levanta a flag 
  g_acordarPeloBotao = true;
}

// --- ISR do Watchdog Timer ---
ISR(WDT_vect) {
  g_acordarPeloWDT = true; // Avisa ao loop para acordar
}

void setup() {
  pinMode(pinoLed, OUTPUT);
  pinMode(pinoBotao, INPUT_PULLUP);
  
  // Configura a interrupção do botão
  attachInterrupt(digitalPinToInterrupt(pinoBotao), isrBotao, FALLING);
  
  // Configura o WDT
  setupWatchdogTimer();
  
  // Configura o Modo de Sono
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}

void setupWatchdogTimer() {
  cli();
  MCUSR &= ~(1 << WDRF);
  WDTCSR |= (1 << WDCE) | (1 << WDE); 
  // Configura WDT para interrupção a cada 1 segundo 
  WDTCSR = (1 << WDIE) | (1 << WDP2) | (1 << WDP1);
  sei();
}

void loop() {
  cli(); // Desliga interrupções para verificar flags 
  
  if (g_acordarPeloBotao) {
    g_acordarPeloBotao = false;
    sei(); // Liga interrupções
    
    // Ação: Acende o LED
    digitalWrite(pinoLed, HIGH);
    
  } else if (g_acordarPeloWDT) {
    g_acordarPeloWDT = false;
    sei(); // Liga interrupções
    
    // Ação: Apaga o LED
    digitalWrite(pinoLed, LOW);
    
  } else {
    // Nenhuma flag, vamos dormir.
    sei();
    sleep_enable();
    sleep_cpu();    // ****** DORME AQUI ****** 
    sleep_disable();  // ****** ACORDA AQUI ****** 
  }
}