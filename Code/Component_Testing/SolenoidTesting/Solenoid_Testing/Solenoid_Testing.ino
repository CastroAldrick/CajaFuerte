/*
 * Solenoid Testing
 */

 int Sol = 39;

void setup() {
 Serial.begin(9600);
 pinMode(Sol, OUTPUT);
}

void loop() {
  digitalWrite(Sol, HIGH);
  delay(2000);
  digitalWrite(Sol, LOW);
  delay(2000);
}
