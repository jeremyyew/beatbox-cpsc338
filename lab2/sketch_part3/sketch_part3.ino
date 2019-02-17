int latchPin = 12;
int clockPin = 11;
int dataPin = 13;
volatile int digit = 0;
volatile unsigned long time;

char numbers[10][5] = { {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
  {0x00, 0x42, 0x7F, 0x40, 0x00},// 1
  {0x42, 0x61, 0x51, 0x49, 0x46},// 2
  {0x21, 0x41, 0x45, 0x4B, 0x31},// 3
  {0x18, 0x14, 0x12, 0x7F, 0x10},// 4
  {0x27, 0x45, 0x45, 0x45, 0x39},// 5
  {0x3C, 0x4A, 0x49, 0x49, 0x30},// 6
  {0x01, 0x71, 0x09, 0x05, 0x03},// 7
  {0x36, 0x49, 0x49, 0x49, 0x36},// 8
  {0x06, 0x49, 0x49, 0x29, 0x1E} };// 9


void setup() {
  // put your setup code here, to run once:
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  Serial.begin(9600);
  time = millis();

  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), updateDigit, FALLING);
}

void loop() {
  for (int colNum = 0; colNum < 5; colNum++) {
     int row = 0;
     char col = B00011111 & ~(1 << colNum);
     
     digitalWrite(latchPin, LOW);
     writeVal(col);
     writeVal(numbers[digit][colNum]);
     digitalWrite(latchPin, HIGH);
  }
}

void writeVal(char value) {
  for (int i = 0; i < 8; i++)  {  
    digitalWrite(clockPin, LOW);       
    digitalWrite(dataPin, !!(value & (1 << (7 - i))));    
    digitalWrite(clockPin, HIGH);
  }
}

void updateDigit() {
  unsigned long last_trigger = time;
  if (millis() > last_trigger + 200) {
    time = millis();
    digit++;
    if (digit > 9) {
      digit = 0;
    }
    Serial.print(digit);
  } else {
    Serial.print("ignored");
  }
}
