#define TIMER_LOAD 0
// target of 2Hz. (16,000,000 / (1024 * 2)) - 1 = 7811.5
#define TIMER_COMPARISON 7812

// Set the pins as constant bytes given by the datasheet.
const byte FLOW_INTAKE_PIN = 0b00000100; // Pin 2
const byte FLOW_DRAIN_PIN = 0b00001000; // Pin 3
const byte MOTION_1_PIN = 0b00010000; // Pin 4
const byte MOTION_2_PIN = 0b00100000; // Pin 5
const byte LED_TIMER_PIN = 0b00000001; // Pin 8
const byte LED_INTAKE_PIN = 0b00000010; // Pin 9
const byte LED_DRAIN_PIN = 0b00000100; // Pin 10
const byte LED_MOTION_1_PIN = 0b00001000; // Pin 11
const byte LED_MOTION_2_PIN = 0b00010000; // Pin 12

// Use volatile variables as they used during an
// interrupt service routine.
volatile uint32_t intakeCount = 0, drainCount = 0;

void setup()
{
  Serial.begin(9600);

  // Set pins 2-5 as input
  DDRD |= 0b00000000;

  // Set pins 2 and 3 to HIGH for pullup resistor
  PORTD |= 0b00001100;

  // Set pins 8-12 as output
  DDRB |= 0b00011111;

  // Set pins 8-12 to LOW
  PORTB |= 0b00000000;
  
  // setup pin change interrupts
  // use port D (pins 0-7)
  PCICR |= 0b00000100;
  // ...of port D, use pins 2 to 5
  PCMSK2 |= 0b00111100;

  // setup timer 1 interrupt. Disable other interrupts
  // while it is being set up.
  noInterrupts();
  TCCR1A = 0;

  // CS12 = 1, CS11 = 0, CS10 = 1 to set prescaler to 1024
  TCCR1B = 0b00000101;

  // Set the Timer/Counter1 counter register to 0
  TCNT1 = TIMER_LOAD;

  // Set the Timer/Counter1 output compare register to 7812
  OCR1A = TIMER_COMPARISON;

  // Set the Timer/Counter1 interrupt mask register to enagle timer 1
  TIMSK1 |= (1 << OCIE1A);
  interrupts();
}

void loop()
{
}

// Interrupt Service Routine for compare mode
ISR(TIMER1_COMPA_vect)
{
  // Preload timer with compare match value
  TCNT1 = TIMER_LOAD;

  // Negate the least significant of PORTB
  // (corresponds to timer LED pin).
  // Has the effect of toggling the LED.
  PORTB ^= LED_TIMER_PIN;

  // If LSB of PORTB is true.
  if (PORTB & LED_TIMER_PIN)
  {
    Serial.println("Timer LED on.");
  }
  else
  {
    Serial.println("Timer LED off.");
  }
}

// Interrupt Service Routine for pin change intterupt request 2.
ISR(PCINT2_vect)
{
  // If the flow intake pin is reading the opposite of the led intake
  // pin. Both values are bit shifted so they will equal 1 or 0,
  // then XOR operator is used.
  if ((PIND & FLOW_INTAKE_PIN) >> 2 ^ (PINB & LED_INTAKE_PIN) >> 1)
  {
    intakeCount++;
    Serial.print("Intake pulse! Count: ");
    Serial.println(intakeCount);
    // Toggles the intake LED's pin
    PORTB ^= LED_INTAKE_PIN;
  }
  
  // If the flow drain pin is reading the opposite of the led drain
  // pin. Both values are bit shifted so they will equal 1 or 0,
  // then XOR operator is used.
  if ((PIND & FLOW_DRAIN_PIN) >> 3 ^ (PINB & LED_DRAIN_PIN) >> 2)
  {
    drainCount++;
    Serial.print("Drain pulse! Count: ");
    Serial.println(drainCount);
    // Toggles the drain LED's pin
    PORTB ^= LED_DRAIN_PIN;
  }
  
  // If the motion1 pin is reading the opposite of the led motion1
  // pin. Both values are bit shifted so they will equal 1 or 0,
  // then XOR operator is used.
  if (((PIND & MOTION_1_PIN) >> 4) ^ ((PINB & LED_MOTION_1_PIN) >> 3))
  {
    // If the motion1 pin is reading high (true).
    if ((PIND & MOTION_1_PIN) >> 4)
    {
      Serial.println("Motion detected by sensor 1!");
    }
    else
    {
      Serial.println("Motion no longer detected by sensor 1.");
    }
    // Toggles the motion1 LED's pin
    PORTB ^= LED_MOTION_1_PIN;
  }
  
  // If the motion2 pin is reading the opposite of the led motion2
  // pin. Both values are bit shifted so they will equal 1 or 0,
  // then XOR operator is used.
  if (((PIND & MOTION_2_PIN) >> 5) ^ ((PINB & LED_MOTION_2_PIN) >> 4))
  {
    // If the motion2 pin is reading high (true).
    if ((PIND & MOTION_2_PIN) >> 5)
    {
      Serial.println("Motion detected by sensor 2!");
    }
    else
    {
      Serial.println("Motion no longer detected by sensor 2.");
    }
    // Toggles the motion2 LED's pin
    PORTB ^= LED_MOTION_2_PIN;
  }
}
