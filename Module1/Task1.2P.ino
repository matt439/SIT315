const int LED_PIN = 13;
const int MOTION_PIN = 2;
volatile bool movement = false;

void setup()
{
  Serial.begin(9600);
  pinMode(MOTION_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(MOTION_PIN), handleMotionChange, CHANGE);
}

void loop()
{
}

void handleMotionChange()
{
  movement = digitalRead(2);
  if (movement)
  {
    Serial.println("Active");
    digitalWrite(LED_PIN, HIGH);
  }
  else
  {
    Serial.println("Inactive");
    digitalWrite(LED_PIN, LOW);
  }
}
