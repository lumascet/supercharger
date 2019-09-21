//---I2C Powersupply communication---

// I2C Library, mit Erweiterung 
// Arduino analog input 4 = I2C SDA
// Arduino analog input 5 = I2C SCL

/*
 Testfragen
 Ist die CPU I2C Adresse ok, sonst mal mit Broadcast und mal mit EEPROM probieren
 Wenn das Lesen nicht geht dann mal das Ctrl Register setzen, mit der CMD ID 0x01 und 0x02 probieren
 (denn dies kann unterschiedlich sein)
 
*/

/*
  Convert a HP 3kW Powersupply to a Battery Charger
 
  I2C Access Test 
  
  There are two I2C addresses, FRU EEPROM and Microcontroller
  Default Address (if A0,A1,A2 is open or connectet to High)
  FRU: 0xAE --> B10101110
  CPU: 0x3E --> B00111110
  
  Also Broadcast Address is possible
  Broadcast: 0x00  --> B00000000
  
  I2C Clock Frequency
  10 to 100 kHz is possible (Standard I2C)
  
  I2C Delay Time between two I2C Messages
  Minimum 50ms
*/



/*
void setup() {
  
  Serial.begin(250000);
  Serial.println("Temperaturlogger");
  initI2C(); //Initialise the i2c bus mit verbindung zu HP Netzteil

}
*/


/*
void loop() {
  //Serial.println("readVersion");  
  //readFirmwareRevision();
 
  //Serial.println("readCtrlReg");  
  readControlStatusRegister();
  
  delay(I2C_MESSAGE_DELAY);
  
  // Mit Input 7 kann der Powerausgang aktiviert und deaktivert werden
  bool val = True;//digitalRead(7);
  if (val) {
    controlStatusRegister &= ~(1 << OUTPUT_DISABLE_BIT);
    setControlStatusRegister(controlStatusRegister); // Enable Power Output
  } else {
    controlStatusRegister |= (1 << OUTPUT_DISABLE_BIT);
    setControlStatusRegister(controlStatusRegister); // Disable Power Output B00001000
  }
  
  delay(I2C_MESSAGE_DELAY);
  
  // Mit Input 6 kann der LÃ¼fter auf Vollspeed gesetzt werden
  val = digitalRead(6);
  if (val) {
    controlStatusRegister |= (1 << FAN_HI_BIT);
    setControlStatusRegister(controlStatusRegister); // Enable Power Output
  } else {
    controlStatusRegister &= ~(1 << FAN_HI_BIT);
    setControlStatusRegister(controlStatusRegister); // Disable Power Output B00001000
  }

  delay(I2C_MESSAGE_DELAY);
  
   //Serial.println("readAnalogSensors");
   readAnalogSensors();
  
}
*/

#define I2C_CPU_ADDR         B0011111 // Highest 7Bit for the Address, the last bit is R/W Flag and added by the lib 
#define I2C_FRU_EEPROM_ADDR  B1010111 // Highest 7Bit of the Address, the last bit is R/W Flag and added by the lib
#define I2C_BROADCAST_ADDR   B0000000 // Broadcast Adresse

/*
// CommandsIDs for HPS3KW only
#define HP3KWPS_SCRC  0x02 // (Set_Control_Register_Command)
#define HP3KWPS_RSRC  0x03 // (Read_Status_Register_Command)
#define HP3KWPS_RASDC 0x01 // (Read_Analog_Sensor_Data_Command)
*/

// CommandIDs for AA21970
#define HP3KWPS_SCRC  0x01 // (Set_Control_Register_Command)
#define HP3KWPS_RSRC  0x02 // (Read_Status_Register_Command)
#define HP3KWPS_RASDC 0x03 // (Read_Analog_Sensor_Data_Command)


// CommandIDs for AA21970 and HP3KW
#define HP3KWPS_TMC   0x04 //(Test_Mode_Command)
#define HP3KWPS_FDC   0x05 // (Firmware_Debug_Command)
#define HP3KWPS_FRNC  0x06 // (Firmware_Revision_Number_Command)
#define HP3KWPS_STMC  0x07 // (System_Test_Mode_Command)
#define HP3KWPS_STDFC 0x08 // (System_Test_Data_Format_Command)
#define HP3KWPS RRAMC 0x09 // (Read_RAM_Command)
#define HP3KWPS_RSFRC 0x0A // (Read_SFR_Command)


void initI2C() {
  i2c_init();          // join i2c bus
  slaveAddress = I2C_CPU_ADDR<<1;  // Prepare Slave Adressbyte 
}

void reinitI2C() {
  i2c_disableTWI();
  delay(1);
  i2c_init();          // join i2c bus
  slaveAddress = I2C_CPU_ADDR<<1;  // Prepare Slave Adressbyte 
}

