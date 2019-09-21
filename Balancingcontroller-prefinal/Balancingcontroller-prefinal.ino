//---Main Routine---
bool debug_log = 1;
bool debug_print = 1;

//DEBUG SHORTCUTS
#define D(x) if(debug_print) Serial.println(x) //DEBUG NORMAL
#define DR(x) if(debug_print) Serial.print(x) //DEBUG no return
#define DP(x, y) if(debug_print) Serial.print(x, y) //DEBUG precision
#define DE(x) if(debug_print) Serial.println("[A]" + (String)x + "[E]") //DEBUG EXTENDED

#include "TaskScheduler.h"
#include "LTC68042.h"
#include "GlobalVar.h"
#include "Serial.h"
#include <SPI.h>

Scheduler runner;

void setup() {
  Serial.begin(250000);
  LTC6804_initialize();
}

void loop() {
  runner.execute();
  readCells();
  checkCellVoltage();
  cellDischarge();
  readAux();
  checkTemperature();
  //printCells();
  /*
  DCC[i]=!DCC[i];
  overwrite_discharge = 1;
  if(i==11){
    i=-1;
  }
  i++;
  cellDischarge();
  */
}

void printCells()
{
  for (int i=0; i<12; i++)
  {
    DR(" C");
    DP(i+1,DEC);
    DR(":");
    DP(cell_codes[0][i]*1e-4 ,4);
    DR(",");
  }
  D();
}


