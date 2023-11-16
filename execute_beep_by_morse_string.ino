const int button_pin = 2;
const int buzzer_pin = 3;

const int dit_duration = 40; // ms
const int dah_duration = dit_duration * 3; // ms
const int char_interval = dit_duration * 3; // ms
const int phrase_interval = dah_duration * 7; // ms
const char char_interval_symbol = ' ';
const char phrase_interval_symbol = '/';

String morse_code = ".... . .. .-.. / .... . .. .-.. / .... . .. .-.. / .... .- .. .-.. / .... .- .. .-.. / .... .- .. .-..";
int current_index = 0;
bool first_time = false;

void setup() {
  pinMode(button_pin, INPUT);
  pinMode(buzzer_pin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  readButton();

  delay(10); // Delay para melhorar a performance da simulação
}

void readButton() {
  static unsigned long press_start_time = 0;
  static bool button_was_pressed = false;

  int button_state = digitalRead(button_pin);

  if (button_state == HIGH && !button_was_pressed) {
    // Botão foi pressionado
    press_start_time = millis();
    button_was_pressed = true;
    first_time = true; // Marca que é a primeira vez que o botão foi pressionado
  } else if (button_state == LOW && button_was_pressed) {
    // Botão foi solto
    unsigned long press_duration = millis() - press_start_time;

    if (press_duration <= dit_duration) {
      // Dit
      beep(dit_duration);
    } else if (press_duration <= dah_duration) {
      // Dah
      beep(dah_duration);
    }

    button_was_pressed = false;
  }

  // Verificar se o código Morse terminou
  if (first_time && current_index < morse_code.length()) {
    // Ainda há caracteres para processar
    char current_char = morse_code[current_index];
    
    if (current_char == '.') {
      beep(dit_duration);
    } else if (current_char == '-') {
      beep(dah_duration);
    } else if (current_char == char_interval_symbol) {
      delay(char_interval);
    } else if (current_char == phrase_interval_symbol) {
      delay(phrase_interval); // Intervalo entre palavras
    }

    current_index++;

    // Verifica se chegou ao final da string
    if (current_index == morse_code.length()) {
      // Resetar para o início após o término do código Morse
      current_index = 0;
      first_time = false; // Marca que não é mais a primeira vez
    }
  }
}

void beep(int duration) {
  digitalWrite(buzzer_pin, HIGH);
  delay(duration);
  digitalWrite(buzzer_pin, LOW);
  delay(dit_duration); // Intervalo entre dit e dah
}
