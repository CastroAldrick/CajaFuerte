/*
 * Programmer: Aldrick
 * Description: Testing of the Fingerprint Scanner
 * Wiring:
 * 
 * FingerPrint Scanner -> Arduino Micro
 *         VCC (Green) -> 3.3V
 *            TX (Red) -> RX
 *         RX (Yellow) -> TX
 *          GND (Blue) -> GND
 */

 // Library
 #include <Adafruit_Fingerprint.h>

 // Variables
 #define mySerial Serial1
 int fingerprintID = 0;
 Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
 
void setup() {
  // Initialization
  startFingerprintSensor();
  Serial.begin(9600);
  Serial.println("Done with Setup");
  finger.begin(57600);
}

void loop() {

  fingerprintID = getFingerprintID();

  if (fingerprintID == 1){
    Serial.println("Hey Aldrick");
  }

  //Serial.println("Enter Finger");
  delay(20);
}

void startFingerprintSensor()
{
  Serial.begin(9600);
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor");
  }
  Serial.println("Waiting for valid finger...");
}

int getFingerprintID() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}

