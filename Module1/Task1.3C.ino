const int LED_INTAKE_PIN = 6;
const int LED_DRAIN_PIN = 7;
const int FLOW_INTAKE_PIN = 2;
const int FLOW_DRAIN_PIN = 3;
volatile bool intakeState = false;
volatile int intakeCount = 0;
volatile bool drainState = false;
volatile int drainCount = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(FLOW_INTAKE_PIN, INPUT_PULLUP);
  pinMode(FLOW_DRAIN_PIN, INPUT_PULLUP);
  pinMode(LED_INTAKE_PIN, OUTPUT);
  pinMode(LED_DRAIN_PIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(FLOW_INTAKE_PIN), intakeFlow, CHANGE);
  attachInterrupt(digitalPinToInterrupt(FLOW_DRAIN_PIN), drainFlow, CHANGE);
}

void loop()
{
}

void intakeFlow()
{
  intakeState = !intakeState;
  intakeCount++;
  digitalWrite(LED_INTAKE_PIN, intakeState);
  Serial.print("Intake pulse! Count: ");
  Serial.println(intakeCount);
}

void drainFlow()
{
  drainState = !drainState;
  drainCount++;
  digitalWrite(LED_DRAIN_PIN, drainState);
  Serial.print("Drain pulse! Count: ");
  Serial.println(drainCount);
}
