//---Scheduler---

#define NOACKSTOPMSG (String)F("Serial ") + (String) SERIALNUMBER + (String)F(": no acknowledge recieved for 5 times, STOPPING SYSTEM!")
#define NOACKREPMSG  (String)F("Serial ") + (String) SERIALNUMBER + (String)F(": no acknowledge recieved, REPEAT!")
//loop:
//runner.execute();
//init
//Scheduler runner;

void ackToutDel0();

void controller();

void pollSerialRead0();

void sollwert();

Task sR0(10, TASK_FOREVER, &pollSerialRead0);

Task contr(3, TASK_FOREVER, &controller);

Task soll(5000, TASK_FOREVER, &sollwert);

Task aT0(100000, TASK_FOREVER, &ackToutDel0);

void sch_init(){
  runner.init();
  
  runner.addTask(sR0);

  runner.addTask(aT0);
  runner.addTask(soll);

  runner.addTask(contr);

  //sR0.enable();

  contr.enable();
  soll.enable();
}

void pollSerialRead0(){
  if(Serial.available()){
    serialRead0();
  }
}

bool sw = 0;

void sollwert(){
 if(sw){
   Usoll = 40;
 }
 else
 {
  Usoll=30;
 }
 sw = !sw;
}

void ackToutDel0(){//runs at scheduler  activation, calls callback on scheduler delay so it dosn't block the main process
  aT0.setCallback(&ackTout0);
  aT0.delay(TIMEOUT);
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

void controller(){
  adcConversion();//do ADC Conversion
  external_measure = digitalRead(PIN_VSENSE);

  now = micros(); // micros, overflow after 70 min! /millis, overflow after 42 days
  timeChange = (float)(now - lastTime) / 1000000; // calculate delta t
  lastTime = now;

  //Controller Algorithm
  adcRead();//read out ADC voltages, write on voltage[]
  u = (float) voltage[adc_voltage_port] * ratio[adc_voltage_port]; //actual voltage
  i= (float) voltage[adc_current_port] * ratio[adc_current_port];//actual current
  r= (float) constrain(u / i,0,10000000);
  b= (float) Usoll / r;
  c= (float) constrain(b, 0, Isoll);
  d= (float) c*r;
  e= (float) 0.0001 + d;
  f= (float) e - u;
  j += (float) f * timeChange * (1/TI);//integrator
  g = (float) f * KP + j;
  //g = (float)constrain(g, 0, 5);
  h = (float) ((-0.7)*g + 3.6)/5 * res;
  //h 0-5V –> indirect proportional
  out = (long) constrain(h, 0, res);//constrain for dac output range 
  //out = (long) constrain(Usoll, 0, res);
  dacWrite(CMD_WRITE_UPDATE, ADR_DAC_B, intToByte(out)); //DAC output
}

