#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "HX711.h"

const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;
HX711 scale;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

float berat = 1;
double berat_real = 0;
double berat_read = 0;
String berat_print;

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  display.clearDisplay();

  display.display();
  delay(500);

  display.invertDisplay(true);
  delay(50);
  display.invertDisplay(false);
  delay(50);
}

void loop(){
  if (scale.is_ready()) {
    double reading = scale.read();
    double berat_read = (reading+199160)/200000 ;//3 dari kiri = 1.kg
    String berat_real = String(berat_read,2);
    Serial.print("Berat : ");
    Serial.println(String(berat_real) + "Kg");
    display.clearDisplay();
    display.setTextSize(2);            
    display.setTextColor(WHITE);        // Draw white text
    display.setCursor(30,0);             // Start at top-left corner
    display.println("Berat");
    display.println();
    display.setTextSize(3);
    display.println(String(berat_real) + "Kg");
    display.display();
    delay(50);

  } else {
    Serial.println("HX711 not found.");
    error();
    }
  }

  void error(void) {
  display.clearDisplay();
  display.setTextSize(3);            
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(25,25);             // Start at top-left corner
  display.println("ERROR");
  display.display();
  delay(5000);
  }
