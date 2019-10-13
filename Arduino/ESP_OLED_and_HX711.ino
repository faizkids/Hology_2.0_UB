//Pinout :
//OLED
//  SCL - D1
//  SDA - D2
//
//HX71I
//  SCK - D3
//  DT - D0


#include "HX711.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = D0;
const int LOADCELL_SCK_PIN = D3;

HX711 scale;
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

long reading,reading2;

void setup() {
  Serial.begin(115200);

//OLED Section  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(1000);
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(3, 5);
  // Display static text
  display.println("Sukses !");
  display.display(); 

// HX711 Section
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
}


void get_berat(void){
  if (scale.is_ready()) {
    long reading = scale.read();
    Serial.print("HX711 reading: ");
    Serial.println(reading);
  } 
  else {
    Serial.println("HX711 not found.");
  }
  delay(100);
}

void set_oled(void){
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,5);
  // Display static text
  display.println(reading2);
  display.display(); 
  reading2++; // HARUS DALAM 1 CLASS
  }

void loop() {
  get_berat(); //Tapi print serial juga
  set_oled();
  }
