//---Battery monitor IC communication---

void cellDischarge(){
  int8_t error = 0;

  tx_cfg[0][4] = 0; //resetting dcc bits
  tx_cfg[0][4] = (DCC[0] * B00000001) | (DCC[1] * B00000010) | (DCC[2] * B00000100) | (DCC[3] * B00001000) | (DCC[4] * B00010000) | (DCC[5] * B00100000) | (DCC[6] * B01000000)  | (DCC[7] * B10000000);
  tx_cfg[0][5] = tx_cfg[0][5] & B11110000; //resetting dcc bits
  tx_cfg[0][5] = (DCC[8] * B00000001) | (DCC[9] * B00000010) | (DCC[10] * B00000100) | (DCC[11] * B00001000);
  
  wakeup_sleep();
  LTC6804_wrcfg(1,tx_cfg); //number of monitor ics , array to read
  delay(10);
  error = LTC6804_rdcfg(1,rx_cfg); //number of monitor ics , array to write
      if (error == -1){
        D("A PEC error was detected in the received data");
  }
  if(tx_cfg[0][4] == rx_cfg[0][4] && tx_cfg[0][5] == rx_cfg[0][5]){
  }
  else{
    D("Config falsch");
  }
}

void readCells(){
  int8_t error = 0;
  wakeup_sleep();
  LTC6804_wrcfg(1,tx_cfg);
  wakeup_idle();
  LTC6804_adcv();
  delay(10);
  wakeup_idle();
  error = LTC6804_rdcv(0, 1,cell_codes);
  if (error == -1)
  {
    D("A PEC error was detected in the received data");
  }
}

void readAux(){
  int8_t error = 0;
  wakeup_sleep();
  LTC6804_adax();
  delay(3);
  error = LTC6804_rdaux(0,1,aux_codes); // Set to read back all aux registers
      if (error == -1)
      {
        D("A PEC error was detected in the received data");
      }  
}

void checkCellVoltage(){//comparing voltages with set maximum cell voltage
  for(int i=0; i<12; i++){
    if(cell_codes[0][i]*1e-4 > dcc_voltage)DCC[i]=1;
    else DCC[i]=0;
  }
}

void checkTemperature(){
  float temp[3];
  for(int i=0; i<3; i++){
    temp[i] = aux_codes[0][i]/5000*TEMP_COEF +25;//has to be modified
    if(aux_codes[0][i]*1e-4 > max_temp){
      //thermal shutdown, current limiting....
    }
  }
}

