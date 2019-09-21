//---SPI ADC communication---

void adcConversion(){
  digitalWrite(PIN_CNV,HIGH); //Conversion starts on pulse
  digitalWrite(PIN_CNV,LOW);
}

void adcRead(){
  uint8_t i, j;
  uint8_t chan;
  uint8_t conf;
  int32_t data;
  uint8_t Result[24];

  while(digitalRead(PIN_BUSY));
  LTC2348_write(Result);//write softspan configuration, get array of 24byte#
  
  j = 0;
  for (i = 0; i < 8; ++i){ //get ADC values
    chan = (Result[j + 2] & CHANNEL_POSITION)>>3;//get channel number
    conf = Result[j + 2] & CONFIG_WORD_POSITION;//get configuration number (softspan)

    data = 0;
    data = (int32_t)Result[j] << 10;//get databits
    data |= (int32_t)Result[j + 1] << 2;
    data |= (int32_t)Result[j + 2] >> 6;
    voltage[chan] = LTC2348_voltage_calculator(data, chan);//calculate float out of databits & softspan configuration

    j = j + 3;//each channel has 3 bytes, 24 in total(3*8) moving 3 bytes on for reading out next one
  }
  /*
  //Debug printing
  for(i = 0; i < 8; ++i){
    Serial.print(i);
    Serial.print("\t");
    Serial.print(voltage[i],6);
    Serial.print("\t");
  }
  Serial.println();
  //*/
}
