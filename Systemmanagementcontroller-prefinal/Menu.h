//---Variables & constants for menu---

//GLOBAL MENU VARIABLES
int8_t menu_dotm = 1;
int8_t submenu_dotm = 0;

int8_t menu_lcd1 = 0;
int8_t submenu_lcd1 = 20;

int8_t menu_lcd2 = 0;
int8_t submenu_lcd2 = 20;

uint8_t last_key_code = KEY_NONE;
unsigned long last_millis;
bool redraw_dotm = 1;
bool redraw_lcd1 = 1;
bool redraw_lcd2 = 1;
uint8_t entries;

uint8_t cursorRow1;
uint8_t cursorColumn1;

uint8_t cursorRow2;
uint8_t cursorColumn2;

uint8_t uiKeyCodeFirst = KEY_NONE;
uint8_t uiKeyCodeSecond = KEY_NONE;
uint8_t uiKeyCode = KEY_NONE;

//SUBPROGRAM NUMBERS
#define MAIN          0
#define CHARGER       1
#define SUPPLY        2
#define SETTINGS      3
#define INFO          4
#define CHARGER_HINT  5
#define SUPPLY_HINT   6
#define ENCODER       7
#define SCROLL        8
#define PROJECT       10
#define VERSION       11
#define UPTIME        12
#define CREDITS       13
#define GRAPHSELECT   14
#define ABORT         19
#define DOTTITLE      20
#define PROFILE       21
#define PROCED        22
#define BALANCING     23
#define VOLTAGE       24
#define CURRENT       25
#define SUMMARY       26
#define CYCLES        27
#define CHARGER_INTERFACE 28
#define XXX  29
#define SUPPLY_INTERFACE 30
#define MENU_SELECT   254//no change of submenu, but btn light will turn on; purpose: for custom and or multiple menu changes
#define MENU_NONE     255//no change of submenu, btn light will turn off; purpose: key binding for no result

//SPECIAL CHARACTERS U8GLIB FONT: üöäÜÖÄ
#define UML_UE "\xfc"
#define UML_OE "\xf6"
#define UML_AE "\xe4"
#define CAP_UML_UE "\xdc"
#define CAP_UML_OE "\xd6"
#define CAP_UML_AE "\xc4"

//SPECIAL CHARACTERS LINE DISPLAY FONT: -> <- üöä
#define ARR_LT "\x7f"
#define ARR_RT "\x7e"
#define LINE_UML_UE "\xf5"
#define LINE_UML_OE "\xef"
#define LINE_UML_AE "\xe1"


//GENERAL STRINGS
const char title[] = "____SUPERCHARGER____";
const char menu_back[] = "<";
const char menu_selected[] = "[X]";
const char menu_unselected[] = "[ ]";
const char* const menu_checkbox[] = {menu_unselected, menu_selected};
const char blank[] = "                    ";
const char empty_profile[] = "Leeres Profil";
const char no_SD_card[] = "Keine SD-Karte";
const char SD_check[] = "SD Karte...";
const char ok[] = "OK";
const char fail[] = "FEHLER";
const char str_yes[] = "JA";
const char str_no[] = "NEIN";
const char str_on[] = "EIN";
const char str_off[] = "AUS";

//SPECIFIC STRINGS

const String file_settings = "SETTINGS.TXT";
const String file_profiles = "PROFILES.TXT";
const String file_description = "DELETE_FILES_TO_GENERATE_DEFAULT_CONFIG";

const char str_dislow[] = "1.00";
const char str_dishig[] = "2.00";

//----------------------------------STRINGS-DOTM-------------------------------
//Definition of the Strings as char-Arrays, the keyword PROGMEM forces to save the variables in program storage

const char string_0_0[] PROGMEM = "Betriebsmodus";
const char string_0_1[] PROGMEM = "Batterieladeger" UML_AE "t   >"; //SBM1
const char string_0_2[] PROGMEM = "Netzger" UML_AE "t           >"; //SBM2
const char string_0_3[] PROGMEM = "Einstellungen       >";          //SBM3
const char string_0_4[] PROGMEM = "Info                >";          //SBM4

