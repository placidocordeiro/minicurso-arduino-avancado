# 🚀 Minicurso Arduino Avançado: Sistemas Embarcados de Tempo Real

Este repositório contém o material do **Minicurso de Arduino Avançado** ministrado por Plácido Cordeiro durante a SECOMP UFAL, focando em técnicas profissionais para desenvolvimento de sistemas não-bloqueantes e de tempo real.

## 🎯 Objetivo do Curso

Dominar técnicas avançadas para desenvolvimento de sistemas embarcados de tempo real, substituindo abordagens convencionais baseadas em `delay()` por métodos profissionais que garantem responsividade e eficiência.

---

## 📚 Estrutura do Minicurso

### **Módulo 1: Display LCD e Interface Básica**
- Controle direto de display LCD (sem I2C)
- Desenvolvimento de menu interativo com botão

### **Módulo 2: Problemas com Abordagens Bloqueantes**
- Demonstração prática dos limites do `delay()`
- Cronômetro com LED piscante - abordagem ingênua
- Análise de complexidade e manutenibilidade

### **Módulo 3: Transição para Tempo Real com `millis()`**
- Implementação não-bloqueante usando `millis()`
- Gerenciamento de múltiplas tarefas temporizadas
- Lógica de debounce para entradas

### **Módulo 4: Máquinas de Estado Finitas (FSM)**
- Modelagem de sistemas com FSM
- Definição de estados e transições
- Implementação prática no sistema base

### **Módulo 5: Escalonamento de Tarefas**
- Agendador Round Robin
- Array de tarefas com temporização precisa
- Verificação cíclica no loop principal

### **Módulo 6: Interrupções de Hardware**
- Uso de pinos de interrupção
- Priorização de tarefas críticas
- ISR (Interrupt Service Routines)

### **Módulo 7: Otimização de Energia**
- Sleep modes e Wake-up sources
- Watchdog timer para tarefas periódicas
- Estratégias de baixo consumo

---

## 🔧 Pré-requisitos

**Conhecimentos:**
- Conhecimentos básicos de Arduino e eletrônica
- Familiaridade com programação em C++

**Hardware:**
- Placa Arduino Uno
- Display LCD 16x2
- Push button (botão de pressão)
- Potenciômetro de 10kΩ (para contraste do display)
- Resistor de 10kΩ (para pull-down)
- Resistor de 220Ω (para LED e pino do display)
- Protoboard e jumpers

**Software:**
- Arduino IDE
- *Opcional:* Tinkercad ou Wokwi para simulação

---

## 📂 Estrutura do Repositório

```
src/
├── 1_delay/              # Módulo 2: Abordagem bloqueante
├── 2_millis/             # Módulo 3: Transição para tempo real
├── 3_fsm/                # Módulo 4: Máquinas de Estado Finitas
├── 4_scheduler/          # Módulo 5: Escalonamento de tarefas
├── 5_interruptions/      # Módulo 6: Interrupções de hardware
└── 6_sleepmode/          # Módulo 7: Otimização de energia
```

---

## 💡 Metodologia

**Desenvolvimento incremental do mesmo sistema:** Todos os módulos aplicam diferentes técnicas no mesmo sistema base, permitindo comparar evolução e benefícios de cada abordagem.

**Resultado esperado:** Domínio prático de técnicas RTOS-like sem uso de sistema operacional real, preparando para projetos profissionais em sistemas embarcados.

---

## 🚀 Como Usar

1. **Clone o repositório:**
   ```bash
   git clone https://github.com/placidocordeiro/minicurso-arduino-avancado.git
   ```

2. **Navegue pelos módulos:**
   - Os códigos estão organizados sequencialmente na pasta `src/`
   - Cada pasta contém a implementação completa do sistema com a técnica correspondente

3. **Execute progressivamente:**
   - Comece pelo módulo 1 e avance sequencialmente
   - Compare o mesmo sistema implementado com diferentes técnicas
   - Observe a evolução na responsividade e organização do código

---

## 📝 Licença

O **código-fonte** está sob licença **[MIT License](LICENSE)**.

O **conteúdo didático e documentação** estão sob licença **[Creative Commons Attribution 4.0 International (CC BY 4.0)](https://creativecommons.org/licenses/by/4.0/)**.

Você é livre para usar, modificar e compartilhar o material, desde que atribua o crédito ao autor original.
