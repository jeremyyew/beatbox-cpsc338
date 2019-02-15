#include <avr/io.h>

void setup()
{
    PORTD |= (1 << PD2);
    DDRD &= ~(1 << PD2);
    // pinMode(4, INPUT_PULLUP);
    Serial.begin(9600);
}

void loop()
{
    // int buttonValue = digitalRead(buttonPin);
    if ((PIND & (1 << PD2)) == 4)
    {
        Serial.print("ON");
    }
    else
    {
        Serial.print("OFF");
    }
}