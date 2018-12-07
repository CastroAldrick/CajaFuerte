/*
 * Solenoid Testing
 */

 int Sol = 26;

void setup() {
 Serial.begin(9600);
 pinMode(Sol, OUTPUT);
}

void loop() {
  digitalWrite(Sol, HIGH);
  delay(5000);
  digitalWrite(Sol, LOW);
  delay(5000);
}