/*
Lesen der Firmware Revision 
 
Firmware Revision Number Command (Command #6)
  Firmware Revision Number will be hard coded into the Firmware itself. This command will return the revision
  number of the Firmware. The Firmware Revision Number Command will respond to the following two
  formats:
  
MCU Command Syntax and Packet Length:
  Command 6:   Firmware Revision Number
  Writes:      1 byte
  Syntax:      CMD#6
  Reads:       2 bytes
               1. Major Revision Number
               2. Minor Revision Number
I2C Frameaufbau
  |Start|Slave Address+W|A|0x06|A|Repeated Start|Slave Address+R|A|Major Revision|A|Minor Revision|NA|Stop|

*/
void readFirmwareRevision() {
    commandByte  = HP3KWPS_FRNC;
    bVal = i2c_start(slaveAddress+I2C_WRITE); // set device address and write mode
    if (bVal) {
      reinitI2C();
      Serial.println("start: dev not ready");
      return;
    }
    i2c_write(commandByte);                 // write a databyte to i2c device (command)
    bVal = i2c_rep_start(slaveAddress+I2C_READ);   // set same device address and define read mode
    if (bVal) {
      reinitI2C();
      Serial.println("repstart: dev not ready");
      return;
    }
    majorVersion = i2c_readAck();              // first read is with commandid defined port, readAck define that we will read more bytes from the device
    minorVersion = i2c_readNak();              // next read is the lower port (see documentation), readNak say we are finished with reading
    i2c_stop();
  
    Serial.print("Version ");
    Serial.print(majorVersion);
    Serial.print("/ ");
    Serial.println(minorVersion);  
}

/*
Lesen und Schreiben des Control Status Register
  
Schreiben mit CommandID #1 HP3KWPS_SCRC (???)
Lesen mit CommandID #2 HP3KWPS_RSRC (????)
  
  
Control Status Register
   BIT 7      BIT 6    BIT 5   BIT 4          BIT 3         BIT 2    BIT 1    BIT 0
   PSON_STAT  BAD_CAL  FAN_HI  SELFTEST_FAIL  ROUT_DISABLE  OC_TRIP  OV_TRIP  OT_TRIP
   R          R        R/W     R              R/W           R        R        R
  
* PSON_STAT: This bit is an image of the PSON# signal coming into the power supply from the system.
* BAD_CAL: This bit will be set to indicate a corrupted calibration table. Under this condition the response
to a request for analog data shall be all zeros.
* FAN_HI: Setting this bit will cause the fan run full speed. Reading this bit will tell if the fan is running at
full speed due to internal temperatures or in response to an I2C command.
* SELFTEST_FAIL: This bit will be set to indicate a power supply self-test failure. Under this condition the
response to a request for analog data shall be all zeros.
* ROUT_DISBLE: Remote Output Disable. Setting this bit will disable the main outputs and clearing this bit
will enable the main output.
* OC_TRIP: Over current trip status. This bit is set when the power supply output has been disabled due to
an over current event. In this condition, the Inlet Air temperature will be sensed only when the Amber LED is
off (Every other second) and the ADC update flag will be set according to the other three ADC channel
reading status.
* OV_TRIP: Over voltage trip status. This bit will be set when the power supply outputs have been
disabled and the power supply temperature has not reached the trip limits. This is not a direct sensing of the
over voltage trip circuit.
* OT_TRIP: Over temperature status. This bit will be set when the power supply outputs have been
disabled and the power supply temperature is past the trip limit. This is not a direct sensing of the over
temperature trip circuit.

*/
void readControlStatusRegister() {
    commandByte  = HP3KWPS_RSRC;
    bVal = i2c_start(slaveAddress+I2C_WRITE); // set device address and write mode
    if (bVal) {
      reinitI2C();
      Serial.println("start: dev not ready");
      return;
    }
    i2c_write(commandByte);                 // write a databyte to i2c device (command)
    
    bVal = i2c_rep_start(slaveAddress+I2C_READ);   // set same device address and define read mode
    if (bVal) {
      reinitI2C();
      Serial.println("repstart: dev not ready");
      return;
    }
    controlStatusRegister = i2c_readNak();              // read one byte
    i2c_stop();
  
    //Serial.print("CtrlReg ");
    //Serial.println(controlStatusRegister);
}

void setControlStatusRegister(int valueByte) {
    commandByte  = HP3KWPS_SCRC;
    checksumm    = valueByte + valueByte;
    bVal = i2c_start(slaveAddress+I2C_WRITE); // set device address and write mode
    if (bVal) {
      reinitI2C();
      Serial.println("start: dev not ready");
      return;
    }
    i2c_write(commandByte);                 // write a databyte to i2c device (command)
    i2c_write(valueByte);                  // write a databyte to i2c device (command)
    i2c_write(valueByte);                  // write a databyte to i2c device (command)
    i2c_write(checksumm);                  // write a databyte to i2c device (command)
    i2c_stop();    
}

/*
  Read Analog Sensor Data
  
  
*/

void readAnalogSensors() {
    int i;
    commandByte  = HP3KWPS_RASDC;
    bVal = i2c_start(slaveAddress+I2C_WRITE); // set device address and write mode
    if (bVal) {
      reinitI2C();
      Serial.println("start: dev not ready");
      return;
    }
    i2c_write(commandByte);                 // write a databyte to i2c device (command)
    bVal = i2c_rep_start(slaveAddress+I2C_READ);   // set same device address and define read mode
    if (bVal) {
      reinitI2C();
      Serial.println("repstart: dev not ready");
      return;
    }
    for (int i=0; i<17; i++) {
      analogSensor[i] = i2c_readAck();
    }
    analogSensor[17] = i2c_readNak();
    i2c_stop();

    i++;
    Serial.print(i);
    Serial.print("; "); 
    Serial.print(analogSensor[11]); //Temp Luftstrom Eingang
    Serial.print("; ");
    Serial.println(analogSensor[14]); //Temp Luftstrom Ausgang
    delay(1000);
/*
    for (int i=0; i<18; i++) {
      Serial.print(i+1);
      Serial.print(" ");
      Serial.println(analogSensor[i]);
    }
*/
}




