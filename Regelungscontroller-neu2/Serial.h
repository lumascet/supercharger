//---Serial configuration---

//Serial Settings
#define BAUD 250000
#define SERIAL_CONF SERIAL_8E1 //Serial startbit:1 databits:8bit parity:EVEN stopbit:1

//Selftest
bool serialTestState[1]= {0};

byte serial_buffer[1][4];//serial buffer for databytes
int counter[1][2]; //counter variable for timouts

bool ackState[1]= {0}; //0=ready 1=busy
byte ackReg[1]; //acknowledge register, needed for resending
byte ackData[1][4];//acknowledge data, needed for resending
byte ackToutCnt[1]={0}; //number of retrys; stopping system on 4

//Acknowledgement
#define TIMEOUT 100 //timeout ack in ms

//Register Definitions (incomplete)
#define ACKNOWLEDGE         1
#define REQUEST             2 //startup test
#define RESPONSE            3 //startup test
#define SET_VOLTAGE_MPC     5
#define SET_CURRENT_MPC     6
#define SET_VOLTAGE_SPC     7
#define SET_CURRENT_SPC     8
#define MES_VOLTAGE_MPC     9
#define MES_CURRENT_MPC     10
#define MES_VOLTAGE_SPC     11
#define MES_CURRENT_SPC     12
#define PWSP_PWR_ON_MPC     13
#define PWSP_PWR_ON_SPC     14
#define PWSP_FAN_HIGH_MPC   15
#define PWSP_FAN_HIGH_SPC   16
#define PWSP_TEMP_IN_MPC    17
#define PWSP_TEMP_OUT_MPC   18
#define PWSP_TEMP_IN_SPC    19
#define PWSP_TEMP_OUT_SPC   20
