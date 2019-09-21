//---Global variables---

float dcc_voltage = 4.2;//discharge voltage
float max_temp = 80;//maximum temperature
#define TEMP_COEF 1 //Temperature coeficient of NTC
bool DCC[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};   

uint16_t cell_codes[1][12]; //cell voltages in v*1e4
uint16_t aux_codes[1][6]; //auxillary voltages in v*1e4
uint8_t tx_cfg[1][6];//write register monitor
uint8_t rx_cfg[1][8];//read register monitor
