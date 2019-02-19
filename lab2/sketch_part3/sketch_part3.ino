volatile int digit = 0;
volatile unsigned long last_increment;

char numbers[10][5] = { 
  {0x3E, 0x51, 0x49, 0x45, 0x3E},   // 0
  {0x00, 0x42, 0x7F, 0x40, 0x00},   // 1
  {0x42, 0x61, 0x51, 0x49, 0x46},   // 2
  {0x21, 0x41, 0x45, 0x4B, 0x31},   // 3
  {0x18, 0x14, 0x12, 0x7F, 0x10},   // 4
  {0x27, 0x45, 0x45, 0x45, 0x39},   // 5
  {0x3C, 0x4A, 0x49, 0x49, 0x30},   // 6
  {0x01, 0x71, 0x09, 0x05, 0x03},   // 7
  {0x36, 0x49, 0x49, 0x49, 0x36},   // 8
  {0x06, 0x49, 0x49, 0x29, 0x1E}    // 9
}; 

void setup() {
  // Set latchPin (12) to OUTPUT (1)
  DDRB |= (1 << PB4);

  // Set dataPin (13) to OUTPUT (1)
  DDRB |= (1 << PB5);

  // Set clockPin (11) to OUTPUT (1)
  DDRB |= (1 << PB3);

  // Start timer for cycling through digits
  last_increment = millis();

  // Set PD2 (2) to INPUT_PULLUP for switch inputs
  pinMode(2, INPUT_PULLUP);

  // Attach the ISR to handle changes in the switch state
  // Only trigger on the falling edge
  attachInterrupt(digitalPinToInterrupt(2), updateDigit_ISR, FALLING);
}

void loop() {
  // Scan through each column and set the rows
  for (int colNum = 0; colNum < 5; colNum++) {
    // Flip the bit for the corresponding column
    char col = B00011111 & ~(1 << colNum);

    // Set latchPin (12) to LOW (0)
    PORTB &= ~(1 << PB4);
    
    writeVal(col);
    writeVal(numbers[digit][colNum]);
    
    // Set latchPin (12) to HIGH (1)
    PORTB |= (1 << PB4);
  }
}

void writeVal(char value) {
  // Send the data for a row
  for (int i = 7; i >= 0; i--) {
    // Set clockPin (11) to LOW (0)
    PORTB &= ~(1 << PB3);
    
    // Set dataPin (13) to the bit at i in value
    if (value & (1 << i)) {
      PORTB |= (1 << PB5);
    } else {
      PORTB &= ~(1 << PB5);
    }
    
    // Set clockPin (11) to HIGH (1)
    PORTB |= (1 << PB3);
  }
}

// Handle changes in switch state
void updateDigit_ISR() {
  // Debounce by checking if 200 ms has passed since digit was last incremented
  if (millis() > last_increment + 200) {
    // Update last increment
    last_increment = millis();

    // Increment digit and wrap around from 9 to 0
    digit = (digit + 1) % 10;
  }
}
