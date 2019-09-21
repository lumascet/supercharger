//---Serial read operations---

#define BUFFEROVFMSG    (String) F("Serial ") + (String) SERIALNUMBER + (String) F(": bufferoverflow, RESETTING BUS")
#define ACKRECMSG       (String) F("Serial ") + (String) SERIALNUMBER + (String) F(": acknowledgement recieved! Reg:") + ackReg[SERIALNUMBER]
#define SELFTESTMSG     (String) F("Serial ") + (String) SERIALNUMBER + (String) F(": Selftest passed!")
#define RECDATAMSG      (String) F("Serial ") + (String) SERIALNUMBER + ": rx r: " + (String) reg + " d: " + (String) f
#define REGNOTALLOCMSG  (String) F("Serial ") + (String) SERIALNUMBER + (String) F(": register not allocated:") + (String) reg
#define WRPACKETSIZMSG  (String) F("Serial ") + (String) SERIALNUMBER + (String) F(": wrong Packetsize, RESETTING BUS")
#define TENFAILMSG      (String) F("Serial ") + (String) SERIALNUMBER + (String) F(": 10 failed attempts of reading data in series!")

void serialFlush(){
  char dummy;
  while(Serial.available() > 0) {
    dummy = Serial.read();
  }
}  

void pollSerialRead(){
  if(Serial.available()){
    //delayMicroseconds(200);
    serialRead0();
  }
}

//--------------------------------------------
float byteToFloat(byte b[4]) {
  float & f = (float &) *b;
  return f;
}

void serialRead0(){
  #define SERIALPORT Serial
  #define SERIALNUMBER 0
  #define SWRITE serialSingleWrite0
  #define RUNNER aT0
  #define CALLBACK ackToutDel0

  //BUFFEROVERFLOW PROTECTION
  //Serial buffer size is 64bytes 
  if(SERIALPORT.available()>60)
  {
    SERIALPORT.end();
    warningLog(BUFFEROVFMSG);
    delay(2);
    SERIALPORT.begin(BAUD, SERIAL_CONF);
    return;
  }

  //MAIN PROTOCOL
  
  byte reg=SERIALPORT.read();
  switch (reg){
    case ACKNOWLEDGE:
      ackState[SERIALNUMBER] = 0;
      ackToutCnt[SERIALNUMBER] = 0;
      RUNNER.setCallback(&CALLBACK);
      RUNNER.disable();
      debugLog(ACKRECMSG);
    break;
    case REQUEST:
      SWRITE(RESPONSE);
    break;
    case RESPONSE:
      serialTestState[SERIALNUMBER]  =  1;
      infoLog(SELFTESTMSG);
    break;
    default://default case: float variables
      readData:
      if (SERIALPORT.available()>=4) {
        //package size okay
        counter[SERIALNUMBER][1]=0;//resetting counter due to successful read of data
        SERIALPORT.readBytes(serial_buffer[SERIALNUMBER],4);
        float & f = (float &) *serial_buffer[SERIALNUMBER];
        debugLog(RECDATAMSG);
        //DEBUG
        /*
        Serial.print("SP\t");
        Serial.print(reg);
        Serial.print("\t");
        Serial.println(f, 5);
        //*/
        //REGISTER ALLOCATION
        switch(reg){
          case SET_VOLTAGE_MPC:
          
          break;
          default://if no allocation was defined: warning
            warningLog(REGNOTALLOCMSG);
          break;
        }
        return;
      }
      //ERROR CORRECTION
      else{
        counter[SERIALNUMBER][0] = 0;
        while (SERIALPORT.available()<4){//If data progression isn't complete
          delayMicroseconds(1);
          if(counter[SERIALNUMBER][0]==100){
            warningLog(WRPACKETSIZMSG);
            SERIALPORT.end();
            delay(2);
            SERIALPORT.begin(BAUD, SERIAL_CONF);
            counter[SERIALNUMBER][1]++;
            if (counter[SERIALNUMBER][1]==10){
              errorLog(TENFAILMSG);
              counter[SERIALNUMBER][1] = 0;
            }
            return;
          }
          counter[SERIALNUMBER][0]++;
          //Serial.println(cnt1);
        }
        goto readData;
      }
    break;
  }
}
