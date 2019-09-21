//---SD-Card transactions---

File myFile;
String log_file;

void init_SD(){
  infoLog("Initializing SD card...");
  pinMode(53, OUTPUT);
  if (SD.begin()) {
    SD_available = 1;
    //create log file - if there are many log files the while loop takes long time
    if (!SD.exists("logs")) SD.mkdir("logs");
    unsigned int fileNum = 0;
    while (SD.exists("logs/log" + (String) fileNum + ".txt")){
      fileNum++;
    }
    log_file = "logs/log" + (String) fileNum + ".txt";
    //check if there is a profile list, if not create example file
    if(!SD.exists(file_profiles)){
      writeSD(file_profiles);
    }
    //check if there is a settings file, if not create file
    if(!SD.exists(file_settings)){
      writeSD(file_settings);
    }
    //check if there is a description, if not create file - not implemented yet
    if(!SD.exists(file_description)){
      myFile = SD.open(file_description);
      myFile.close();
    }
  }
  else{
    warningLog(F("SD KARTE NICHT VERFÜGBAR"));
    SD_available = 0;
  }
}

void SdCheck(){//periodically check, if sd card was removed
  if(!SD.exists("logs") && SD_available){
    SD_available = false;
    #if ENABLESERIALPRINT
    Serial.println(F("SD card disconnected!"));
    #endif
  }
}

void sdLogDirectWrite(String msg){//direct logging
  if (!SD_available || debug_log == 0) return;
  File dataFile = SD.open(log_file, FILE_WRITE);
  if (dataFile) {
    dataFile.println(msg);
    dataFile.close();
  }
}

void sdLogWrite(String type, String msg){//log with type
  if (!SD_available || debug_log == 0) return;
  File dataFile = SD.open(log_file, FILE_WRITE);
  if (dataFile) {
    dataFile.println(upTime() + " " + type + "\t" + msg);
    dataFile.close();
  }
}


void readAllProfiles(uint8_t number){//read Profilenames for Profile selection menu
  Profile profile = {"",0,0,0,0,0,0,0,0};
  Config config = {"a",""};
  profile_entries = 0;
  if(!SD_available){
    for(int i=0; i<number;i++){
      strcpy(buffer[i+2], no_SD_card);
    }
    return;
  }
  while(config.name != ""){
    config = readSD(file_profiles);
    for(int i=0; i<number;i++){
      if(config.name == "profile_name_" + (String)i) {
        config.value.toCharArray(profile.name,21);
        strcpy(buffer[i+2], profile.name);
        profile_entries++;
      }
    }
  }
  for(int i=profile_entries; i<number; i++){
    strcpy(buffer[i+2], empty_profile);
  }
  return;
}

Profile readProfile(uint8_t number){//read complete content of profile
  Profile profile = {"",0,0,0,0,0,0,0,0};
  Config config = {"a",""}; //starting variable
  if(!SD_available){
    strcpy(profile.name, no_SD_card);
    return profile;
  }
  profile.number = number;
  while(config.name != ""){
    config = readSD(file_profiles);
    if(config.name == "profile_name_" + (String)number) {
      config.value.toCharArray(profile.name,21);//convert to char array and cut at 21
      profile.name_length = strlen(config.value.c_str());//count length
    }
    else if(config.name.equals("balancing_" + (String)number))              profile.balancing = toBoolean(config.value);
    else if(config.name.equals("charge_voltage_" + (String)number))         profile.charge_voltage = toFloat(config.value);
    else if(config.name.equals("discharge_voltage_" + (String)number))      profile.discharge_voltage = toFloat(config.value);
    else if(config.name.equals("discharge_voltage_" + (String)number))      profile.discharge_voltage = toFloat(config.value);
    else if(config.name.equals("charge_current_" + (String)number))         profile.charge_current = toFloat(config.value);
    else if(config.name.equals("high_discharge_current_" + (String)number)) profile.high_discharge_current = toBoolean(config.value);
    else if(config.name.equals("cells_" + (String)number))                  profile.discharge_voltage = toInt(config.value);
    
  }
  if(profile.name == ""){//if profile number X is not available
    strcpy(profile.name, empty_profile);
  }
  /*
  Serial.println(profile.name);
  Serial.println(profile.number);
  Serial.println(profile.balancing);
  Serial.println(profile.charge_voltage);
  Serial.println(profile.discharge_voltage);
  */
  return profile;
}

void readSettings(){//read setting file
  Config config = {"a", ""};
  while(config.name != ""){
    config = readSD(file_settings);
    if(config.name == "encoderMode") {
      encoder_mode = toBoolean(config.value);
      Serial.println(encoder_mode);
    }
    if(config.name == "scrollMode") {
      scroll_mode = toBoolean(config.value);
      Serial.println(scroll_mode);
    }
  }
}

Config readSD(String fileName){//read config file formate: [variable=value]
  char character;
  Config config = {"",""};
  if(!SD_open){
    myFile = SD.open(fileName);
  }
  if (myFile) {
    SD_open = 1;
    while (myFile.available()) {
      character = myFile.read();
      while((myFile.available()) && (character != '[')){
        character = myFile.read();
      }
      character = myFile.read();
      while((myFile.available()) && (character != '=')){
        config.name += character;
        character = myFile.read();
      }
      character = myFile.read();
      while((myFile.available()) && (character != ']')){
        config.value += character;
        character = myFile.read();
      }
      if(character == ']'){
        //Serial.print(config.name);
        //Serial.println(config.value);
        return config;
      }
    }
    // close the file:
    myFile.close();
    SD_open = 0;
  } 
  else {
    SD_open = 0;
    D("error opening" +(String)fileName);
  }
}

void writeSD(String fileName) {//write file to sd card
  if(SD.exists(fileName)){
    SD.remove(fileName);
  }
  myFile = SD.open(fileName, FILE_WRITE);
  if(fileName.equals(file_settings)){//on settings write
    writeConfig(F("encoderMode"), (String)encoder_mode);
    writeConfig(F("scrollMode"), (String)scroll_mode);
  }
  else if(fileName.equals(file_profiles)){//on profile write
    writeConfig(F("profile_name_0"), "Demoprofil");
    writeConfig(F("balancing_0"), "1");
    writeConfig(F("charge_voltage_0"), "4.2");
    writeConfig(F("discharge_voltage_0"), "3.1");
    writeConfig(F("charge_current_0"), "20.5");
    writeConfig(F("high_discharge_current_0"), "0");
    writeConfig(F("cells_0"), "12");
    myFile.println(F("Dieses Beispiel enthält alle möglichen Einstellungswerte"));
    myFile.println(F("Werden Werte ausgelassen, so wird für diese die Standartkonfiguration übernommen."));
    myFile.println(F("Derzeit können bis zu zehn Profile (0-9) hinzugefügt werden."));
  }
  myFile.close();
}

void writeConfig(String name, String value){//write values in config file syntax
  myFile.print("[");
  myFile.print(name);
  myFile.print("=");
  myFile.print(value);
  myFile.println("]");
  //Serial.print(name);
  //Serial.println(value);
}

