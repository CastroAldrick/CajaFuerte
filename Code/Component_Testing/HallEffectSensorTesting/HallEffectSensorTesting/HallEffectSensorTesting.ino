int const hallEffect = A0;
void setup() {
  // put your setup code here, to run once:
  pinMode(hallEffect, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(analogRead(hallEffect));
  delay(1000);
}