const char string_1_0[] PROGMEM = "Lademodus";
const char string_1_1[] PROGMEM = "2 x 12 Zellen @ 60A >";          //SBM5
const char string_1_2[] PROGMEM = "1 x 24 Zellen @ 60A >";          //SBM5
const char string_1_3[] PROGMEM = "1 x 12 Zellen @120A  ";          //SBM5

const char string_2_0[] PROGMEM = "Netzger" UML_AE "tmodus";
const char string_2_1[] PROGMEM = "2 x 50 V @ 60A      >";          //SBM6
const char string_2_2[] PROGMEM = "1 x 100V @ 60A      >";          //SBM6
const char string_2_3[] PROGMEM = "1 x 50 V @120A       ";          //SBM6

const char string_3_0[] PROGMEM = "Einstellungen";
const char string_3_1[] PROGMEM = "Encoder Wert"UML_UE"bergabe>";   //SBM7
const char string_3_2[] PROGMEM = "Scrollmodus         >";          //SBM8
const char string_3_3[] PROGMEM = "-                   >";          //SBM9

const char string_4_0[] PROGMEM = "Info";
const char string_4_1[] PROGMEM = "Projekt             >";          //SBM10
const char string_4_2[] PROGMEM = "Programmversion     >";          //SBM11
const char string_4_3[] PROGMEM = "Systemlaufzeit      >";          //SBM12
const char string_4_4[] PROGMEM = "Credits             >";          //SBM13

const char string_5_0[] PROGMEM = "Hinweis";
const char string_5_1[] PROGMEM = "Eingabe erfolgt";
const char string_5_2[] PROGMEM = "nun auf den";
const char string_5_3[] PROGMEM = "Zeilendisplays";

const char string_6_0[] PROGMEM = "Hinweis";
const char string_6_1[] PROGMEM = "Werte links: Sollwert";
const char string_6_2[] PROGMEM = "Werte rechts: Istwert";
const char string_6_3[] PROGMEM = "Output mittels ON/OFF";

const char string_7_0[] PROGMEM = "Encoder Output";
const char string_7_1[] PROGMEM = "Sofort "CAP_UML_UE"bertragen    ";
const char string_7_2[] PROGMEM = "Mit Best"UML_AE"tigung      ";

const char string_8_0[] PROGMEM = "Scrollmodus";
const char string_8_1[] PROGMEM = "Mit Anschlag         ";
const char string_8_2[] PROGMEM = "Mit "CAP_UML_UE"berlauf         ";

const char string_10_0[] PROGMEM = "Unser Projekt";
const char string_10_1[] PROGMEM = "\x22Supercharger\x22";
const char string_10_2[] PROGMEM = "JGG: 5BHET 2015/16";
const char string_10_3[] PROGMEM = "Laden von mehrzell.";
const char string_10_4[] PROGMEM = "Akkus mit einer";
const char string_10_5[] PROGMEM = "Leistung bis 6kW";

const char string_11_0[] PROGMEM = "Version";
const char string_11_1[] PROGMEM = "";

const char string_12_0[] PROGMEM = "Laufzeit";
const char string_12_1[] PROGMEM = "";

const char string_13_0[] PROGMEM = "Credits";
const char string_13_1[] PROGMEM = "Prof. PFEILER";
const char string_13_2[] PROGMEM = "Florian BARESCH";
const char string_13_3[] PROGMEM = "Matthias EFFENBERGER";
const char string_13_4[] PROGMEM = "Daniel SCHNIZER";
const char string_13_5[] PROGMEM = "Lukas SCHR"CAP_UML_OE"ER";

const char string_14_0[] PROGMEM = "Monitor";
const char string_14_1[] PROGMEM = "Zellspannungen      >";
const char string_14_2[] PROGMEM = "Temperaturen        >";
const char string_14_3[] PROGMEM = "Kapazit"UML_AE"ten         >";
const char string_14_4[] PROGMEM = "Stromverlauf        >";
const char string_14_5[] PROGMEM = "Spannungsverlauf    >";
const char string_14_6[] PROGMEM = "Kapazit"UML_AE"tsverlauf   >";

const char string_19_0[] PROGMEM = "Warnung!";
const char string_19_1[] PROGMEM = "Sie sind im Begriff";
const char string_19_2[] PROGMEM = "den derzeitigen";
const char string_19_3[] PROGMEM = "Vorgang abzubrechen!";
const char string_19_4[] PROGMEM = "Vorgang fortsetzen  >";

