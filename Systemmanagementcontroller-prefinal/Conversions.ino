//---Converting datatypes---

//from String Conversions
float toFloat(String val){
  char floatbuf[val.length()+1];
  val.toCharArray(floatbuf, sizeof(floatbuf));
  float f = atof(floatbuf);
  return f;
}
  
long toLong(String val){
  char longbuf[val.length()+1];
  val.toCharArray(longbuf, sizeof(longbuf));
  long l = atol(longbuf);
  return l;
}

boolean toBoolean(String val) {
  if(val.toInt()==1){
    return true;
  } 
  else {
    return false;
  }
}

int toInt(String val){
  int i  = val.toInt();
  return i;
}

//from Int
String intToString(int val){
  return String(val);
}

uint8_t getLength(int val){
  uint8_t l ;
  if (val < 1) l = 1; //catch exception
  else l = log10(val) + 1;
  return l;
}

//from float
String floatToString(float val, int decimals){//crash
  uint8_t length;
  char* cinsert;
  if(decimals != 0) length = getLength(val) + 1 + decimals;
  else length = getLength(val) + decimals;
  dtostrf(val, length, decimals, cinsert);
  String str(cinsert);
  return str;
}

//to binary
byte* floatToByte(float data) {
  byte* b = (byte *) &data;
  return b;
}

byte* intToByte(int data) {
  byte* b = (byte *) &data;
  return b;
}

byte* longToByte(long data) {
  byte* b = (byte *) &data;
  return b;
}



