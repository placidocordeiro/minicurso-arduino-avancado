#include <LiquidCrystal.h>

// --- Configuração do Hardware ---
LiquidCrystal lcd(12, 11, 6, 5, 4, 3);
const int pinoBotao = 2;
const int pinoLed = 8;

// --- Variáveis de Controle do Menu ---
int telaAtual = 0;
const int numeroDeTelas = 3;

// --- Variável para simular o tempo (que não funciona bem com delay) ---
unsigned long tempoSimulado = 0;

// --- Função de configuração inicial do sistema ---
void setup() {
  lcd.begin(16, 2);
  pinMode(pinoBotao, INPUT_PULLUP);
  pinMode(pinoLed, OUTPUT);
  
  exibirMenu();
}

// --- Função principal (loop) ---
void loop() {
  // Liga o LED e trava tudo por 2 segundos
  digitalWrite(pinoLed, HIGH);
  delay(2000); 

  // Após 2 segundos, verifica o botão e atualiza a tela
  verificarBotaoEAtualizarTela();
  
  // Desliga o LED e trava tudo por mais 2 segundos
  digitalWrite(pinoLed, LOW);
  delay(2000);

  // Após mais 2 segundos, verifica e atualiza novamente
  verificarBotaoEAtualizarTela();
}

void verificarBotaoEAtualizarTela() {
  // Verifica se o botão foi pressionado
  int estadoBotao = digitalRead(pinoBotao);
  if (estadoBotao == LOW) {
    // Se sim, avançamos para a próxima tela
    telaAtual++;
    // Se chegamos ao fim, voltamos à primerira tela
    if (telaAtual >= numeroDeTelas) {
      telaAtual = 0;
    }
    exibirMenu();
    
    // Pequeno delay para evitar múltiplas leituras do botão (debounce)
    delay(200); 
  }
  
  tempoSimulado += 2; // Adiciona 2 segundos ao "cronômetro" a cada iteração
  
  // Atualiza o display APENAS se estiver na tela de monitoramento
  if (telaAtual == 2) {
    lcd.setCursor(0, 1);
    lcd.print("Tempo: ");
    lcd.print(tempoSimulado);
    lcd.print("s      "); // Espaços para limpar a linha anterior
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
