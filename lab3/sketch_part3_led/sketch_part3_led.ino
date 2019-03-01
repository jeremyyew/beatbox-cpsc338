#include <Adafruit_SSD1306.h>

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

void setup() {
  disp.begin(SSD1306_SWITCHCAPVCC);
  disp.clearDisplay();
  disp.display();
  disp.drawLine(0, 0, 63, 63, WHITE);
}

void loop() {
  disp.display();
}