//--------------------------------------STRINGS-LINEDISPLAY-------------------------------------------------------

const char string_20_0[] PROGMEM = " Grundeinstellungen ";
const char string_20_1[] PROGMEM = "  am Grafikdisplay  ";
const char string_20_2[] PROGMEM = "       treffen      ";

const char string_21_0[] PROGMEM = " [     Profile    ]";
const char string_21_1[] PROGMEM = "Manuell             ";

const char string_22_0[] PROGMEM = ARR_LT"[                ]";
const char string_22_1[] PROGMEM = "Laden              "ARR_RT;
const char string_22_2[] PROGMEM = "Entladen           "ARR_RT;
const char string_22_3[] PROGMEM = "Zyklusbetrieb      "ARR_RT;

const char string_23_0[] PROGMEM = ARR_LT"[   Balancing    ]"ARR_RT;
const char string_23_1[] PROGMEM = "Aktiviert        [ ]";
const char string_23_2[] PROGMEM = "Zellanzahl          ";
const char string_23_3[] PROGMEM = "                    ";

const char string_24_0[] PROGMEM = ARR_LT"[   Spannungen   ]"ARR_RT;
const char string_24_1[] PROGMEM = "Ladeschluss        V";
const char string_24_2[] PROGMEM = "Entladeschluss     V";
const char string_24_3[] PROGMEM = "                    ";

const char string_25_0[] PROGMEM = ARR_LT"[     Str"LINE_UML_OE"me     ]"ARR_RT;
const char string_25_1[] PROGMEM = "Laden              A";
const char string_25_2[] PROGMEM = "Entladen           A";
const char string_25_3[] PROGMEM = "                    ";

const char string_26_0[] PROGMEM = ARR_LT"[Zusammenfassung ]"ARR_RT;
const char string_26_1[] PROGMEM = "                    ";//Profilename
const char string_26_2[] PROGMEM = "                    ";//Mode: Charge/Discharge/Cycle: amount
const char string_26_3[] PROGMEM = "Balancing:     Z:   ";
const char string_26_4[] PROGMEM = "U: .  V    I:   .  A";

const char string_27_0[] PROGMEM = ARR_LT"[     Zyklen     ]"ARR_RT;
const char string_27_1[] PROGMEM = "Anzahl      X Zyklen";

const char string_28_0[] PROGMEM = " [                ]";//charging discharging cycle: State
const char string_28_1[] PROGMEM = "00:00:00:000        ";//Zeit
const char string_28_2[] PROGMEM = "U:  0.000V I:  0.00A";
const char string_28_3[] PROGMEM = "P:  0.00W           ";
const char string_28_4[] PROGMEM = "C:   0.000Ah        ";

const char string_29_0[] PROGMEM = ARR_LT"[               ]";
const char string_29_1[] PROGMEM = "                    ";
const char string_29_2[] PROGMEM = "                    ";
const char string_29_3[] PROGMEM = "Balancing:     Z:   ";
const char string_29_4[] PROGMEM = "U:0.00V    I:  0.00A";

const char string_30_0[] PROGMEM = "[  Netzger"LINE_UML_AE"t:      ]";//
const char string_30_1[] PROGMEM = "U:       V         V";
const char string_30_2[] PROGMEM = "I:       A         A";
const char string_30_3[] PROGMEM = "P:       W         W";


