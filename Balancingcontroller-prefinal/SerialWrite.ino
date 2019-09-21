//---Serial write operations---

#define DATASENTMSG   (String) F("Serial ") + (String) SERIALNUMBER + ": tx r:" + (String) reg + " d:" + (String) data
#define ACKTIMEOUTMSG (String) F("Serial ") + (String) SERIALNUMBER + (String) F(": First acknowledge not recieved within timeout, STOPPING SYSTEM")
#define TWOACKMSG     (String) F("Serial ") + (String) SERIALNUMBER + (String) F(": Another data package with acknowledge request, WAITING")
#define SPBUSY        (String) F("Serial ") + (String) SERIALNUMBER + (String) F(": Port is busy, no data sent")

void serialSingleWrite0(byte reg){
  #define SERIALPORT Serial
  if(SERIALPORT.availableForWrite()) SERIALPORT.write(reg);
}
//----------------------------------------------------------------

void floatWrite0(byte reg, float data, bool ack){
  #define SERIALNUMBER 0
  #define SERIALWRITE serialWrite0
  
  byte *b = floatToByte(data);
  SERIALWRITE(reg, b, ack);
  #if ENABLEDEBUGLOG 
  debugLog(DATASENTMSG);
  #endif
}
//------------------------------------------------

void serialWrite0(byte reg, byte data[4], bool ack){
  #define SERIALPORT Serial
  #define SERIALNUMBER 0
  #define POLLREAD pollSerialRead0
  #define RUNNER aT0

  //ROUTINE IF A SECOND DATAPACKAGE WITH ACK IS REQUESTED TO SEND, BUT THE FIRST ACK HAS NOT BEEN RECIEVED
  int cnt = 0;
  while(ackState[SERIALNUMBER]){
    if (cnt==TIMEOUT)criticalStop(ACKTIMEOUTMSG);
    if (cnt==0) warningLog(TWOACKMSG);
    POLLREAD();
    delay(1);
    cnt++;
  }
  
  if(SERIALPORT.availableForWrite()){

    //WRITE DATA
    
    ///*
    SERIALPORT.write(reg);
    SERIALPORT.write(data[0]);
    SERIALPORT.write(data[1]);
    SERIALPORT.write(data[2]);
    SERIALPORT.write(data[3]);
    //*/

    //ACKNOWLEDGE
    
    if (ack){
      ackState[SERIALNUMBER] = 1;
      ackReg[SERIALNUMBER] = reg;
      ackData[SERIALNUMBER][0] = data[0];
      ackData[SERIALNUMBER][1] = data[1];
      ackData[SERIALNUMBER][2] = data[2];
      ackData[SERIALNUMBER][3] = data[3];
      RUNNER.enable();
    }

    //DEBUG
    
    /*
    Serial.println("-----");
    Serial.println(reg);
    Serial.println(data[0], BIN);
    Serial.println(data[1], BIN);
    Serial.println(data[2], BIN);
    Serial.println(data[3], BIN);
    */
  }
  else{
    warningLog(SPBUSY);
  }
}
