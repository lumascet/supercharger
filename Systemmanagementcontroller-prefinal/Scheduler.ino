//---Scheduler---

#define NOACKSTOPMSG (String)F("Serial ") + (String) SERIALNUMBER + (String)F(": no acknowledge recieved for 5 times, STOPPING SYSTEM!")
#define NOACKREPMSG  (String)F("Serial ") + (String) SERIALNUMBER + (String)F(": no acknowledge recieved, REPEAT!")
//loop:
//runner.execute();
//init
//Scheduler runner;

void ackToutDel0();
void ackToutDel1();
void ackToutDel2();
void ackToutDel3();

void pollSerialRead0();
void pollSerialRead1();
void pollSerialRead2();
void pollSerialRead3();

void uiStepTask();
void refreshDisplay();
void sysTask();

Task sR0(10, TASK_FOREVER, &pollSerialRead0);
Task sR1(10, TASK_FOREVER, &pollSerialRead1);
Task sR2(10, TASK_FOREVER, &pollSerialRead2);
Task sR3(10, TASK_FOREVER, &pollSerialRead3);

Task aT0(100000, TASK_FOREVER, &ackToutDel0);
Task aT1(100000, TASK_FOREVER, &ackToutDel1);
Task aT2(100000, TASK_FOREVER, &ackToutDel2);
Task aT3(100000, TASK_FOREVER, &ackToutDel3);

Task controlTask(40, TASK_FOREVER, &uiStepTask);

Task refreshTask(100, TASK_FOREVER, &refreshDisplay);

Task systemTask(1000, TASK_FOREVER, &sysTask);

void sch_init(){//initialize scheduler tasks
  runner.init();
  
  runner.addTask(sR0);
  runner.addTask(sR1);
  runner.addTask(sR2);
  runner.addTask(sR3);

  runner.addTask(aT0);
  runner.addTask(aT1);
  runner.addTask(aT2);
  runner.addTask(aT3);

  runner.addTask(controlTask);
  runner.addTask(systemTask);
  runner.addTask(refreshTask);

  sR0.enable();
  sR1.enable();
  sR2.enable();
  sR3.enable();

  controlTask.enable();
  refreshTask.enable();
  systemTask.enable();
}


//---------------------------------------
void pollSerialRead0(){
  if(Serial.available()){
    serialRead0();
  }
}
void pollSerialRead1(){
  if(Serial1.available()){
    serialRead1();
  }
}
void pollSerialRead2(){
  if(Serial2.available()){
    serialRead2();
  }
}
void pollSerialRead3(){
  if(Serial3.available()){
    serialRead3();
  }
}
//----------------------------------

void ackToutDel0(){//runs at scheduler  activation, calls callback on scheduler delay so it dosn't block the main process
  aT0.setCallback(&ackTout0);
  aT0.delay(TIMEOUT);
}
void ackToutDel1(){
  aT1.setCallback(&ackTout1);
  aT1.delay(TIMEOUT);
}
void ackToutDel2(){
  aT2.setCallback(&ackTout2);
  aT2.delay(TIMEOUT);
}
void ackToutDel3(){
  aT3.setCallback(&ackTout3);
  aT3.delay(TIMEOUT);
}

void ackTout0(){
  #define SERIALNUMBER 0
  #define SERIALWRITE serialWrite0
  #define RUNNER aT0
  #define CALLBACK ackToutDel0
  
  if(ackToutCnt[SERIALNUMBER] == 4){//if there was no acknowledge within the timeout border
    ackToutCnt[SERIALNUMBER]=0;//resetting counter
    criticalStop(NOACKSTOPMSG);
  }
  ackState[SERIALNUMBER]=0;//setting ack state 0 for resending
  ackToutCnt[SERIALNUMBER]++;//increase counter by 1
  warningLog(NOACKREPMSG);//throw out warning
  SERIALWRITE(ackReg[SERIALNUMBER], ackData[SERIALNUMBER], true);//resend package
  RUNNER.setCallback(&CALLBACK);//set timeout callback
}

void ackTout1(){
  #define SERIALNUMBER 1
  #define SERIALWRITE serialWrite1
  #define RUNNER aT1
  #define CALLBACK ackToutDel1

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

void ackTout2(){
  #define SERIALNUMBER 2
  #define SERIALWRITE serialWrite2
  #define RUNNER aT2
  #define CALLBACK ackToutDel2

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

void ackTout3(){
  #define SERIALNUMBER 3
  #define SERIALWRITE serialWrite3
  #define RUNNER aT3
  #define CALLBACK ackToutDel3
  
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
  SdCheck();
}

void refreshDisplay(){//refresh display automatically after time
  switch (submenu_dotm) {
    case UPTIME:
      redraw_dotm = 1;
    break;
  }
}

void uiStepTask(){
  uiStep();
}

