//---Main Routine---

bool debug_log = 1;
bool debug_print = 1;

//DEBUG SHORTCUTS
#define D(x) if(debug_print) Serial.println(x) //DEBUG NORMAL
#define DR(x) if(debug_print) Serial.print(x) //DEBUG no return
#define DE(x) if(debug_print) Serial.println("[A]" + (String)x + "[E]") //DEBUG EXTENDED

#include <SPI.h>
#include "HardwareAllocation.h"
#include "LTC2348.h"
#include "GlobalVar.h"
#include "TaskScheduler.h"
#include "i2cmaster.h" 
#include "Serial.h"
#include <Filters.h>

Scheduler runner;

// filters out changes faster that 125 Hz.
float filterFrequency = 20;

// create a one pole (RC) lowpass filter
FilterOnePole FilterVoltage( LOWPASS, filterFrequency );
FilterOnePole FilterCurrent( LOWPASS, filterFrequency );   

void setup() {
  uint8_t dummy[24];
  
  initPins();
  Serial.begin(250000);
  SPI.begin();
  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0)); //CPOL=0, CPA=0, f=2MHz
  dacWrite(CMD_WRITE_UPDATE, ADR_DAC_AB, intToByte(0));
  LTC2348_write(dummy);//write softspan configuration, get array of 24byte
  delay(100);

  initI2C();//initialize i2c
  readControlStatusRegister();
  sch_init();
  //register Output State: always on
  if (1) {
    controlStatusRegister &= ~(1 << OUTPUT_DISABLE_BIT);
    setControlStatusRegister(controlStatusRegister); // Enable Power Output
    delay(I2C_MESSAGE_DELAY);
  } else {
    controlStatusRegister |= (1 << OUTPUT_DISABLE_BIT);
    setControlStatusRegister(controlStatusRegister); // Disable Power Output B00001000
    delay(I2C_MESSAGE_DELAY);
  }
  
  fanState(fan_high);//set fan state to value
  
  D("BEFORE");
  printState();
  setOutput(output_state);//change output state
  D("AFTER");
  printState();
}

void loop() {
  runner.execute();
  //cycle(1);



  
  //cycle(0);
  //Serial.println();
  //cycle(1);
  //Serial.println();
  
  adcConversion();//do ADC Conversion
  external_measure = digitalRead(PIN_VSENSE);
  
  now = micros(); // micros, overflow after 70 min! /millis, overflow after 42 days
  timeChange = (float)(now - lastTime) / 1000000; // calculate delta t
  lastTime = now;

  
  if(!output_state){
    g=0;
  }
  if(update_state){
    changeMode(operating_mode);
    fanState(fan_high);
    setOutput(output_state);
    update_state = 0;
  }
  if(Serial.available() != 0){
    int md = Serial.parseInt();
    switch(md){
      case 1://set value
        Serial.print("Change Uset: ");
      break;
      case 2://op_mode
        Serial.print("Change OPmod: ");
      break;
      case 3://Output
        Serial.print("Change Output: ");
      break;
      case 4://Output
        Serial.print("Debug info: ");
      break;
      case 5://fan high
        Serial.print("fan high: ");
      break;
      case 6:
        Serial.print("print parameters: ");
      break;
      case 7:
        Serial.print("Change Iset: ");
      break;
    }
    switch(md){
      case 1://set value
        Usoll = Serial.parseFloat();
        //g=0;
        lastTime = micros();
        Serial.print("Usoll: ");
        Serial.println(Usoll);
      break;
      case 2://op_mode
        operating_mode = Serial.parseInt();
        update_state = 1;
        Serial.print("OPmod: ");
        Serial.println(operating_mode);
      break;
      case 3://Output
        output_state = Serial.parseInt();
        setOutput(output_state);
        Serial.print("output_state: ");
        Serial.println(output_state);
        //g=0;
      break;
      case 4://Debug info
        debug_info = Serial.parseInt();
        Serial.print("debug_info: ");
        Serial.println(debug_info);
      break;
      case 5://
        fan_high = Serial.parseInt();
        update_state = 1;
        Serial.print("fan_high: ");
        Serial.println(fan_high);
      break;
      case 6:
        parameters_info = Serial.parseInt();
        update_state = 1;
        Serial.print("parameters: ");
        Serial.println(parameters_info);
      break;
      case 7:
        Isoll = Serial.parseFloat();
        //lastTime = micros();
        Serial.print("Isoll: ");
        Serial.println(Isoll);
      break;
    }
  }
  
  

  //delay(100);

  if(debug_info){//only important debug info
    printState();
  }
  if(parameters_info){//detailed control loop info
    printVal();
  }
}



void printState(){//for debugging purposes only
  Serial.print("PIN_PSON: ");
  Serial.println(digitalRead(PIN_PSON));

  Serial.print("PIN_PWOK: ");
  Serial.println(supply_output);

  Serial.print("Power Supply Voltage: ");
  Serial.println(voltage[adc_voltage_port] * ratio[adc_voltage_port]);

  Serial.print("Power Supply Current: ");
  Serial.println(voltage[adc_current_port] * ratio[adc_current_port]);
}

void printVal(){//for debugging purposes only
  Serial.print("Usoll: ");
  Serial.println(Usoll);
  Serial.print("Isoll: ");
  Serial.println(Isoll);
  Serial.print("a: ");
  Serial.println(a);
  Serial.print("b: ");
  Serial.println(b);
  Serial.print("c: ");
  Serial.println(c);
  Serial.print("d: ");
  Serial.println(d);
  Serial.print("e: ");
  Serial.println(e);
  Serial.print("f: ");
  Serial.println(f);
  Serial.print("g: ");
  Serial.println(g);
  Serial.print("h: ");
  Serial.println(h);
  Serial.print("out: ");
  Serial.println(out);
  Serial.print("u: ");
  Serial.println(u);
  Serial.print("i: ");
  Serial.println(i);
  Serial.print("r: ");
  Serial.println(r);
}

