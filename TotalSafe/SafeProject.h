#ifndef Safe
#define Safe

#if (ARDUINO >= 100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

 #include <SPI.h>
 #include <MFRC522.h>
 #include "SafeProject.h"
 #include <Adafruit_Fingerprint.h>

class SafeProject {
  public:    
    // Constructor
    SafeProject (bool hi=false);

    // Variable
    String enteredCard = "";

    // Defines
    #define SS_PINfp   10
    #define RST_PIN   9
    #define mySerial Serial1
    Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
    
    
    // Methods
    byte getID();
    
    void startFingerprintSensor();
    int getFingerprintID();
};







#endif
