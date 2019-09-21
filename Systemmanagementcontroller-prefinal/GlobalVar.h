//---Global variables---

//GENERAL STRINGS
char buffer[15][22];//rows / columns
const char version[] = "0.4(23.03.2016)";
//GLOBAL DEFINITIONS

//OPERATING MODES
#define CHARGE_INDEP    1
#define CHARGE_SERIAL   2
#define CHARGE_PARALLEL 3
#define SUPPLY_INDEP    4
#define SUPPLY_SERIAL   5
#define SUPPLY_PARALLEL 6

//GLOBAL VARIABLES
uint8_t operating_mode = 0;//independent_charge=1/serial_charge=2/parallel_charge=3/independent_supply=4/serial_supply=5/parallel_supply=6
bool common_mode = 0;//=1 when serial or parallel mode is active
uint8_t charging_mode[2] = {0,0}; //charging = 1/discharging = 2/ cyclemode=3
uint8_t current_charging_mode;//for active display
int charging_cycles[2] = {1,1};//cycles
int current_charging_cycles = 0;//for active display
bool encoder_mode = 0; //button press=0/instant=1
bool scroll_mode = 0; //constrain at end/ go back to start at end
bool state = 0;//working variable for setting state (global or else arduino crashes)
bool system_halt = 0;//1 when emergency stop initiated
bool output_1 = 0;
bool output_2 = 0;

//SD VARIABLES

bool SD_available = 0;
bool SD_open = 0;
int profile_entries = 0;

typedef struct{
  char name[21];
  uint8_t name_length;
  uint8_t number;
  bool balancing;
  int cells;
  float charge_voltage;
  float discharge_voltage;
  float charge_current;
  bool high_discharge_current;
}Profile;

typedef struct{
  String name;
  String value;
}Config;

Profile buffer_profile;

Profile active_profile_1;
Profile active_profile_2;

uint8_t ui_buffer;// buffer variable

//POWERSUPPLY DATA
typedef struct{
  float voltage;
  float current;
  float power;
}param;

param supply_set_1 = {1.23,3.45,7.89};
param supply_set_2 = {7.89, 3.45, 1.23};

param supply_act_1 = {9.87,6.54,3.21};
param supply_act_2 = {7.89, 3.45, 1.23};

param supply_wbuffer = supply_set_1;
param supply_rbuffer = supply_act_1;//measured data buffer

//BUZZER STRUCT
struct{
  bool state = 0;
  int count = 2;        //Beep on startup
  int duration = 100;
  unsigned long delay=0;
}buzzer;