//Creation of a twodimensional Pointer-Array, for submenu allocation
//for example: string_array[1] -> contains all texts from submenu 1 (string_1_0, string_1_1, string_1_2, string_1_3)
const char* const string_array[][8] PROGMEM = {{string_0_0, string_0_1, string_0_2, string_0_3, string_0_4, NULL},                              //0
                                              {string_1_0, string_1_1, string_1_2, string_1_3, NULL},
                                              {string_2_0, string_2_1, string_2_2, string_2_3, NULL},
                                              {string_3_0, string_3_1, string_3_2, string_3_3, NULL},
                                              {string_4_0, string_4_1, string_4_2, string_4_3, string_4_4, NULL},
                                              {string_5_0, string_5_1, string_5_2, string_5_3, NULL},                                           //5
                                              {string_6_0, string_6_1, string_6_2, string_6_3, NULL},
                                              {string_7_0, string_7_1, string_7_2, NULL},
                                              {string_8_0, string_8_1, string_8_2, NULL},
                                              {NULL},
                                              {string_10_0, string_10_1, string_10_2, string_10_3, string_10_4, string_10_5, NULL},             //10
                                              {string_11_0, string_11_1, NULL},
                                              {string_12_0, string_12_1, NULL},
                                              {string_13_0, string_13_1, string_13_2, string_13_3, string_13_4, string_13_5, NULL},
                                              {string_14_0, string_14_1, string_14_2, string_14_3, string_14_4, string_14_5, string_14_6, NULL},
                                              {NULL},                                                                                           //15
                                              {NULL},
                                              {NULL},
                                              {NULL},
                                              {string_19_0, string_19_1, string_19_2, string_19_3, string_19_4, NULL},
                                              {string_20_0, string_20_1, string_20_2, NULL},                                                    //20
                                              {string_21_0, string_21_1, NULL},
                                              {string_22_0, string_22_1, string_22_2, string_22_3, NULL},
                                              {string_23_0, string_23_1, string_23_2, string_23_3, NULL},
                                              {string_24_0, string_24_1, string_24_2, string_24_3, NULL},
                                              {string_25_0, string_25_1, string_25_2, string_25_3, NULL},
                                              {string_26_0, string_26_1, string_26_2, string_26_3, string_26_4, NULL},
                                              {string_27_0, string_27_1, NULL},
                                              {string_28_0, string_28_1, string_28_2, string_28_3, string_28_4, NULL},
                                              {string_29_0, string_29_1, string_29_2, string_29_3, NULL},
                                              {string_30_0, string_30_1, string_30_2, string_30_3, NULL}};

//MENU CONSTANTS (PROGMEM array has to be inizialized first);
const uint8_t string_array_size = sizeof(string_array)/sizeof(string_array[0]);
uint8_t menu_items[string_array_size];
#define MENU_SPLIT 5

const uint8_t link_next[][11] PROGMEM ={//link table of next submenu
{MENU_NONE,  CHARGER, SUPPLY, SETTINGS, INFO},           //0
{MENU_NONE,  CHARGER_HINT, CHARGER_HINT, MENU_NONE},
{MENU_NONE,  SUPPLY_HINT, SUPPLY_HINT, MENU_NONE},
{MENU_NONE,  ENCODER, SCROLL},
{MENU_NONE,  PROJECT, VERSION, UPTIME, CREDITS},
{MENU_NONE,  MENU_NONE, MENU_NONE, MENU_NONE},           //5
{MENU_NONE,  MENU_NONE, MENU_NONE, MENU_NONE},
{MENU_NONE,  MENU_SELECT, MENU_SELECT},
{MENU_NONE,  MENU_SELECT, MENU_SELECT},
{},
{MENU_NONE,  MENU_NONE, MENU_NONE, MENU_NONE, MENU_NONE, MENU_NONE}, //10
{MENU_NONE,  MENU_NONE},
{MENU_NONE,  MENU_NONE},
{MENU_NONE,  MENU_NONE, MENU_NONE, MENU_NONE, MENU_NONE, MENU_NONE},
{MENU_NONE, MENU_NONE, MENU_NONE, MENU_NONE, MENU_NONE, MENU_NONE, MENU_NONE},
{MENU_NONE},                                             //15
{MENU_NONE},
{MENU_NONE},
{MENU_NONE},
{MENU_NONE, MENU_NONE, MENU_NONE, MENU_NONE, MENU_SELECT},
{MENU_NONE, MENU_NONE, MENU_NONE},                       //20
{MENU_NONE, PROCED},                                     //exception rule PROCED * profiles
{MENU_NONE, BALANCING, BALANCING, CYCLES},
{VOLTAGE, MENU_NONE, MENU_NONE, MENU_NONE},
{CURRENT, MENU_NONE, MENU_NONE, MENU_NONE},
{SUMMARY, MENU_NONE, MENU_NONE, MENU_NONE},                 //25 
{CHARGER_INTERFACE,  MENU_NONE, MENU_NONE, MENU_NONE},
{BALANCING,  MENU_NONE, MENU_NONE, MENU_NONE},
{MENU_NONE,  MENU_NONE, MENU_NONE, MENU_NONE},
{MENU_NONE,  MENU_NONE, MENU_NONE, MENU_NONE},
{MENU_NONE, MENU_NONE, MENU_NONE, MENU_NONE}};          //30         

