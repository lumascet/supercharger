//---Global variables---

//Controller VARIABLES & CONSTANTS---------------------------
#define KI 0.00021 //800 
#define TI 0.00005

float Usoll=40;
float Isoll=10;
float a=0;
float b=0;
float c=0;
float d=0;
float e=0;
float f=0;
float g=0;
float h=0;
float i=0.0001;
float u=0.0001;
float r=0;
long out=1;

unsigned long now=0;
unsigned long lastTime=0;
float timeChange;

//DAC VARIABLES & CONSTANTS----------------------------------
#define CMD_WRITE_INPUT_REG             B0000
#define CMD_UPDATE_REG                  B0001
#define CMD_WRITE_INPUT_AND_UPDATE_ALL  B0010
#define CMD_WRITE_UPDATE                B0011
#define CMD_POWER_DOWN                  B0100
#define CMD_NO_OPERATION                B1111

#define ADR_DAC_A                       B0000
#define ADR_DAC_B                       B0001
#define ADR_DAC_AB                      B1111

const float vref = 4.096;
const uint16_t res = 65535;

//ADC VARIABLES & CONSTANTS-----------------------------------

#define CONFIG_WORD_POSITION  B111
#define CHANNEL_POSITION      B111000

/*
ADC CHANNEL USAGE

CH0: Voltage sense dedicaded powersupply
CH1: Voltage sense dedicaded output
CH2: Voltage sense dedicaded output external
CH3: Current sense dedicaded powersupply

POWER SUPPLY MASTER CONTROLLER ONLY:

CH4: Voltage sense serial/parallel output
CH5: Voltage sense serial/parallel output external
CH6: Current sense serial/parallel output
CH7: NC

POWER SUPPLY SLAVE CONTROLLER ONLY:

CH4: NC
CH5: NC
CH6: NC
CH7: NC

 */ 

#define RATIO_CH0 11.62364434666667
#define RATIO_CH1 11.62364434666667
#define RATIO_CH2 11.62364434666667
#define RATIO_CH3 14.7929

///*
//MASTER CONTROLLER
#define RATIO_CH4 23
#define RATIO_CH5 23
#define RATIO_CH6 1
#define RATIO_CH7 0
//*/

/*
//SLAVE CONTROLLER
#define RATIO_CH4 0
#define RATIO_CH5 0
#define RATIO_CH6 0
#define RATIO_CH7 0
*/

struct Config_Word_Struct CWSTRUCT = { 1, 1, 1, 1, 1, 1, 1, 0}; //softspan-configuration 0=disabled; 1=0 to 5.12V; 3=-5.12V to 5.12V
const float ratio[8] = {RATIO_CH0, RATIO_CH1, RATIO_CH2, RATIO_CH3, RATIO_CH4, RATIO_CH5, RATIO_CH6, RATIO_CH7};
float voltage[8];

uint8_t adc_voltage_port = 0;
uint8_t adc_current_port = 3;
bool external_measure = 0;

//POWERSUPPLY VARIABLES & CONSTANTS----------------------------------------
#define I2C_MESSAGE_DELAY    50 // minimum delay time between i2c requests

#define OUTPUT_DISABLE_BIT 3
#define FAN_HI_BIT 5

int slaveAddress;
int commandByte;
int valueByte;
int checksumm;
int readByte1;
int readByte2;
byte bVal;

int majorVersion;
int minorVersion; 
int controlStatusRegister;
int analogSensor[18];

bool supply_output = true;
bool fan_high = false;

//Program variables--------------------------------------

uint8_t operating_mode = 0; //0=none, indpendent = 1, serial = 2, parallel = 3
bool update_state = 1;
bool output_state = 1;
bool debug_info = 0;
bool parameters_info = 0;
