// Adafruit GFX Library - Version: Latest 
#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>

// Adafruit SSD1306 - Version: 1.2.9
#include <Adafruit_SSD1306.h>
#include <splash.h>

// TODO:
// 1. circuit diagram for part 1-3
// 2. comment code for part 1-3
// 3. explain analogRead implementation part 1
// 4. video part 2
// 5. part 3 merge + velocity
// 6. part 3 debouncing?
// 6. part 3 video 
/* 
   #define statements for OLED_CS, OLED_DC, OLED_RST, OLED_SI, OLED_CLK 
   go here.
*/
#define OLED_CS 10
#define OLED_DC 9

// Reset pin
#define OLED_RST 8

// D1 pin
#define OLED_SI 11

// D0 pin
#define OLED_CLK 13


Adafruit_SSD1306 disp(128,64,OLED_SI,OLED_CLK,OLED_DC,OLED_RST,OLED_CS);
int counter = 0; 

void setup() {
  disp.begin(SSD1306_SWITCHCAPVCC);
  disp.clearDisplay();
  disp.display();
}

void loop() {
  counter = (counter + 10) % 128;
  disp.clearDisplay();
  disp.fillCircle(counter, 32, 4, WHITE);
  disp.display();
  delay(200);
}
