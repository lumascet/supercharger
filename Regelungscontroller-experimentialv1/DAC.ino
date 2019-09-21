//---SPI DAC communication---

void dacWrite(byte command, byte address, byte value[2]){

  byte cmdAddr = (command << 4) + address;
  digitalWrite(PIN_DACSS, LOW);//Slaveselect LOW
  SPI.transfer(cmdAddr);//write address of DAC (A/B/AB)
  SPI.transfer(value[1]);//write first byte of integer
  SPI.transfer(value[0]);//write second byte of integer
  digitalWrite(PIN_DACSS, HIGH); //Slaveselect HIGH
  /*
  Serial.println(command,BIN);
  Serial.println(address,BIN);
  Serial.println(cmdAddr,BIN);
  Serial.print(value[1],BIN);
  Serial.print("-");
  Serial.println(value[0],BIN);
  Serial.println("------------");
  */
}
