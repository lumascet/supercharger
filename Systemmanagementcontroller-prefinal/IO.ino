//---Input & Output operations---

void init_Pin() {
  pinMode(BUZZER, OUTPUT);
  
  pinMode(PM_LED_LT, OUTPUT);
  pinMode(PM_LED_RT, OUTPUT);
  pinMode(PM_LED_UP, OUTPUT);
  pinMode(PM_LED_DN, OUTPUT);
  pinMode(PM_LED_LSET, OUTPUT);
  pinMode(PM_LED_RSET, OUTPUT);
  pinMode(PM_LED_IDP, OUTPUT);
  pinMode(PM_LED_SER, OUTPUT);
  pinMode(PM_LED_PAR, OUTPUT);
  pinMode(30, INPUT);//connected with GND, so defined as input

  pinMode(PM_BTN_LSET, INPUT_PULLUP);
  pinMode(PM_BTN_RSET, INPUT_PULLUP);
  pinMode(PM_BTN_UP, INPUT_PULLUP);
  pinMode(PM_BTN_DN, INPUT_PULLUP);
  pinMode(PM_BTN_LT, INPUT_PULLUP);
  pinMode(PM_BTN_RT, INPUT_PULLUP);

  //PANEL 1
  pinMode(P1_LED_RES, OUTPUT);
  pinMode(P1_LED_ON, OUTPUT);
  pinMode(P1_LED_OFF, OUTPUT);
  pinMode(P1_LED_OK, OUTPUT);

  pinMode(P1_BTN_ON, INPUT_PULLUP);
  pinMode(P1_BTN_OFF, INPUT_PULLUP);
  pinMode(P1_BTN_RES, INPUT_PULLUP);
  pinMode(P1_BTN_LT, INPUT_PULLUP);
  pinMode(P1_BTN_OK, INPUT_PULLUP);
  pinMode(P1_BTN_RT, INPUT_PULLUP);

  pinMode(P1_BTN_SW, INPUT_PULLUP);
  /*
  pinMode(P1_BTN_DT, INPUT);
  pinMode(P1_BTN_CLK, INPUT);
  */
  //PANEL 2

  pinMode(P2_LED_RES, OUTPUT);
  pinMode(P2_LED_ON, OUTPUT);
  pinMode(P2_LED_OFF, OUTPUT);
  pinMode(P2_LED_OK, OUTPUT);

  pinMode(P2_BTN_ON, INPUT_PULLUP);
  pinMode(P2_BTN_OFF, INPUT_PULLUP);
  pinMode(P2_BTN_RES, INPUT_PULLUP);
  pinMode(P2_BTN_LT, INPUT_PULLUP);
  pinMode(P2_BTN_OK, INPUT_PULLUP);
  pinMode(P2_BTN_RT, INPUT_PULLUP);

  pinMode(P2_BTN_SW, INPUT_PULLUP);
  /*
  pinMode(P2_BTN_DT, INPUT);
  pinMode(P2_BTN_CLK, INPUT);
  */
  //RELAYS
  pinMode(REL_1_M1, OUTPUT);
  digitalWrite(REL_1_M1, HIGH);//Relay OFF
  pinMode(REL_1_M2, OUTPUT);
  digitalWrite(REL_1_M2, HIGH);//Relay OFF
  pinMode(REL_2_M3, OUTPUT);
  digitalWrite(REL_2_M3, HIGH);//Relay OFF
  pinMode(REL_2_M4, OUTPUT);
  digitalWrite(REL_2_M4, HIGH);//Relay OFF
  pinMode(REL_3_M5, OUTPUT);
  digitalWrite(REL_3_M5, HIGH);//Relay OFF
  pinMode(REL_3_B, OUTPUT);
  digitalWrite(REL_3_B, HIGH);//Relay OFF
}

void updateLight() {  
  if(operating_mode == CHARGE_INDEP || operating_mode == SUPPLY_INDEP){
    digitalWrite(PM_LED_IDP, HIGH);
    digitalWrite(PM_LED_SER, LOW);
    digitalWrite(PM_LED_PAR, LOW);
  }
  else if(operating_mode == CHARGE_SERIAL || operating_mode == SUPPLY_SERIAL){
    digitalWrite(PM_LED_IDP, LOW);
    digitalWrite(PM_LED_SER, HIGH);
    digitalWrite(PM_LED_PAR, LOW);
  }
  else if(operating_mode == CHARGE_PARALLEL || operating_mode == SUPPLY_PARALLEL){
    digitalWrite(PM_LED_IDP, LOW);
    digitalWrite(PM_LED_SER, LOW);
    digitalWrite(PM_LED_PAR, HIGH);
  }
  else{
    digitalWrite(PM_LED_IDP, LOW);
    digitalWrite(PM_LED_SER, LOW);
    digitalWrite(PM_LED_PAR, LOW);
  }
  //LEFT
  if (submenu_dotm != 0 && menu_dotm == 0) {
    digitalWrite(PM_LED_LT, HIGH);
  }
  else {
    digitalWrite(PM_LED_LT, LOW);
  }

  //RIGHT
  ui_buffer = pgm_read_byte(&link_next[submenu_dotm][menu_dotm]);
  if (ui_buffer != MENU_NONE) digitalWrite(PM_LED_RT, HIGH);
  else digitalWrite(PM_LED_RT, LOW);

  //UP DOWN
  if (menu_dotm == 0) {
    digitalWrite(PM_LED_UP, LOW);
    digitalWrite(PM_LED_DN, HIGH);
  }
  else if (menu_dotm == menu_items[submenu_dotm] - 1) {
    digitalWrite(PM_LED_UP, HIGH);
    digitalWrite(PM_LED_DN, LOW);
  }
  else {
    digitalWrite(PM_LED_UP, HIGH);
    digitalWrite(PM_LED_DN, HIGH);
  }
  //P1
  if (submenu_lcd1 > 20) digitalWrite(P1_LED_OK, HIGH);
  else digitalWrite(P1_LED_OK, LOW);

  if (submenu_lcd2 > 20) digitalWrite(P2_LED_OK, HIGH);
  else digitalWrite(P2_LED_OK, LOW);

  if (submenu_lcd1 == SUPPLY_INTERFACE || submenu_lcd1 == CHARGER_INTERFACE){
    if(output_1){
      digitalWrite(P1_LED_ON, LOW);
      digitalWrite(P1_LED_OFF, HIGH);
    }
    else{
      digitalWrite(P1_LED_ON, HIGH);
      digitalWrite(P1_LED_OFF, LOW);
    }      
  }
  else{
    digitalWrite(P1_LED_ON, LOW);
    digitalWrite(P1_LED_OFF, LOW);
  }

  //P2
  if (submenu_lcd2 == SUPPLY_INTERFACE || submenu_lcd2 == CHARGER_INTERFACE){
    if(output_2){
      digitalWrite(P2_LED_ON, LOW);
      digitalWrite(P2_LED_OFF, HIGH);
    }
    else{
      digitalWrite(P2_LED_ON, HIGH);
      digitalWrite(P2_LED_OFF, LOW);
    }      
  }
  else{
    digitalWrite(P2_LED_ON, LOW);
    digitalWrite(P2_LED_OFF, LOW);
  }
   
   
}

void buzzer_task(){
  if(buzzer.count == 0){
    return;
  }
  if(millis() >= buzzer.delay){
    buzzer.state = !buzzer.state;
    digitalWrite(A0, buzzer.state);
    if(buzzer.state == 0) {
      buzzer.count--;
      if(buzzer.count == 0) return;
    }
    buzzer.delay = millis() + buzzer.duration;
  }
}
