//---Hardware allocation---

//ALLOCATE DISPLAY PINS
U8GLIB_ST7920_128X64_4X u8g(11, 12, 13);  // SPI Com: SCK = E = 11, MOSI = rw = 12, CS = RS = 13
LiquidCrystal_I2C lcd1(38, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // Addr, En, Rw, Rs, d4, d5, d6, d7, backlighpin, polarity
LiquidCrystal_I2C lcd2(39, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // Addr, En, Rw, Rs, d4, d5, d6, d7, backlighpin, polarity

//SET UP ENCODER
ClickEncoder *encoder1;
ClickEncoder *encoder2;

//ENCODER DIRECTION KEYCODE
#define ENCODER_1_UP 6
#define ENCODER_1_DN 7
#define ENCODER_2_UP 9
#define ENCODER_2_DN 10

//NO KEY
#define KEY_NONE  0
#define BUZZER    A0

//ENCODER BUTTON

//MIDDLE PANEL

#define PM_LED_LT   A2
#define PM_LED_RT   A3
#define PM_LED_UP   A4
#define PM_LED_DN   A5
#define PM_LED_LSET A6
#define PM_LED_RSET A7
#define PM_LED_IDP  28
#define PM_LED_SER  31
#define PM_LED_PAR  29

#define PM_BTN_LSET 22
#define PM_BTN_RSET 23
#define PM_BTN_UP   24
#define PM_BTN_DN   25
#define PM_BTN_LT   26
#define PM_BTN_RT   27

//PANEL 1

#define P1_BTN_SW   5
#define P1_BTN_DT   6
#define P1_BTN_CLK  7

#define P1_BTN_ON   38
#define P1_BTN_OFF  39
#define P1_BTN_RES  44
#define P1_BTN_LT   45
#define P1_BTN_OK   46
#define P1_BTN_RT   47

#define P1_LED_RES  A12
#define P1_LED_ON   A13
#define P1_LED_OFF  A14
#define P1_LED_OK   A15

//PANEL 2

#define P2_BTN_SW   8
#define P2_BTN_DT   9
#define P2_BTN_CLK  10

#define P2_BTN_ON   40
#define P2_BTN_OFF  41
#define P2_BTN_RES  42
#define P2_BTN_LT   43
#define P2_BTN_OK   48
#define P2_BTN_RT   49


#define P2_LED_RES  A8
#define P2_LED_ON   A9
#define P2_LED_OFF  A10
#define P2_LED_OK   A11

//RELAYS

#define REL_1_M1    32
#define REL_1_M2    33
#define REL_2_M3    34
#define REL_2_M4    35
#define REL_3_M5    36
#define REL_3_B     37