const uint8_t link_back[][1] PROGMEM = {{MENU_NONE},                             //link table of previous submenu
                                        {MAIN},
                                        {MAIN},
                                        {MAIN},
                                        {MAIN},
                                        {CHARGER},   //5
                                        {SUPPLY},
                                        {SETTINGS},
                                        {SETTINGS},
                                        {MENU_NONE},
                                        {INFO},     //10
                                        {INFO},
                                        {INFO},
                                        {INFO},
                                        {MENU_NONE},
                                        {MENU_NONE},         //15
                                        {MENU_NONE},
                                        {MENU_NONE},
                                        {MENU_NONE},
                                        {MENU_SELECT},
                                        {MENU_NONE}, //20
                                        {MENU_NONE},
                                        {PROFILE},
                                        {PROCED},
                                        {BALANCING},
                                        {VOLTAGE},  //25
                                        {CURRENT},
                                        {PROFILE},
                                        {MENU_NONE},
                                        {MENU_NONE},
                                        {MENU_NONE}};   //30

typedef struct{//struct for variable menu entries
  void* content;//void pointer for data content
  uint8_t type;//datatype for void content recognition
  uint8_t decimals;//if float, number of decimals
  uint8_t row;//x-Position of starting point (in case of a number starting point is ones-digit)
  uint8_t column;//y-Position of starting point
  int lower_border;//fixed lower border
  int* upper_border;//variable upper border pointer
  uint8_t edit; //flag for editable variables only 1 editable entry per row
}menuvar;

uint8_t var_none = 0;

uint8_t current_display = 0; //which display is the buffer refering: 0=graphic_disp, 1=linedisp 1, 2=linedis 2
menuvar current_menuvar[3][8]; // pointer to editable menuvar in order of row

int mv_none_upper_border = 0;

menuvar mv_none ={.content = &var_none,
                  .type = 0,
                  .decimals = 0, 
                  .row = 0,
                  .column = 0,
                  .lower_border = 0,
                  .upper_border = &mv_none_upper_border,
                  .edit = 0};

menuvar* edit_var[3] = {&mv_none, &mv_none, &mv_none};//temp variable pointer, which variable is currently editing

int8_t edit_multipler_1 = 0;
int8_t edit_multipler_2 = 0;
const uint8_t type_int = 1;
const uint8_t type_float = 2;
const uint8_t type_string = 3;
const uint8_t type_bool = 4;


//arrays for editing Values
/*
#define BALANCING     23
#define VOLTAGE       24
#define CURRENT       25
#define SUMMARY       26
#define CYCLES        27
*/
//STANDARDIZED MENU MANIPULATION ENTRIES
const menuvar menuvar_none PROGMEM = {&var_none, 0, 0, 0, 0, 0, &mv_none_upper_border, false};
/*
const menuvar mv_ver PROGMEM = {.content = &version,
                                .type = type_string,
                                .decimals = 0, 
                                .row = 1,
                                .column = 0,
                                .lower_border = 0,
                                .upper_border = &mv_none_upper_border,
                                .edit = 0};
*/


//MENU: VOLTAGES

int mv_v_upper_border = 5;

const menuvar mv_chv PROGMEM = {.content = &buffer_profile.charge_voltage,
                                .type = type_float,
                                .decimals = 2, 
                                .row = 1,
                                .column = 15,
                                .lower_border = 0,
                                .upper_border = &mv_v_upper_border,
                                .edit = true};

const menuvar mv_dcv PROGMEM = {.content = &buffer_profile.discharge_voltage,
                                .type = type_float,
                                .decimals = 2, 
                                .row = 2,
                                .column = 15,
                                .lower_border = 0,
                                .upper_border = &mv_v_upper_border,
                                .edit = true};


//MENU: CURRENT

int mv_c_upper_border = 50;

