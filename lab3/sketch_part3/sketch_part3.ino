// Adafruit GFX Library - Version: Latest 
#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>

// Adafruit SSD1306 - Version: 1.2.9
#include <Adafruit_SSD1306.h>
#include <splash.h>

// rotary encoder code from http://bildr.org/2012/08/rotary-encoder-arduino/
// adafruit graphics primitives code from https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives

// TODO:
// 1. circuit diagram for part 1-3
// 2. part 2 video 
// 3. explain analogRead implementation part 1
// 4. comment code for part 1-3
// 5. part 3 debouncing?
// 6. sources for part 1-2

#define OLED_CS 10
#define OLED_DC 9

// Reset pin
#define OLED_RST 8

// D1 pin
#define OLED_SI 11

// D0 pin
#define OLED_CLK 13

#define DISP_WIDTH 128
#define DISP_HEIGHT 64
#define BALL_RADIUS 4

Adafruit_SSD1306 disp(DISP_WIDTH, DISP_HEIGHT,OLED_SI,OLED_CLK,OLED_DC,OLED_RST,OLED_CS);

// Arduino digital pin numbers
int encoderPin1 = 2;
int encoderPin2 = 3;

volatile int lastEncoded = 0;
volatile long encoderValue = 128; // Starting value - corresponding to velocity = 0

long lastencoderValue = 0;
int lastMSB = 0;
int lastLSB = 0;

// Positioning and movement of ball
int x;
int y;
int direction;
char msg[3];

void setup() {
  Serial.begin (9600);
  x = 0;
  y = 31;
  direction = 1;

  pinMode(encoderPin1, INPUT);
  pinMode(encoderPin2, INPUT);
  
  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH); //turn pullup resistor on
  
  //call updateEncoder() when any high/low changed seen
  //on interrupt 0 (pin 2), or interrupt 1 (pin 3)
  attachInterrupt(0, updateEncoder, CHANGE);
  attachInterrupt(1, updateEncoder, CHANGE);

  disp.begin(SSD1306_SWITCHCAPVCC);
  disp.clearDisplay();
}

void loop() {
  disp.clearDisplay();
  // update position first before checking if it has hit the edge 
  x = x + (getVelocity() * direction);
  
  if (at_left_edge(x, y)) {
    x = BALL_RADIUS;
    direction = direction * -1;
  }
  
  if (at_right_edge(x, y)) {
    x = DISP_WIDTH - BALL_RADIUS - 4; // a little bit of offset since it cuts off otherwise
    direction = direction * -1;
  }

  disp.drawCircle(x, y, BALL_RADIUS, WHITE);
  // set up display of encoder value
  disp.setCursor(55, 5);
  disp.setTextColor(WHITE);
  disp.print(encoderValue);
  // display everything 
  disp.display();
  delay(100);
}

int getVelocity() {
    // encoderValue is [0, 255] inclusive
    // velocity is [-26, 25] inclusive
    // (encoderValue = 128) => (velocity = 0)
    // velocity is in 10px/sec since delay is 0.1 sec
  return (encoderValue - 128) / 5;
}

int at_left_edge(int x, int y) {
  if (x - BALL_RADIUS <= 0) {
    return 1;
  } else {
    return 0;
  }
}

int at_right_edge(int x, int y) {
  if (x + BALL_RADIUS >= DISP_WIDTH) {
    return 1;
  } else {
    return 0;
  }
}

void updateEncoder(){
  int MSB = digitalRead(encoderPin1); //MSB = most significant bit
  int LSB = digitalRead(encoderPin2); //LSB = least significant bit
  
  int encoded = (MSB << 1) | LSB; //converting the 2 pin values to 2 bit number
  int sum = (lastEncoded << 2) | encoded; // adding it to the previous encoded value 

  // Checking valid transition states
  // TODO: is this clockwise or counter-clockwise?
  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
    encoderValue++;
    encoderValue = constrain(encoderValue, 0, 255); // Saturating end points
  }

  // Checking valid transition states
  // TODO: is this clockwise or counter-clockwise?
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
    encoderValue--; 
    encoderValue = constrain(encoderValue, 0, 255); // Saturating end points
  }
  lastEncoded = encoded; //store this value for next time 
}