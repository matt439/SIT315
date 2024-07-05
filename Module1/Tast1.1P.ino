void setup()
{
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(13, OUTPUT);
}

void loop()
{
  if (digitalRead(2) == HIGH)
  {
    Serial.println("Active");
    digitalWrite(13, HIGH);
  }
  else
  {
    Serial.println("Inactive");
    digitalWrite(13, LOW);
  }
  delay(1000);
}
