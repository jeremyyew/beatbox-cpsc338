/* This code is based on an example that shows basic usage of the NeoTrellis.
  The buttons will light up various colors when pressed.
  The interrupt pin is not used in this example.
*/

#include "Adafruit_NeoTrellis.h"

byte noteON = 144; //note on command
Adafruit_NeoTrellis trellis;
uint32_t off = trellis.pixels.Color(0, 0, 0);
uint32_t A = trellis.pixels.Color(100, 0, 0);       // wrong (red)
uint32_t B = trellis.pixels.Color(0, 100, 100);     // active (bright blue)
uint32_t C = trellis.pixels.Color(0, 0, 50);        // hint (dark blue)
uint32_t D = trellis.pixels.Color(50, 0, 100);      // correct (purple)
unsigned long buttonLastHit[4] = {0, 0, 0, 0};

//define a callback for key presses
TrellisCallback blink(keyEvent evt){
  if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
    int column = evt.bit.NUM % 4;
    if (millis() - buttonLastHit[column] < 1000) {
      return 0;
    }
    buttonLastHit[column] = millis();
    Serial.write(evt.bit.NUM + 48);
  } else if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING) {
    Serial.write(evt.bit.NUM + 16 + 48);
  }

  return 0;
}

void setup() {
  Serial.begin(115200);
  
  if (!trellis.begin()) {
    // Error
    while(1);
  }

  //activate all keys and set callbacks
  for(int i=0; i<NEO_TRELLIS_NUM_KEYS; i++){
    trellis.activateKey(i, SEESAW_KEYPAD_EDGE_RISING);
    trellis.activateKey(i, SEESAW_KEYPAD_EDGE_FALLING);
    trellis.registerCallback(i, blink);
  }

  //do a little animation to show we're on
  for (uint16_t i=0; i<trellis.pixels.numPixels(); i++) {
    trellis.pixels.setPixelColor(i, Wheel(map(i, 0, trellis.pixels.numPixels(), 0, 255)));
    trellis.pixels.show();
    delay(50);
  }
  
  for (uint16_t i=0; i<trellis.pixels.numPixels(); i++) {
    trellis.pixels.setPixelColor(i, 0x000000);
    trellis.pixels.show();
    delay(50);
  }
}

void loop() {
  if (Serial.available() > 0) {
    int readIn = Serial.read() - '0';
    if (readIn < 16) {
      trellis.pixels.setPixelColor(readIn, off);
    } else if (readIn < 32) {
      trellis.pixels.setPixelColor(readIn - 16, A);
    } else if (readIn < 48) {
      trellis.pixels.setPixelColor(readIn - 32, B);
    } else if (readIn < 64) {
      trellis.pixels.setPixelColor(readIn - 48, C);
    } else if (readIn < 80) {
      trellis.pixels.setPixelColor(readIn - 64, D);
    }
    trellis.pixels.show();
  }

  trellis.read();

  delay(20); //the trellis has a resolution of around 60hz
}


/******************************************/

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return trellis.pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return trellis.pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return trellis.pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  return 0;
}
