//---Diagnostic data&logging---

void debugLog(String debug){
  D((String) F("[DEBG]\t") + debug);
  sdLogWrite(F("[DEBG]"), debug);
}

void infoLog(String info){
  D((String) F("[INFO]\t") + info);
  sdLogWrite(F("[INFO]"), info);
}

void warningLog(String warning){
  D((String) F("[WARN]\t") + warning);
  sdLogWrite(F("[WARN]"), warning);
  buzzer.duration = 100;
  buzzer.count = 1;
}

void errorLog(String error){
  D((String) F("[ERRO]\t") + error);
  sdLogWrite(F("[ERRO]"), error);
  buzzer.duration = 100;
  buzzer.count = 2;
}

void criticalStop(String error){
  D((String) F("[CRIT]\t") + error);
  sdLogWrite(F("[CRIT]") , error);
  buzzer.duration = 100;
  buzzer.count = 3;
  while (1){
    delay(1000);
    buzzer.duration = 50;
    buzzer.count = 1;
  }
}

String upTime(){//returns the current uptime of the arduino in this format hh:mm:ss:msmsms
  String uptime = "";
  int t_sec, t_min, t_hrs, t_millis;
  unsigned long runtime = millis(); //49 days till rollover

  t_millis = runtime % 1000;
  t_sec = (runtime / 1000) % 60;
  t_min = runtime / 60000 - (runtime / 3600000) * 60;
  t_hrs = runtime / 3600000;
  if ((t_hrs - 9) <= 0) {
    uptime += "0";
  }
  uptime += (String) t_hrs + ":";
  if ((t_min - 9) <= 0) {
    uptime += "0";
  }
  uptime += (String) t_min + ":";
  if ((t_sec - 9) <= 0) {
    uptime += "0";
  }
  uptime += (String) t_sec + ":";
  if ((t_millis - 99) <= 0) {
    uptime += "0";
    if ((t_millis - 9) <= 0) {
      uptime += "0";
    }
  }
  uptime += (String) t_millis;
  return uptime;
}


