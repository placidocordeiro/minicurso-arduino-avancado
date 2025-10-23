#include <LiquidCrystal.h>
#include <avr/interrupt.h> // Para ISRs
#include <avr/sleep.h>     // Para os modos de sono
#include <avr/wdt.h>       // Para o Watchdog Timer

// --- Configuração dos Pinos e do LCD ---
LiquidCrystal lcd(12, 11, 6, 5, 4, 3);
const int pinoBotao = 2; // Botão DEVE estar no pino 2
const int pinoLed = 8;

// --- FSM do Menu ---
enum Estado {
  MENU_PRINCIPAL,
  STATUS_SISTEMA,
  MONITORAMENTO
};
Estado estadoAtual = MENU_PRINCIPAL;

// --- Flags de Interrupção ---
volatile bool g_botaoPressionado = false;
volatile unsigned long g_tempoUltimaInterrupcao = 0; // Agora conta SEGUNDOS do WDT
volatile bool g_wdtDisparou = false;

// --- Nossas próprias variáveis de tempo ---
volatile unsigned long g_segundosDeExecucao = 0;
volatile int g_estadoLed = LOW;

// --- Protótipos ---
void entrarNoEstado(Estado novoEstado);
void setupWatchdogTimer();

// --- ISR do Botão (INT0) ---
void isrBotao() {
  // Apenas levanta a flag.
  g_botaoPressionado = true;
}

// --- ISR do Watchdog Timer ---
ISR(WDT_vect) {
  g_wdtDisparou = true; 
  g_segundosDeExecucao++;
  g_estadoLed = !g_estadoLed;
}

void setup() {
  lcd.begin(16, 2);
  pinMode(pinoBotao, INPUT_PULLUP); 
  pinMode(pinoLed, OUTPUT);
  
  entrarNoEstado(estadoAtual);
  
  // --- Configuração da Interrupção do Botão ---
  attachInterrupt(digitalPinToInterrupt(pinoBotao), isrBotao, FALLING);
  
  // --- Configuração do Watchdog Timer (WDT) ---
  setupWatchdogTimer();
  
  // --- Configuração do Modo de Sono ---
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}

void setupWatchdogTimer() {
  cli(); // Desliga interrupções
  MCUSR &= ~(1 << WDRF);
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  // Define prescaler para 1 segundo
  WDTCSR = (1 << WDIE) | (1 << WDP2) | (1 << WDP1);
  sei(); // Liga interrupções
}

void loop() {
  cli(); // Desliga interrupções para verificar flags
  
  if (g_botaoPressionado) {
    g_botaoPressionado = false;
    sei(); // Re-liga interrupções
    
    // MUDANÇA: Lógica de debounce baseada no WDT (g_segundosDeExecucao)
    // Em vez de millis().
    // Só processa o clique se tiver passado mais de 0 "ticks" do WDT
    // desde o último clique. (Debounce de 1 segundo)
    if (g_segundosDeExecucao - g_tempoUltimaInterrupcao > 0) {  
      
      // Atualiza o tempo do último clique VÁLIDO
      g_tempoUltimaInterrupcao = g_segundosDeExecucao;
      
      Estado proximoEstado;
      switch (estadoAtual) {
        case MENU_PRINCIPAL: proximoEstado = STATUS_SISTEMA; break;
        case STATUS_SISTEMA: proximoEstado = MONITORAMENTO; break;
        case MONITORAMENTO: proximoEstado = MENU_PRINCIPAL; break;
      }
      estadoAtual = proximoEstado;
      entrarNoEstado(estadoAtual);
    }
    
  } else if (g_wdtDisparou) {
    g_wdtDisparou = false;
    sei(); // Re-liga interrupções
    
    // 1. Atualiza o pino do LED
    digitalWrite(pinoLed, g_estadoLed);
    
    // 2. Se estiver na tela de monitoramento, atualiza o LCD
    if (estadoAtual == MONITORAMENTO) {
      lcd.setCursor(0, 1);
      lcd.print("Tempo: ");
      lcd.print(g_segundosDeExecucao);
      lcd.print("s   ");
    }
    
  } else {
    // Nenhuma flag, vamos dormir.
    sei(); // Liga interrupções ANTES de dormir
    sleep_enable();
    sleep_cpu();    // ****** DORME AQUI ******
    sleep_disable();  // ****** ACORDA AQUI ******
  }
}

// --- Funções Auxiliares ---

void entrarNoEstado(Estado novoEstado) {
  lcd.clear();
  lcd.setCursor(0, 0);
  switch (novoEstado) {
    case MENU_PRINCIPAL: lcd.print("Menu Principal"); lcd.setCursor(0, 1); lcd.print("Pressione..."); break;
    case STATUS_SISTEMA: lcd.print("Status do Sistema"); lcd.setCursor(0, 1); lcd.print("Tudo OK!"); break;
    case MONITORAMENTO: lcd.print("Monitoramento"); break;
  }
}