const menuvar mv_chc PROGMEM = {.content = &buffer_profile.charge_current,
                                .type = type_float,
                                .decimals = 2, 
                                .row = 1,
                                .column = 15,
                                .lower_border = 0,
                                .upper_border = &mv_c_upper_border,
                                .edit = true};

const menuvar mv_dcc PROGMEM = {.content = &buffer_profile.high_discharge_current,
                                .type = type_bool,
                                .decimals = 0, 
                                .row = 2,
                                .column = 15,
                                .lower_border = 0,
                                .upper_border = &mv_none_upper_border,
                                .edit = true};


//MENU: BALANCING

const menuvar mv_bal PROGMEM = {.content = &buffer_profile.balancing,
                                .type = type_bool,
                                .decimals = 0, 
                                .row = 1,
                                .column = 17,
                                .lower_border = 0,
                                .upper_border = &mv_none_upper_border,
                                .edit = true};

int mv_cel_upper_border = 12;

const menuvar mv_cel PROGMEM = {.content = &buffer_profile.cells,
                                .type = type_int,
                                .decimals = 0, 
                                .row = 2,
                                .column = 19,
                                .lower_border = 1,
                                .upper_border = &mv_cel_upper_border,
                                .edit = true};

//MENU: SUPPLY

int mv_sv_supply_upper_border = 60;

const menuvar mv_sv_supply PROGMEM = {.content = &supply_wbuffer.voltage,
                                .type = type_float,
                                .decimals = 3, 
                                .row = 1,
                                .column = 4,
                                .lower_border = 0,
                                .upper_border = &mv_sv_supply_upper_border,
                                .edit = true};

int mv_sc_supply_upper_border = 50;

const menuvar mv_sc_supply PROGMEM = {.content = &supply_wbuffer.current,
                                .type = type_float,
                                .decimals = 3, 
                                .row = 2,
                                .column = 4,
                                .lower_border = 0,
                                .upper_border = &mv_sc_supply_upper_border,
                                .edit = true};

int mv_sp_supply_upper_border = 3000;
/*
const menuvar mv_sp_supply PROGMEM = {.content = &supply_buffer.power,
                                .type = type_float,
                                .decimals = 0, 
                                .row = 3,
                                .column = 5,
                                .lower_border = 0,
                                .upper_border = &mv_sp_supply_upper_border,
                                .edit = 1};
*/
const menuvar mv_v_supply PROGMEM = {.content = &supply_rbuffer.voltage,
                                .type = type_float,
                                .decimals = 3, 
                                .row = 1,
                                .column = 14,
                                .lower_border = 0,
                                .upper_border = &mv_sv_supply_upper_border,
                                .edit = false};

const menuvar mv_c_supply PROGMEM = {.content = &supply_rbuffer.current,
                                .type = type_float,
                                .decimals = 3, 
                                .row = 2,
                                .column = 14,
                                .lower_border = 0,
                                .upper_border = &mv_sc_supply_upper_border,
                                .edit = false};

const menuvar mv_p_supply PROGMEM = {.content = &supply_rbuffer.power,
                                .type = type_float,
                                .decimals = 2, 
                                .row = 3,
                                .column = 15,
                                .lower_border = 0,
                                .upper_border = &mv_sp_supply_upper_border,
                                .edit = false};

//MENU: SUMMARY

const menuvar mv_cel_sum PROGMEM = {.content = &buffer_profile.cells,
                                .type = type_int,
                                .decimals = 0, 
                                .row = 3,
                                .column = 18,
                                .lower_border = 1,
                                .upper_border = &mv_cel_upper_border,
                                .edit = false};

const menuvar mv_bal_sum PROGMEM = {.content = &buffer_profile.balancing,
                                .type = type_bool,
                                .decimals = 0, 
                                .row = 3,
                                .column = 10,
                                .lower_border = 0,
                                .upper_border = &mv_none_upper_border,
                                .edit = false};

//MENU: CYCLES

int mv_cycles_upper_border = 99;

const menuvar mv_cyc PROGMEM = {.content = &current_charging_cycles,
                                .type = type_int,
                                .decimals = 0, 
                                .row = 1,
                                .column = 12,
                                .lower_border = 1,
                                .upper_border = &mv_cycles_upper_border,
                                .edit = true};

//MENU: SUPPLY INTERFACE

