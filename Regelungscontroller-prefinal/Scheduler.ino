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

Task soll(10000, TASK_FOREVER, &sollwert);

Task aT0(100000, TASK_FOREVER, &ackToutDel0);

void sch_init(){
  runner.init();
  
  runner.addTask(sR0);

  runner.addTask(aT0);
  runner.addTask(soll);

  runner.addTask(contr);

  //sR0.enable();

  contr.enable();
  //1soll.enable();
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
  Usoll=35;
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
  h = (float) voltage[adc_voltage_port] * ratio[adc_voltage_port]; //actual voltage
  FilterVoltage.input(h);
  x = (float) FilterVoltage.output() * U_28;
  error = (float) Usoll - x;//control deviation  Usoll - x
  a= (float) error * U_4;
  b= (float) ((d-e)/*-(out-k)*/) * U_11;
  ab = (float) a+b;
  i1 += (float) ab * timeChange * (KI_14/TI_14);//integrator
  i1 = (float)constrain(i1, -100000000, 100000000);
  c= (float) i1 + a * U_5;
  cd= (float)(-1) * c;
  i= (float) voltage[adc_current_port] * ratio[adc_current_port];//actual current
  FilterCurrent.input(i);
  g= (float) FilterCurrent.output(); //strom
  d= (float) cd / (x / g);
  e= (float) constrain(d, (-1) * Isoll, LIMIT_HIGH);//current limit block
  ef= (float) e * (x / g);
  f= (float) ef *  U_25 * U_15;
  k= (float)(0.9339975* f + 5)/5 * res;
  //f 0-5V –> indirect proportional
  out = (long) constrain(k, 0, res);//constrain for dac output range 
  dacWrite(CMD_WRITE_UPDATE, ADR_DAC_B, intToByte(out)); //DAC output
}

