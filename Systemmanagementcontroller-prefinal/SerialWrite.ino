//---Serial write operations---

#define DATASENTMSG   (String) F("Serial ") + (String) SERIALNUMBER + ": tx r:" + (String) reg + " d:" + (String) data
#define ACKTIMEOUTMSG (String) F("Serial ") + (String) SERIALNUMBER + (String) F(": First acknowledge not recieved within timeout, STOPPING SYSTEM")
#define TWOACKMSG     (String) F("Serial ") + (String) SERIALNUMBER + (String) F(": Another data package with acknowledge request, WAITING")
#define SPBUSY        (String) F("Serial ") + (String) SERIALNUMBER + (String) F(": Port is busy, no data sent")
//----------------------------------------------------------------

void serialSingleWrite0(byte reg){//write one package
  #define SERIALPORT Serial
  if(SERIALPORT.availableForWrite()) SERIALPORT.write(reg);
}

void serialSingleWrite1(byte reg){
  #define SERIALPORT Serial1
  if(SERIALPORT.availableForWrite()) SERIALPORT.write(reg);
}

void serialSingleWrite2(byte reg){
  #define SERIALPORT Serial2
  if(SERIALPORT.availableForWrite()) SERIALPORT.write(reg);
}

void serialSingleWrite3(byte reg){
  #define SERIALPORT Serial3
  if(SERIALPORT.availableForWrite()) SERIALPORT.write(reg);
}

//----------------------------------------------------------------

void floatWrite0(byte reg, float data, bool ack){//write float package
  #define SERIALNUMBER 0
  #define SERIALWRITE serialWrite0
  
  byte *b = floatToByte(data);
  SERIALWRITE(reg, b, ack);
  debugLog(DATASENTMSG);
}

void floatWrite1(byte reg, float data, bool ack){
  #define SERIALNUMBER 1
  #define SERIALWRITE serialWrite1
  
  byte *b = floatToByte(data);
  SERIALWRITE(reg, b, ack);
  debugLog(DATASENTMSG);
}

void floatWrite2(byte reg, float data, bool ack){
  #define SERIALNUMBER 2
  #define SERIALWRITE serialWrite2
  
  byte *b = floatToByte(data);
  SERIALWRITE(reg, b, ack);
  debugLog(DATASENTMSG);
}

void floatWrite3(byte reg, float data, bool ack){
  #define SERIALNUMBER 3
  #define SERIALWRITE serialWrite3
  
  byte *b = floatToByte(data);
  SERIALWRITE(reg, b, ack);
  debugLog(DATASENTMSG);
}


//------------------------------------------------

void serialWrite0(byte reg, byte data[4], bool ack){//write 5byte package
  #define SERIALPORT Serial
  #define SERIALNUMBER 0
  #define POLLREAD pollSerialRead0
  #define RUNNER aT0

  //ROUTINE IF A SECOND DATAPACKAGE WITH ACK IS REQUESTED TO SEND, BUT THE FIRST ACK HAS NOT BEEN RECIEVED
  //stop resending acknowledge requests, simply wait for answer, if no answer comes,  system stops
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

//------------------------------------------------

void serialWrite1(byte reg, byte data[4], bool ack){
  #define SERIALPORT Serial1
  #define SERIALNUMBER 1
  #define POLLREAD pollSerialRead1
  #define RUNNER aT1

  //ROUTINE IF A SECOND DATAPACKAGE WITH ACK IS REQUESTED TO SEND, BUT THE FIRST ACK HAS NOT BEEN RECIEVED
  int cnt = 0;
  while(ackState[SERIALNUMBER]){
    if (cnt==TIMEOUT)criticalStop(ACKTIMEOUTMSG);
    if (cnt==0) warningLog(TWOACKMSG);
    pollSerialRead2();//DEBUG
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

//------------------------------------------------

void serialWrite2(byte reg, byte data[4], bool ack){
  #define SERIALPORT Serial2
  #define SERIALNUMBER 2
  #define POLLREAD pollSerialRead2
  #define RUNNER aT2

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

//------------------------------------------------

void serialWrite3(byte reg, byte data[4], bool ack){
  #define SERIALPORT Serial3
  #define SERIALNUMBER 3
  #define POLLREAD pollSerialRead3
  #define RUNNER aT3

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

//------------------------------------------------
