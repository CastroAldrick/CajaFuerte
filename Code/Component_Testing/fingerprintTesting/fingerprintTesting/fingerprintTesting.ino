/*
 * Programmer: Aldrick
 * Description: Testing of the Fingerprint Scanner
 * Wiring:
 * 
 * FingerPrint Scanner -> Arduino Mega
 *           VCC (Red) -> 3.3V/5V
 *          TX (Green) -> 19 RX1
 *          RX (White) -> 18 TX1
 *         GND (Black) -> GND
 */

 // Library
 #include <Adafruit_Fingerprint.h>

 // Variables
 #define mySerial Serial1
 int fingerprintID = 0;
 Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

 int option = 1;    // 0 = Scanning
                    // 1 = Enrolling
                    // 2 = Deletingfinger.emptyDatabase();
                    // 3 = Clearing All
 bool Done = false;
 uint8_t id;
 uint8_t L;
 
void setup() {
  // Initialization
  startFingerprintSensor();
  while(!Serial);
  //finger.emptyDatabase();
  Serial.println("Done with Setup");
  finger.begin(57600);

  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  
}

void loop() {

//  if (counter == 5)
//    Serial.println("Test Complete");

  if (!Done)
    action(option);

  
//  
//  fingerprintID = getFingerprintID();
//
//  if (fingerprintID != -1){
//    Serial.println("Hey Aldrick");
//  }
//
//  //Serial.println("Enter Finger");
  delay(20);
}

int action (int option){
  switch (option)
  {
    case 0:
      Serial.println("Scanning");
      
      delay (1000);
      fingerprintID = getFingerprintID();
      //Serial.println(fingerprintID);
      if (fingerprintID != -1){
        Serial.println("Good");
        Done = true;
      }
      break;
      
    case 1:
      Serial.println("Enrolling");
      Serial.println("Ready to enroll a fingerprint!");
      Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
      id = readnumber();
      if (id == 0) {// ID #0 not allowed, try again!
         return;
      }
      Serial.print("Enrolling ID #");
      Serial.println(id);
      
      L = getFingerprintEnroll();
      while (!L);

      delay (1000);
      break;
      
    case 2:
      Serial.println("Delete");
      Serial.println("Enter id");

      id = readnumber();
      if (id == 0) {// ID #0 not allowed, try again!
         return;
      }
      
      Serial.print("Deleting ID #");
      Serial.println(id);
      
      deleteFingerprint(id);
      
      delay (1000);
      break;

    case 3:
      Serial.println("Empty Database");

      Serial.println("\n\nDeleting all fingerprint templates!");
      Serial.println("Press 'Y' key to continue");
    
      while (1) {
        if (Serial.available() && (Serial.read() == 'Y')) {
          finger.emptyDatabase();
          Serial.println("Now database is empty :)");
          Done = true;
          break;
        }
        if (Serial.available() && (Serial.read() == 'N')){
          Serial.println("Cancelled");
          Done = true;
          break;
        }
      }
      delay(1000);
      break;
    
  }
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

uint8_t readnumber(void) {
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    Done = true;
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
}

uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;
  
  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
    Done = true;
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
    return p;
  }   
}


