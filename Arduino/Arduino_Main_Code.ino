//Pinout :
// Using ESP8266 - NodeMcu 12-E
//OLED
//  SCL - D1
//  SDA - D2
//
//HX71I
//  SCK - D3
//  DT - D0
// Button 1 = D5
// Button 2 = D6

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include "HX711.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

const char* ssid = "Fara 2";
const char* password = "12345678.";
String url_base = "http://192.168.43.70:8080/";

// WORK
const int button1 = D5;
const int button2 = D6;

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = D0;
const int LOADCELL_SCK_PIN = D3;

HX711 scale;
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int state1, state2;
int lstate1, lstate2;
int number;

double berat_read;
String berat_real, berat_save;
String show, show2;


String url1 = "id=";
String url2 = "&jenis=";
String url3 = "&berat=";
String data1, data2, data3;
String url;


long prev = 0;
String saved_weight;
int DURATION_IN_MILLIS = 1000;
int buttonState = 0;  // 0 = not pressed   --- 1 = long pressed --- 2 short pressed
int jenisX;
int inputState = 1;
int weightState = 0;
int buffer1;

void setup () {
  Serial.begin(115200);
  pinMode(D6, INPUT); //---------------KUDUNE D6
  pinMode(D5, INPUT);

  //OLED Section
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  delay(1000);


  // HX711 Section
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print("Connecting..");
    display.clearDisplay();
    set_oled("Mencari", "Wifi");
    display.display();
  }

  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(3, 5);
  // Display static text
  display.println(" Sukses !");
  display.display();
  delay(1000);

  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(3, 5);
  // Display static text
  display.println("Klik");
  display.println("Bagian");
  display.println("Kiri");
  display.display();
  delay(5000);
  display.clearDisplay();
}
void get_button() {
  state1 = digitalRead(button1);
  state2 = digitalRead(button2);

  if (inputState == 1) { //Inputstate 1 = ID Input

    if (state2 != lstate2) {
      // if the state has changed, increment the counter
      if (state2 == HIGH) {
        number = number + 10;
        if (number > 99) {
          number = 0;
        }
        Serial.println(number);
      }
      delay(50);
    }

    if (state1 != lstate1) {
      // if the state has changed, increment the counter
      if (state1 == HIGH) {
        number++;
        if (number > 99) {
          number = 0;
        }
        Serial.println(number);
      }
      delay(50);
    }
    lstate1 = state1;
    lstate2 = state2;
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
      Serial.print("ID :");
      Serial.println(number);
      Serial.println("Masukan Jenis Sampah:");
      jenisX = 0;
      inputState = 2;
    }

  }
  if (inputState == 2) { 
    if (state1 != lstate1) {
      // if the state has changed, increment the counter
      if (state1 == HIGH) {
        jenisX++;
        if (jenisX >= 10) {
          jenisX = 0;
        }
        Serial.println(jenisX);
        set_oled("Jenis", String(jenisX));
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
      number = number - 10;
      Serial.print("ID =");
      Serial.println(number);
      Serial.print("Jenis =");
      Serial.println(jenisX);
      kirim_data();
      delay(100);
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);        // Draw white text
      display.setCursor(0, 0);            // Start at top-left corner
      display.println("ID   :" + String(number));
      display.println("Jenis:" + String(jenisX));
      display.println("Berat:" + saved_weight);
      display.display();
      delay(4000);

      Serial.print("Program Start!"); // --------------- Back to Beginning
      inputState = 1;
      weightState = 0;
    }

    else if (buttonState == 2) {
      if (state2 != lstate2) {

        if (state2 == HIGH) {
          set_oled("Ulangi", "");
          delay(2000); // Set supaya ulangi ID dan jenis
          jenisX = 0;
          inputState = 1;
          number = 0;
        }
        delay(50);
      }
      //TODO button is pressed short
    }
    lstate2 = state2;

  }
}

void kirim_data() {
  data1 = number;
  data2 = jenisX;
  data3 = saved_weight;
  if ((WiFi.status() == WL_CONNECTED)) { //Check WiFi connection status
    HTTPClient http;  //Declare an object of class HTTPClient

    url = url_base + url1 + data1 + url2 + data2 + url3 + data3;
    http.begin(url);  //Specify request destination
    Serial.println("Link = " + url);
    int httpCode = http.GET();  //Send the request

    if (httpCode > 0) // httpCode = negative if error
    {
      Serial.println("Sukses-");
      Serial.print(httpCode);
      set_oled(" Data", "Sent");
      delay(1000);    //Send a request every 30 seconds
    }
    http.end();   //Close connection
  }
}

void get_berat(void) { //Calibrated
  if (scale.is_ready()) {
    long reading = scale.read();
    long step1 = (-185000 - reading + 4000);
    Serial.println(step1);
    long step2 = step1 / (-2000);
    //    if (real_weight <= 0) {
    //      real_weight = 0;
    //    }
    Serial.print("Step2=");
    Serial.println(step2);
    float stepper = float(step2); //asline kali -1
    float stepper2 = stepper / 100;
    Serial.println("Stepper2:" + String(stepper2, 1));
    Serial.print("real..");
    String real_weight = String(stepper2, 1);
    Serial.print("Stepper=");
    Serial.println(stepper);
    Serial.print("real weight=");
    Serial.println(real_weight);
    Serial.print("HX711 reading: ");
    Serial.println(reading);
    Serial.print("Cek:");
    Serial.println(float(step2));
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);        // Draw white text
    display.setCursor(30, 0);            // Start at top-left corner
    display.println("Berat");
    display.println();
    display.setTextSize(3);
    display.print(real_weight);
    display.println(" Kg");
    display.display();
    saved_weight = real_weight;
    Serial.println(saved_weight);
    delay(300);

  } else {
    Serial.println("HX711 not found.");
  }
}

void set_oled(String show, String show2) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(30, 0);            // Start at top-left corner
  display.println(show);
  display.println();
  display.print("   ");
  display.println(show2);
  display.display();
  delay(20);
}


void loop() {
  get_button();

  if (digitalRead(button1) == HIGH) { // Rak kanggo, USELESS
    weightState = 1;
    berat_save = berat_real;
  }

  if (weightState == 0) {
    get_berat(); 
    number = -1;
  }
  else if (weightState == 1) {

    if (inputState == 1) {
      set_oled("  ID", ("  " + String(number)));
      delay(10);
    }
    else if (inputState == 2) {
      set_oled("Jenis", String(jenisX));
      delay(10);
    }
  }
}
