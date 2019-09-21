//---System checks---

void serialTest(){//check for availability of all  participants
  byte cnt = 0;
  serialSingleWrite0(REQUEST);
  serialSingleWrite1(REQUEST);
  serialSingleWrite2(REQUEST);
  serialSingleWrite3(REQUEST);
  while (!serialTestState[0] || !serialTestState[1] || !serialTestState[2] || !serialTestState[3]){
    pollSerialRead();
    delay(5);
    if (cnt == 100){
      criticalStop("Serial test failed!");
    }
    cnt++;
  }
  infoLog("System selftest passed!");
}
