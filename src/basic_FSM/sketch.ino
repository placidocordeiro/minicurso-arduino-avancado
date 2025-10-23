const int pinoLed = 8;
const int pinoBotao = 2;

// --- Definição dos Estados da Máquina de Estados --- 
enum EstadoLed {
  ESTADO_DESLIGADO,
  ESTADO_PISCANDO,
  ESTADO_LIGADO
};

// --- Variáveis Globais ---
EstadoLed estadoAtual = ESTADO_DESLIGADO; // Começa desligado
int estadoBotaoAnterior = HIGH; // Começa em HIGH devido ao pull-up

// --- Variáveis para tarefas não-bloqueantes ---
unsigned long tempoAnteriorLed = 0;
const long intervaloPisca = 250;
unsigned long tempoAnteriorBotao = 0;
const long intervaloBotao = 50; 

void setup() {
  pinMode(pinoLed, OUTPUT);
  pinMode(pinoBotao, INPUT_PULLUP);
}

void loop() {
  unsigned long tempoAtual = millis();

  // 1. A função que VERIFICA eventos (transições)
  verificarBotao(tempoAtual);

  // 2. A função que EXECUTA o estado atual
  executarEstado(tempoAtual);
}

void verificarBotao(unsigned long tempoAtual) {
  // Lógica de debounce não-bloqueante 
  if (tempoAtual - tempoAnteriorBotao >= intervaloBotao) {
    tempoAnteriorBotao = tempoAtual;
    int estadoBotaoAtual = digitalRead(pinoBotao);

    // Se o botão foi pressionado (borda de descida)
    if (estadoBotaoAtual == LOW && estadoBotaoAnterior == HIGH) {
      // Faz a TRANSIÇÃO para o próximo estado
      switch (estadoAtual) {
        case ESTADO_DESLIGADO:
          estadoAtual = ESTADO_PISCANDO;
          break;
        case ESTADO_PISCANDO:
          estadoAtual = ESTADO_LIGADO;
          break;
        case ESTADO_LIGADO:
          estadoAtual = ESTADO_DESLIGADO;
          break;
      }
    }
    estadoBotaoAnterior = estadoBotaoAtual;
  }
}

void executarEstado(unsigned long tempoAtual) {
  // Este switch executa a AÇÃO de cada estado
  switch (estadoAtual) {
    case ESTADO_DESLIGADO:
      digitalWrite(pinoLed, LOW);
      break;

    case ESTADO_PISCANDO:
      // Lógica de piscar não-bloqueante 
      if (tempoAtual - tempoAnteriorLed >= intervaloPisca) {
        tempoAnteriorLed = tempoAtual;
        digitalWrite(pinoLed, !digitalRead(pinoLed));
      }
      break;

    case ESTADO_LIGADO:
      digitalWrite(pinoLed, HIGH);
      break;
  }
}