/*                                                
                                                //  7       6       5       4       3       2       1       0 
byte config_register[1][6] =       {B11111111,  //GPIO5   GPIO4   GPIO3   GPIO2   GPIO1   REFON   SWTRD   ADCOPT
                                    B00000000,  //VUV[7]  VUV[6]  VUV[5]  VUV[4]  VUV[3]  VUV[2]  VUV[1]  VUV[0]  Uuv = (VUV + 1) • 16 • 100μV --> VUV = Uuv / (100µV * 16) - 1
                                    B11110000,  //VOV[3]  VOV[2]  VOV[1]  VOV[0]  VUV[11] VUV[10] VUV[9]  VUV[8]
                                    B11111111,  //VOV[11] VOV[10] VOV[9]  VOV[8]  VOV[7]  VOV[6]  VOV[5]  VOV[4]  Uov = VOV • 16 • 100μV --> VOV = Uov / (100µV * 16)
                                    B00000000,  //DCC8    DCC7    DCC6    DCC5    DCC4    DCC3    DCC2    DCC1    DischargeCellx
                                    B00000000}; //DCTO[3] DCTO[2] DCTO[1] DCTO[0] DCC12   DCC11   DCC10   DCC9

byte cell_voltage_register_A[6] =  {B00000000,  //C1V[7]  C1V[6]  C1V[5]  C1V[4]  C1V[3]  C1V[2]  C1V[1]  C1V[0]
                                    B00000000,  //C1V[15] C1V[14] C1V[13] C1V[12] C1V[11] C1V[10] C1V[9]  C1V[8]
                                    B00000000,  //C2V[7]  C2V[6]  C2V[5]  C2V[4]  C2V[3]  C2V[2]  C2V[1]  C2V[0]
                                    B00000000,  //C2V[15] C2V[14] C2V[13] C2V[12] C2V[11] C2V[10] C2V[9]  C2V[8]
                                    B00000000,  //C3V[7]  C3V[6]  C3V[5]  C3V[4]  C3V[3]  C3V[2]  C3V[1]  C3V[0]
                                    B00000000}; //C3V[15] C3V[14] C3V[13] C3V[12] C3V[11] C3V[10] C3V[9]  C3V[8]

byte cell_voltage_register_B[6] =  {B00000000,  //C4V[7]  C4V[6]  C4V[5]  C4V[4]  C4V[3]  C4V[2]  C4V[1]  C4V[0]
                                    B00000000,  //C4V[15] C4V[14] C4V[13] C4V[12] C4V[11] C4V[10] C4V[9]  C4V[8]
                                    B00000000,  //C5V[7]  C5V[6]  C5V[5]  C5V[4]  C5V[3]  C5V[2]  C5V[1]  C5V[0]
                                    B00000000,  //C5V[15] C5V[14] C5V[13] C5V[12] C5V[11] C5V[10] C5V[9]  C5V[8]
                                    B00000000,  //C6V[7]  C6V[6]  C6V[5]  C6V[4]  C6V[3]  C6V[2]  C6V[1]  C6V[0]
                                    B00000000}; //C6V[15] C6V[14] C6V[13] C6V[12] C6V[11] C6V[10] C6V[9]  C6V[8]

byte cell_voltage_register_C[6] =  {B00000000,  //C7V[7]  C7V[6]  C7V[5]  C7V[4]  C7V[3]  C7V[2]  C7V[1]  C7V[0]
                                    B00000000,  //C7V[15] C7V[14] C7V[13] C7V[12] C7V[11] C7V[10] C7V[9]  C7V[8]
                                    B00000000,  //C8V[7]  C8V[6]  C8V[5]  C8V[4]  C8V[3]  C8V[2]  C8V[1]  C8V[0]
                                    B00000000,  //C8V[15] C8V[14] C8V[13] C8V[12] C8V[11] C8V[10] C8V[9]  C8V[8]
                                    B00000000,  //C9V[7]  C9V[6]  C9V[5]  C9V[4]  C9V[3]  C9V[2]  C9V[1]  C9V[0]
                                    B00000000}; //C9V[15] C9V[14] C9V[13] C9V[12] C9V[11] C9V[10] C9V[9]  C9V[8]

byte cell_voltage_register_D[6] =  {B00000000,  //C10V[7] C10V[6] C10V[5] C10V[4] C10V[3] C10V[2] C10V[1] C10V[0]
                                    B00000000,  //C10V[15]C10V[14]C10V[13]C10V[12]C10V[11]C10V[10]C10V[9] C10V[8]
                                    B00000000,  //C11V[7] C11V[6] C11V[5] C11V[4] C11V[3] C11V[2] C11V[1] C11V[0]
                                    B00000000,  //C11V[15]C11V[14]C11V[13]C11V[12]C11V[11]C11V[10]C11V[9] C11V[8]
                                    B00000000,  //C12V[7] C12V[6] C12V[5] C12V[4] C12V[3] C12V[2] C12V[1] C12V[0]
                                    B00000000}; //C12V[15]C12V[14]C12V[13]C12V[12]C12V[11]C12V[10]C12V[9] C12V[8]

byte axilliary_register_A[6] =     {B00000000,  //G1V[7]  G1V[6]  G1V[5]  G1V[4]  G1V[3]  G1V[2]  G1V[1]  G1V[0]
                                    B00000000,  //G1V[15] G1V[14] G1V[13] G1V[12] G1V[11] G1V[10] G1V[9]  G1V[8]
                                    B00000000,  //G2V[7]  G2V[6]  G2V[5]  G2V[4]  G2V[3]  G2V[2]  G2V[1]  G2V[0]
                                    B00000000,  //G2V[15] G2V[14] G2V[13] G2V[12] G2V[11] G2V[10] G2V[9]  G2V[8]
                                    B00000000,  //G3V[7]  G3V[6]  G3V[5]  G3V[4]  G3V[3]  G3V[2]  G3V[1]  G3V[0]
                                    B00000000}; //G3V[15] G3V[14] G3V[13] G3V[12] G3V[11] G3V[10] G3V[9]  G3V[8]

byte axilliary_register_B[6] =     {B00000000,  //G4V[7]  G4V[6]  G4V[5]  G4V[4]  G4V[3]  G4V[2]  G4V[1]  G4V[0]
                                    B00000000,  //G4V[15] G4V[14] G4V[13] G4V[12] G4V[11] G4V[10] G4V[9]  G4V[8]
                                    B00000000,  //G5V[7]  G5V[6]  G5V[5]  G5V[4]  G5V[3]  G5V[2]  G5V[1]  G5V[0]
                                    B00000000,  //G5V[15] G5V[14] G5V[13] G5V[12] G5V[11] G5V[10] G5V[9]  G5V[8]
                                    B00000000,  //REF[7]  REF[6]  REF[5]  REF[4]  REF[3]  REF[2]  REF[1]  REF[0]
                                    B00000000}; //REF[15] REF[14] REF[13] REF[12] REF[11] REF[10] REF[9]  REF[8]

byte status_register_A[6] =        {B00000000,  //SOC[7]  SOC[6]  SOC[5]  SOC[4]  SOC[3]  SOC[2]  SOC[1]  SOC[0]
                                    B00000000,  //SOC[15] SOC[14] SOC[13] SOC[12] SOC[11] SOC[10] SOC[9]  SOC[8]
                                    B00000000,  //ITMP[7] ITMP[6] ITMP[5] ITMP[4] ITMP[3] ITMP[2] ITMP[1] ITMP[0]
                                    B00000000,  //ITMP[15]ITMP[14]ITMP[13]ITMP[12]ITMP[11]ITMP[10]ITMP[9] ITMP[8]
                                    B00000000,  //VA[7]   VA[6]   VA[5]   VA[4]   VA[3]   VA[2]   VA[1]   VA[0]
                                    B00000000}; //VA[15]  VA[14]  VA[13]  VA[12]  VA[11]  VA[10]  VA[9]   VA[8]


byte status_register_B[6] =        {B00000000,  //VD[7]   VD[6]   VD[5]   VD[4]   VD[3]   VD[2]   VD[1]   VD[0]
                                    B00000000,  //VD[15]  VD[14]  VD[13]  VD[12]  VD[11]  VD[10]  VD[9]   VD[8]
                                    B00000000,  //C4OV    C4UV    C3OV    C3UV    C2OV    C2UV    C1OV    C1UV
                                    B00000000,  //C8OV    C8UV    C7OV    C7UV    C6OV    C6UV    C5OV    C5UV
                                    B00000000,  //C12OV   C12UV   C11OV   C11UV   C10OV   C10UV   C9OV    C9UV
                                    B00000000}; //REV[3]  REV[2]  REV[1]  REV[0]  RSVD    RSVD    MUXFAIL THSD


byte comm_register[6] =            {B00000000,  //ICOM0[3]ICOM0[2]ICOM0[1]ICOM0[0]D0[7]   D0[6]   D0[5]   D0[4]
                                    B00000000,  //D0[3]   D0[2]   D0[1]   D0[0]   FCOM0[3]FCOM0[2]FCOM0[1]FCOM0[0]
                                    B00000000,  //ICOM1[3]ICOM1[2]ICOM1[1]ICOM1[0]D1[7]   D1[6]   D1[5]   D1[4]
                                    B00000000,  //D1[3]   D1[2]   D1[1]   D1[0]   FCOM1[3]FCOM1[2]FCOM1[1]FCOM1[0]
                                    B00000000,  //ICOM2[3]ICOM2[2]ICOM2[1]ICOM2[0]D2[7]   D2[6]   D2[5]   D2[4]
                                    B00000000}; //D2[3]   D2[2]   D2[1]   D2[0]   FCOM2[3]FCOM2[2]FCOM2[1]FCOM2[0]
*/
