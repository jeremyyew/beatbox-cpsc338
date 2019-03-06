// Code referenced from http://csl.yale.edu/~rajit/classes/eeng348/files/photocells.pdf

int analogPin = A0;
// RGB pins mapped to digital pins 11, 10, 9 respectively
int LED = 9;
int reading;
int brightness;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    reading = analogRead(analogPin);
    Serial.print("reading = ");
    Serial.println(reading);
    brightness = map(reading, 0, 1023, 255, 0);
    analogWrite(LED, brightness);
    delay(500);
}

/* Implementing analogRead()
- We need to set the highest 2 bits (REFS 1 and REFS 0) of the ADMUX (ADC Multiplexer Selection Register) register to the bits that correspond to the reference voltage, which is set via analogReference().

- We also need to select the analog input channel by writing to the least significant 4 bits of ADMUX, ie MUX0-3 (refer to the Analog Channel Selection Bits table to map I/O pins to channel number).

- The ADLAR (ADC Left Adjust Result) bit of ADMUX controls if the results are left adjusted, and defaults to 0, representing right alignment.

- Then, we would need to set the ADSC (AD start conversion) bit of the 
ADCSRA (ADC Control and Status Register A) to 1 to start the conversion.
Then we could wait in a loop until ADSC is cleared by hardware, indicating the conversion completed.

- The results from the ADC are stored in the ADC Data Registers, ADCL and ADCH.
We would need to read ADCL and combine it with ADCH (shifted by 😎 for the complete 10-bit result. 
- We would need to read ADCL first since both registers are locked after an ADCL read in order to prevent ADCH from changing to the values resulting from another conversion.

Finally, we can return the 10-bit result as the result of analogRead().

Reference: 
https://garretlab.web.fc2.com/en/arduino/inside/arduino/wiring_analog.c/analogRead.html
http://csl.yale.edu/~rajit/classes/eeng348/files/ATMega328.pdf
*/ 