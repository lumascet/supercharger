//---Scheduler---

#define NOACKSTOPMSG (String)F("Serial ") + (String) SERIALNUMBER + (String)F(": no acknowledge recieved for 5 times, STOPPING SYSTEM!")
#define NOACKREPMSG  (String)F("Serial ") + (String) SERIALNUMBER + (String)F(": no acknowledge recieved, REPEAT!")
//loop:
//runner.execute();
//init
//Scheduler runner;

void ackToutDel0();
void pollSerialRead0();
void rotEnc();
void sysTask();

Task sR0(10, TASK_FOREVER, &pollSerialRead0);
Task aT0(100000, TASK_FOREVER, &ackToutDel0);
Task sT(1000, TASK_FOREVER, &sysTask);

void schInit(){
  runner.init();
  runner.addTask(sR0);
  runner.addTask(aT0);
  runner.addTask(sT);

  sR0.enable();
  sT.enable();
}





//---------------------------------------
void pollSerialRead0(){
  if(Serial.available()){
    serialRead0();
  }
}
//----------------------------------

void ackToutDel0(){
  aT0.setCallback(&ackTout0);
  aT0.delay(TIMEOUT);
}

void ackTout0(){
  #define SERIALNUMBER 0
  #define SERIALWRITE serialWrite0
  #define RUNNER aT0
  #define CALLBACK ackToutDel0
  
  if(ackToutCnt[SERIALNUMBER] == 4){
    ackToutCnt[SERIALNUMBER]=0;
    criticalStop(NOACKSTOPMSG);
  }
  ackState[SERIALNUMBER]=0;
  ackToutCnt[SERIALNUMBER]++;
  warningLog(NOACKREPMSG);
  SERIALWRITE(ackReg[SERIALNUMBER], ackData[SERIALNUMBER], true);
  RUNNER.setCallback(&CALLBACK);
}


//---------------------
void sysTask(){
}
