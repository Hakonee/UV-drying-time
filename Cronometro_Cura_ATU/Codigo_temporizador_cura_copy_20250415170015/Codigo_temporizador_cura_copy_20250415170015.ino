#include <LiquidCrystal.h>

// Inicializa o LCD Keypad Shield nos pinos corretos
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

const int relayPin = 2;       // Pino do relé
const int buttonPin = 3;      // Pino do botão externo
const int greenLedPin = 13;   // LED verde (contagem não iniciada)
const int redLedPin = 11;     // LED vermelho (contagem ativa)
const int yellowLedPin = 12;  // LED amarelo (tampa aberta)
const int buzzerPin = A1;     // Pino pro fone/speaker
int minutes = 0;              // Tempo inicial em minutos
int seconds = 0;              // Tempo inicial em segundos
int remainingTime;            // Tempo restante para contagem regressiva (segundos)
int totalTime;                // Tempo total inicial para cálculo da barra
bool running = false;
bool paused = false;
bool buttonPaused = false;    // Indica pausa causada pelo botão externo
bool settingMinutes = true;   // Alterna entre configuração de minutos e segundos
int menuState = 0;            // Estado do menu
int preDefinedTimeIndex = 0;  // Índice de tempo pré-definido
int preDefinedTimes[8][2] = { 
  {1, 0}, {1, 10}, {1, 15}, {1, 25}, 
  {1, 40}, {2, 0}, {3, 20}, {3, 50} 
};

