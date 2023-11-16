const int buttonPin = 2;
const int buzzerPin = 3;

const int ditDuration = 40; // 0.04 segundos
const int dahDuration = ditDuration * 3; // 0.12 segundos
const int interCharacterDelay = ditDuration; // 1.2 segundos entre caracteres
const int buttonPressTime = 1000; // 1 segundo

unsigned long buttonPressStartTime = 0;
int pressCounter = 0;

char morseAlphabet[][5] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."};

int currentIndex = 0;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {
    if (millis() - buttonPressStartTime >= buttonPressTime) {
      // Executa ações após 1 segundo
      buttonPressStartTime = millis();

      // Conta quantas vezes o botão foi pressionado durante o último segundo
      int pressCount = countButtonPresses();

      // Atualiza o índice com base no número de pressionamentos
      if (pressCount == 1) {
         // Repete o índice
      } else if (pressCount == 2) {
       
        currentIndex = (currentIndex + 1) % (sizeof(morseAlphabet) / sizeof(morseAlphabet[0]));
      } else if (pressCount == 3) {
        currentIndex = (currentIndex - 1 + sizeof(morseAlphabet) / sizeof(morseAlphabet[0])) % (sizeof(morseAlphabet) / sizeof(morseAlphabet[0]));
      }

      // Reproduz a sequência Morse no buzzer
      playMorse(morseAlphabet[currentIndex]);
    }
  }

  delay(50); // Debounce
}

int countButtonPresses() {
  int count = 0;
  unsigned long startTime = millis();

  // Aguarda 1 segundo e conta os pressionamentos durante esse período
  while (millis() - startTime < buttonPressTime) {
    if (digitalRead(buttonPin) == HIGH) {
      delay(50); // Debounce
      if (digitalRead(buttonPin) == HIGH) {
        count++;
        // Aguarda até que o botão seja liberado
        while (digitalRead(buttonPin) == HIGH) {
          delay(50);
        }
      }
    }
    
    delay(50);
  }

  return count;
}

void playMorse(char morseCode[5]) {
  for (int i = 0; i < 5; i++) {
    char currentChar = morseCode[i];

    if (currentChar == '.') {
      beep(ditDuration);
    } else if (currentChar == '-') {
      beep(dahDuration);
    }

    if (i < 4) {
      delay(ditDuration); // Intervalo entre dit e dah
    }
  }

  delay(interCharacterDelay); // Intervalo entre caracteres
}

void beep(int duration) {
  digitalWrite(buzzerPin, HIGH);
  delay(duration);
  digitalWrite(buzzerPin, LOW);
  delay(ditDuration); // Intervalo entre dit e dah
}
