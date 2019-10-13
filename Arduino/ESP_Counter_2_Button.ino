// WORK
const int button1 = D5;
const int button2 = D6;

int state1, state2;
int lstate1, lstate2;
int counter = 0;
//int lastButtonState = 0;
int number, digit;
int digit0, digit1, digit2;

long prev = 0;
int DURATION_IN_MILLIS = 1000;
int buttonState = 0;  // 0 = not pressed   --- 1 = long pressed --- 2 short pressed

void setup() {
  Serial.begin(115200);
  pinMode(D4, INPUT);
  pinMode(D5, INPUT);
  Serial.println("Masukan digit ke-0");
}
void get_button() {
  state1 = digitalRead(button1);
  state2 = digitalRead(button2);
  if (state1 != lstate1) {
    // if the state has changed, increment the counter
    if (state1 == HIGH) {
      counter++;
      if (counter >= 10) {
        counter = 0;
      }
      Serial.println(counter);
    }
    delay(50);
  }
  lstate1 = state1;

  buttonState = 0;

  if (digitalRead(button2)) {
    prev = millis();
    buttonState = 1;
    while ((millis() - prev) <= DURATION_IN_MILLIS) {
      if (!(digitalRead(button2))) {
        buttonState = 2;
        break;
      }
    }
  }
  if (buttonState == 1) {
    // TODO button is pressed long
    Serial.print("Sending data for ID : ");
    Serial.println(number);
  } else if (buttonState == 2) {
    if (state2 != lstate2) {

      if (state2 == HIGH) {
        if (digit == 0) {
          digit0 = counter;
          digit = 1;
          counter = 0;
        }
        else if (digit == 1) {
          digit1 = counter;
          digit = 2;
          counter = 0;
        }
        else if (digit == 2) {
          digit2 = counter;
          counter = 0;
          digit = 0;
        }
        Serial.print("Masukan Digit ke-");
        Serial.println(digit + 1);
        number = (digit0 * 100) + (digit1 * 10) + digit2;

        cetak_angka();
      }
      delay(50);
    }
    //TODO button is pressed short
  }
  lstate2 = state2;
}

void cetak_angka(void) {
  Serial.print("ID :");
  if (number < 100 && number >= 10) {
    Serial.print("0");
    Serial.println(number);
  }
  if (number < 10) {
    Serial.print("00");
    Serial.println(number);
  }
  else {
    Serial.println(number);
  }
}

void loop() {
  get_button();

}
