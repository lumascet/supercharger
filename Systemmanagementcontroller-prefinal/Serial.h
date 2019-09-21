//---Serial configuration---

//Serial Settings
#define BAUD 250000
#define SERIAL_CONF SERIAL_8E1 //Serial startbit:1 databits:8bit parity:EVEN stopbit:1

//Selftest
bool serialTestState[4]= {0,0,0,0};

byte serial_buffer[4][4];//serial buffer for databytes
int counter[4][2]; //counter variable for timouts

bool ackState[4]= {0,0,0,0}; //0=ready 1=busy
byte ackReg[4]; //acknowledge register, needed for resending
byte ackData[4][4];//acknowledge data, needed for resending
byte ackToutCnt[4]={0,0,0,0}; //number of retrys; stopping system on 4

//Acknowledgement
#define TIMEOUT 100 //timeout ack in ms

//Register Definitions (incomplete)
#define BLANK    0
#define ACKNOWLEDGE    1
#define REQUEST    2
#define SELFTEST_RESPONSE    3
#define RESPONSE    4
#define CHARGER_INDEP    6
#define CHARGER_SERIAL    7
#define CHARGER_PARALLEL    8
#define SUPPLY_INDEP    9
#define SUPPLY_SERIAL    10
#define SUPPLY_PARALLEL    11
#define SET_VOLTAGE    21
#define SET_CURRENT    22
#define ACT_VOLTAGE    23
#define ACT_CURRENT    24
#define PWSP_PWR_ON    25
#define PWSP_FAN_HIGH    26
#define PWSP_TEMP_IN    27
#define PWSP_TEMP_OUT    28
#define DCC    29
#define CELL_VOLT_1    30
#define CELL_VOLT_2    31
#define CELL_VOLT_3    32
#define CELL_VOLT_4    33
#define CELL_VOLT_5    34
#define CELL_VOLT_6    35
#define CELL_VOLT_7    36
#define CELL_VOLT_8    37
#define CELL_VOLT_9    38
#define CELL_VOLT_10    39
#define CELL_VOLT_11    40
#define CELL_VOLT_12    41
#define TEMP_1    42
#define TEMP_2    43