const menuvar mv_sup_out PROGMEM = {.content = &output_1,
                                .type = type_bool,
                                .decimals = 0, 
                                .row = 0,
                                .column = 14,
                                .lower_border = 0,
                                .upper_border = &mv_none_upper_border,
                                .edit = false};

const menuvar* const substitute_var[][8] PROGMEM = {//pointer array for menu variables, columns doesn't have to be in a specific order.
  {&menuvar_none},//0
  {&menuvar_none},
  {&menuvar_none},
  {&menuvar_none},
  {&menuvar_none},
  {&menuvar_none},//5
  {&menuvar_none},
  {&menuvar_none},
  {&menuvar_none},
  {&menuvar_none},
  {&menuvar_none},//10
  {&menuvar_none},
  {&menuvar_none},
  {&menuvar_none},
  {&menuvar_none},
  {&menuvar_none},//15
  {&menuvar_none},
  {&menuvar_none},
  {&menuvar_none},
  {&menuvar_none},
  {&menuvar_none},//20
  {&menuvar_none},
  {&menuvar_none},
  {&mv_bal, &mv_cel, &menuvar_none},
  {&mv_chv, &mv_dcv, &menuvar_none},
  {&mv_chc, &mv_dcc, &menuvar_none},//25
  {&mv_cel_sum, &mv_bal_sum, &menuvar_none},
  {&mv_cyc, &menuvar_none},
  {&menuvar_none},
  {&menuvar_none},
  {&mv_sup_out, &mv_sv_supply, &mv_sc_supply, &mv_v_supply, &mv_c_supply, &mv_p_supply, &menuvar_none}//30
  };

#define title_xbm_width 128
#define title_xbm_height 18
static unsigned char title_xbm[] U8G_PROGMEM = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xf8, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc,
   0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x1f, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xfe, 0x3b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xfc, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x3b, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x1c, 0xc0, 0xe1, 0xfc, 0xe3, 0x3f, 0x7f, 0x07,
   0xf8, 0x8f, 0xff, 0xfc, 0xf9, 0xcf, 0x3f, 0xf6, 0x1c, 0xc0, 0xe0, 0xfe,
   0xe7, 0x3f, 0x7f, 0x07, 0xf8, 0x9f, 0xff, 0xfc, 0xfd, 0xef, 0x7f, 0x7e,
   0xfc, 0xe7, 0x60, 0xfe, 0xf7, 0x3f, 0x7f, 0x07, 0xf8, 0x9f, 0xff, 0xfc,
   0xfd, 0xef, 0x7f, 0x7f, 0xfc, 0xef, 0x70, 0x0e, 0x77, 0xb8, 0x03, 0x07,
   0x1c, 0x0c, 0xe0, 0x0c, 0x0c, 0x66, 0x70, 0x07, 0x00, 0xe7, 0x70, 0x06,
   0xf3, 0xbf, 0x83, 0x03, 0x1c, 0xce, 0xef, 0x0e, 0x0e, 0x67, 0x3f, 0x07,
   0x00, 0xe7, 0x70, 0x87, 0xb3, 0x9f, 0x83, 0x03, 0x1c, 0xce, 0xef, 0x0e,
   0x0e, 0x77, 0x3f, 0x07, 0x00, 0x67, 0x70, 0x87, 0x3b, 0x80, 0x83, 0x03,
   0x0c, 0xee, 0x60, 0x0e, 0x0e, 0x77, 0x80, 0x03, 0xfe, 0xf7, 0x3f, 0xff,
   0xfb, 0x8f, 0x81, 0xff, 0x0e, 0xee, 0x7f, 0x06, 0x7e, 0xf7, 0x9f, 0x03,
   0xff, 0xf3, 0x3f, 0xff, 0xfb, 0xcf, 0x81, 0xff, 0x0e, 0xe6, 0x7f, 0x06,
   0xfe, 0xf3, 0x9f, 0x03, 0xff, 0xe3, 0x3f, 0xfb, 0xf1, 0xcf, 0x81, 0xff,
   0x0e, 0xe7, 0x7f, 0x07, 0xfe, 0xf3, 0x9f, 0x03, 0x00, 0x00, 0x80, 0x03,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03, 0x00, 0x00,
   0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00 };
