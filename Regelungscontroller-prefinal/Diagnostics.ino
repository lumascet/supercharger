//---Diagnostic data&logging---

void debugLog(String debug){
  D((String) F("[DEBG]\t") + debug);
}

void infoLog(String info){
  D((String) F("[INFO]\t") + info);
}

void warningLog(String warning){
  D((String) F("[WARN]\t") + warning);
}

void errorLog(String error){
  D((String) F("[ERRO]\t") + error);
}

void criticalStop(String error){
  D((String) F("[CRIT]\t") + error);
  //send sth to mega
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

unsigned long start; 
unsigned long dauer;
  
void cycle(bool state){
 if(state){
   start = micros();
 }
 else{
    dauer = micros() - start;
    Serial.println(dauer);
  }
}

/*
  cycle(1);             //STARTE TIMER
  //HIER DAS PROGRAMM
  cycle(0);             //STOPPE TIMER
 */


