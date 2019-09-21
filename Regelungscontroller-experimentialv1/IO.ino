//---Input & Output operations---

void initPins(){
  //PIN Setup
  pinMode(PIN_PWOK, INPUT);
  pinMode(PIN_PSON, OUTPUT);
  digitalWrite(PIN_PSON, HIGH);
  
  pinMode(PIN_RX, INPUT_PULLUP);
  pinMode(PIN_TX, INPUT_PULLUP);
  
  pinMode(PIN_VSENSE, INPUT);

  pinMode(PIN_LEDCURR, OUTPUT);
  digitalWrite(PIN_LEDCURR, LOW);
  pinMode(PIN_LEDVOLT, OUTPUT);
  digitalWrite(PIN_LEDVOLT, LOW);
  
  
  pinMode(PIN_DACSS, OUTPUT);
  digitalWrite(PIN_DACSS, HIGH);
  pinMode(PIN_ADCSS, OUTPUT);
  digitalWrite(PIN_ADCSS, HIGH);
  pinMode(PIN_CNV, OUTPUT);
  adcConversion();
  pinMode(PIN_BUSY, INPUT);
}

void setOutput(bool state){//set output State
  if(state) digitalWrite(PIN_PSON, LOW);
  else      digitalWrite(PIN_PSON, HIGH);
    
}

void changeMode(uint8_t mode){//change ADC inputs according to mode
  if(output_state == 0){
    adc_voltage_port = 0;
    return;
  }
  switch(mode){
    case 0:
    adc_voltage_port = 0;
    adc_current_port = 3;
    Serial.println("Mode 0");
    break;
    case 1:
    if(external_measure)adc_voltage_port = 1;
    else                adc_voltage_port = 2;
    adc_current_port = 3;
    Serial.println("Mode 1");
    break;
    case 2:
    if(external_measure)adc_voltage_port = 5;
    else                adc_current_port = 4;
    adc_current_port = 3;
    Serial.println("Mode 2");
    break;
    case 3:
    //not implemented
    break;
  }
}

void fanState(bool state){//normal or high fan state
  if (state) {
    controlStatusRegister |= (1 << FAN_HI_BIT);
    setControlStatusRegister(controlStatusRegister); // SET FAN HIGH
    delay(I2C_MESSAGE_DELAY);
  } else {
    controlStatusRegister &= ~(1 << FAN_HI_BIT);
    setControlStatusRegister(controlStatusRegister); // SET FAN LOW
    delay(I2C_MESSAGE_DELAY);
  }
}