// Caractere personalizado para a barra de progresso (bloco cheio)
byte progressBlock[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

void setup() {
  lcd.begin(16, 2);  
  pinMode(relayPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);  // Botão com pull-up interno
  pinMode(greenLedPin, OUTPUT);      // LED verde
  pinMode(redLedPin, OUTPUT);        // LED vermelho
  pinMode(yellowLedPin, OUTPUT);     // LED amarelo
  pinMode(buzzerPin, OUTPUT);        // Fone/speaker
  digitalWrite(relayPin, LOW);       // Relé inicia desligado
  digitalWrite(greenLedPin, LOW);    // LEDs começam apagados
  digitalWrite(redLedPin, LOW);
  digitalWrite(yellowLedPin, LOW);

  // Cria o caractere personalizado para a barra
  lcd.createChar(0, progressBlock);

  // Mensagem inicial do desenvolvedor com música épica e LEDs (parte 1)
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Desenvolvido por");
  lcd.setCursor(0, 1);
  lcd.print("Juan Henrique");
  // Primeira parte da melodia (~2,5s) com LEDs
  tone(buzzerPin, 500, 100); digitalWrite(greenLedPin, HIGH); delay(150); digitalWrite(greenLedPin, LOW);
  tone(buzzerPin, 700, 100); digitalWrite(redLedPin, HIGH); delay(150); digitalWrite(redLedPin, LOW);
  tone(buzzerPin, 900, 150); digitalWrite(yellowLedPin, HIGH); delay(200); digitalWrite(yellowLedPin, LOW);
  tone(buzzerPin, 1200, 200); digitalWrite(greenLedPin, HIGH); delay(250); digitalWrite(greenLedPin, LOW);
  tone(buzzerPin, 600, 100); digitalWrite(redLedPin, HIGH); delay(150); digitalWrite(redLedPin, LOW);
  tone(buzzerPin, 800, 150); digitalWrite(yellowLedPin, HIGH); delay(200); digitalWrite(yellowLedPin, LOW);
  tone(buzzerPin, 1000, 200); digitalWrite(greenLedPin, HIGH); digitalWrite(redLedPin, HIGH); digitalWrite(yellowLedPin, HIGH); delay(200); digitalWrite(greenLedPin, LOW); digitalWrite(redLedPin, LOW); digitalWrite(yellowLedPin, LOW);
  delay(1050); // Completa ~3s

  // Mensagem do Instagram com música épica e LEDs (parte 2)
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Siga no Instagram:");
  lcd.setCursor(0, 1);
  lcd.print("@juanhqsilva");
  // Segunda parte da melodia (~3s) com LEDs
  tone(buzzerPin, 1300, 300); digitalWrite(greenLedPin, HIGH); digitalWrite(redLedPin, HIGH); delay(200); digitalWrite(greenLedPin, LOW); digitalWrite(redLedPin, LOW);
  tone(buzzerPin, 1000, 150); digitalWrite(yellowLedPin, HIGH); delay(200); digitalWrite(yellowLedPin, LOW);
  tone(buzzerPin, 800, 150); digitalWrite(greenLedPin, HIGH); delay(200); digitalWrite(greenLedPin, LOW);
  tone(buzzerPin, 600, 100); digitalWrite(redLedPin, HIGH); delay(150); digitalWrite(redLedPin, LOW);
  tone(buzzerPin, 900, 200); digitalWrite(yellowLedPin, HIGH); delay(250); digitalWrite(yellowLedPin, LOW);
  tone(buzzerPin, 1100, 200); digitalWrite(greenLedPin, HIGH); delay(250); digitalWrite(greenLedPin, LOW);
  tone(buzzerPin, 1400, 300); digitalWrite(greenLedPin, HIGH); digitalWrite(redLedPin, HIGH); digitalWrite(yellowLedPin, HIGH); delay(200); digitalWrite(greenLedPin, LOW); digitalWrite(redLedPin, LOW); digitalWrite(yellowLedPin, LOW);
  delay(250); // Completa ~3s

  digitalWrite(greenLedPin, digitalRead(buttonPin) == LOW ? HIGH : LOW);  // Reforça LED verde
  displayMenuIntro();
}

void loop() {
  int button = readButton();  // Botões do keypad

  // Atualiza LEDs
  updateLeds();

  if (menuState == 0) {
    if (button == 2) {  // UP - vai para Tempo Personalizado
      menuState = 1;
      minutes = 0;
      seconds = 0;
      settingMinutes = true;
      tone(buzzerPin, 800, 50);  // Bipe de UP
      updateLCD();
    } else if (button == 3) {  // DOWN - vai para Tempos Pré-Definidos
      menuState = 2;
      preDefinedTimeIndex = 0;
      tone(buzzerPin, 700, 50);  // Bipe de DOWN
      updateLCD();
    }
  } 
  else if (menuState == 1 || menuState == 2) {
    if (button == 2 && !running) {  // UP - aumenta o tempo
      bool validAction = false;
      if (menuState == 1) {
        if (settingMinutes) {
          minutes++;
          validAction = true;
        } else if (seconds < 59) {
          seconds++;
          validAction = true;
        }
      } else {
        preDefinedTimeIndex = (preDefinedTimeIndex + 1) % 8;
        validAction = true;
      }
      if (validAction) tone(buzzerPin, 800, 50);  // Bipe de UP
      updateLCD();
      delay(300);
    } else if (button == 3 && !running) {  // DOWN - diminui o tempo
      bool validAction = false;
      if (menuState == 1) {
        if (settingMinutes && minutes > 0) {
          minutes--;
          validAction = true;
        } else if (!settingMinutes && seconds > 0) {
          seconds--;
          validAction = true;
        }
      } else {
        preDefinedTimeIndex = (preDefinedTimeIndex - 1 + 8) % 8;
        validAction = true;
      }
      if (validAction) tone(buzzerPin, 700, 50);  // Bipe de DOWN
      updateLCD();
      delay(300);
    } else if (button == 5 && !running) {  // SELECT - tenta iniciar
      if (digitalRead(buttonPin) == LOW) {  // Tampa fechada
        tone(buzzerPin, 1000, 50);  // Bipe de SELECT
        if (menuState == 1) {
          startRelay();
        } else {
          minutes = preDefinedTimes[preDefinedTimeIndex][0];
          seconds = preDefinedTimes[preDefinedTimeIndex][1];
          startRelay();
        }
      } else {  // Tampa aberta
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Feche a Tampa");
        lcd.setCursor(0, 1);
        lcd.print("para iniciar!!");
        // Bipe de erro: 2 bipes graves
        tone(buzzerPin, 400, 300);
        delay(400);
        tone(buzzerPin, 400, 300);
        delay(2000);
        updateLCD();
      }
    } else if (button == 1 && running) {  // RIGHT - pausa/resume
      togglePause();
    } else if (button == 4 && running) {  // LEFT - cancelar
      cancelRelay();
    } else if (button == 1 && !running && menuState == 1) {  // RIGHT - alterna minutos/segundos
      settingMinutes = !settingMinutes;
      tone(buzzerPin, 900, 50);  // Bipe de RIGHT
      updateLCD();
      delay(300);
    } else if (button == 4 && !running) {  // LEFT - volta ao Menu Principal
      menuState = 0;
      minutes = 0;
      seconds = 0;
      tone(buzzerPin, 600, 50);  // Bipe de LEFT
      displayMenuIntro();
    }
  }

  // Monitora a tampa durante a contagem
  static bool lastTampaState = LOW;
  bool currentTampaState = digitalRead(buttonPin);
  if (running && !paused) {
    if (currentTampaState == HIGH && lastTampaState == LOW) {  // Tampa aberta
      buttonPaused = true;
      paused = true;
      digitalWrite(relayPin, LOW);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Pausado: Tampa");
      lcd.setCursor(0, 1);
      lcd.print("aberta");
      // Quatro bipes alternados
      tone(buzzerPin, 500, 200);
      delay(350);
      tone(buzzerPin, 700, 200);
      delay(350);
      tone(buzzerPin, 500, 200);
      delay(350);
      tone(buzzerPin, 700, 200);
    }
    countdown();
  } else if (running && buttonPaused && currentTampaState == HIGH) {  // Tampa ainda aberta
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Pausado: Tampa");
    lcd.setCursor(0, 1);
    lcd.print("aberta");
    delay(100);
  } else if (running && buttonPaused && currentTampaState == LOW) {  // Tampa fechada
    buttonPaused = false;
    paused = false;
    digitalWrite(relayPin, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Resumindo...");
    delay(1000);
  }
  lastTampaState = currentTampaState;

  // Atualiza o LCD se não pausado
  if (running && !paused && !buttonPaused) {
    countdown();
  }
}

void updateLeds() {
  // LED Amarelo: Pisca se tampa aberta
  static unsigned long lastBlink = 0;
  const unsigned long blinkInterval = 500;
  static bool ledState = false;

  if (digitalRead(buttonPin) == HIGH) {
    if (millis() - lastBlink >= blinkInterval) {
      ledState = !ledState;
      digitalWrite(yellowLedPin, ledState ? HIGH : LOW);
      lastBlink = millis();
    }
  } else {
    digitalWrite(yellowLedPin, LOW);
    ledState = false;
  }

  // LED Verde: Contagem não iniciada e tampa fechada
  if (!running && digitalRead(buttonPin) == LOW) {
    digitalWrite(greenLedPin, HIGH);
  } else {
    digitalWrite(greenLedPin, LOW);
  }

  // LED Vermelho: Contagem ativa
  digitalWrite(redLedPin, running && !paused && !buttonPaused ? HIGH : LOW);
}

bool readExternalButton() {
  static bool lastState = HIGH;
  static unsigned long lastDebounceTime = 0;
  const unsigned long debounceDelay = 50;

  bool reading = digitalRead(buttonPin);
  if (reading != lastState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    lastState = reading;
    return !reading;
  }
  return !lastState;
}

void displayMenuIntro() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Menu Principal");
  // Som de "Power Up"
  tone(buzzerPin, 1000, 100);
  delay(150);
  tone(buzzerPin, 1200, 100);
  delay(150);
  delay(700); // Completa ~1s
  updateLCD();
}

void updateLCD() {
  lcd.clear();
  if (menuState == 0) {
    lcd.setCursor(0, 0);
    lcd.print("1: Personalizado");
    lcd.setCursor(0, 1);
    lcd.print("2: Predefinidos");
  } else if (menuState == 1) {
    lcd.setCursor(0, 0);
    lcd.print("Tempo: ");
    lcd.print(minutes);
    lcd.print(" min ");
    lcd.print(seconds);
    lcd.print(" s");
    lcd.setCursor(0, 1);
    if (settingMinutes) {
      lcd.print("Ajustando min");
    } else {
      lcd.print("Ajustando seg");
    }
  } else if (menuState == 2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tempo:");
    lcd.print(preDefinedTimes[preDefinedTimeIndex][0]);
    lcd.print("m ");
    lcd.print(preDefinedTimes[preDefinedTimeIndex][1]);
    lcd.print("s");
    lcd.setCursor(0, 1);
    lcd.print("Selecione tempo");
  }
}

void startRelay() {
  running = true;
  paused = false;
  buttonPaused = false;
  remainingTime = (minutes * 60) + seconds;
  totalTime = remainingTime;
  digitalWrite(relayPin, HIGH);
  // Três bipes subindo
  tone(buzzerPin, 800, 150);
  delay(250);
  tone(buzzerPin, 1000, 150);
  delay(250);
  tone(buzzerPin, 1200, 150);
  countdown();
}

void countdown() {
  static unsigned long lastUpdate = 0;
  static bool milestones[3] = {false, false, false};  // 25%, 50%, 75%
  static bool finalBeeps[5] = {false, false, false, false, false};  // 5s, 4s, 3s, 2s, 1s
  if (millis() - lastUpdate >= 1000 && !paused && !buttonPaused) {
    lastUpdate = millis();
    if (remainingTime > 0) {
      lcd.clear();
      String timeStr = String(remainingTime / 60) + " min " + String(remainingTime % 60) + " s";
      int len = timeStr.length();
      int col = (16 - len) / 2;
      lcd.setCursor(col, 0);
      lcd.print(timeStr);
      if (totalTime > 0) {
        float progress = (float)(totalTime - remainingTime) / totalTime;
        int blocks = round(progress * 12);
        lcd.setCursor(1, 1);
        lcd.print("[");
        for (int i = 0; i < 12; i++) {
          if (i < blocks) lcd.write((uint8_t)0);
          else lcd.print("_");
        }
        lcd.print("]");
        // Bipes progressivos
        if (progress >= 0.25 && !milestones[0]) {
          tone(buzzerPin, 800, 100);
          milestones[0] = true;
        }
        if (progress >= 0.50 && !milestones[1]) {
          tone(buzzerPin, 900, 100);
          milestones[1] = true;
        }
        if (progress >= 0.75 && !milestones[2]) {
          tone(buzzerPin, 1000, 100);
          milestones[2] = true;
        }
      }
      // Bipes de reta final (5s a 1s)
      if (remainingTime == 5 && !finalBeeps[0]) {
        tone(buzzerPin, 1100, 100);
        finalBeeps[0] = true;
      }
      if (remainingTime == 4 && !finalBeeps[1]) {
        tone(buzzerPin, 1100, 100);
        finalBeeps[1] = true;
      }
      if (remainingTime == 3 && !finalBeeps[2]) {
        tone(buzzerPin, 1100, 100);
        finalBeeps[2] = true;
      }
      if (remainingTime == 2 && !finalBeeps[3]) {
        tone(buzzerPin, 1100, 100);
        finalBeeps[3] = true;
      }
      if (remainingTime == 1 && !finalBeeps[4]) {
        tone(buzzerPin, 1100, 100);
        finalBeeps[4] = true;
      }
      remainingTime--;
    } else {
      digitalWrite(relayPin, LOW);
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Finalizado!");
      // Melodia de fim
      tone(buzzerPin, 800, 200);
      delay(300);
      tone(buzzerPin, 1200, 200);
      delay(300);
      tone(buzzerPin, 1600, 200);
      delay(300);
      tone(buzzerPin, 2000, 400);
      delay(2000);
      running = false;
      menuState = 0;
      minutes = 0;
      seconds = 0;
      milestones[0] = milestones[1] = milestones[2] = false;  // Reseta progressivos
      finalBeeps[0] = finalBeeps[1] = finalBeeps[2] = finalBeeps[3] = finalBeeps[4] = false;  // Reseta reta final
      displayMenuIntro();
    }
  }
}

void cancelRelay() {
  running = false;
  paused = false;
  buttonPaused = false;
  digitalWrite(relayPin, LOW);
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Cancelado!");
  // Bipe de cancelamento
  tone(buzzerPin, 600, 200);
  delay(300);
  tone(buzzerPin, 400, 200);
  delay(2000);
  menuState = 0;
  minutes = 0;
  seconds = 0;
  displayMenuIntro();
}

void togglePause() {
  paused = !paused;
  buttonPaused = false;
  lcd.clear();
  lcd.setCursor(4, 0);
  if (paused) {
    lcd.print("Pausado");
    digitalWrite(relayPin, LOW);
    tone(buzzerPin, 1000, 300);  // Bipe de pausa
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Resumindo...");
    digitalWrite(relayPin, HIGH);
    tone(buzzerPin, 1000, 150);  // Bipe de retomar
    delay(250);
    tone(buzzerPin, 1200, 150);
  }
  delay(1000);
}

int readButton() {
  int adcKeyVal = analogRead(A0);
  if (adcKeyVal < 50)   return 1;  // RIGHT
  if (adcKeyVal < 200)  return 2;  // UP
  if (adcKeyVal < 400)  return 3;  // DOWN
  if (adcKeyVal < 600)  return 4;  // LEFT
  if (adcKeyVal < 850)  return 5;  // SELECT
  return 0;
}