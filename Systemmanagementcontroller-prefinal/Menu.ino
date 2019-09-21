//---Menu management---

void countEntries(){
  for (int i; i <= string_array_size; i++) { //Counting Entries of all Submenus
    int j = 0;
    while ((char*)pgm_read_word(&(string_array[i][j])) != NULL) j++;
    switch (i) { //exceptions, don't forget to subtract number in loadMenuEntries
      case SETTINGS:
        j -= 1;
      break;
      case PROFILE:
        j += 10;
      break;
    }
    menu_items[i] = j;
    /*
    Serial.print(i);
    Serial.println("\t" + (String)menu_items[i]);
    */
  }
}

#define FIRST_KEY_DOWN(key) if(digitalRead(key) == LOW) uiKeyCodeFirst = key
#define NEXT_KEY_DOWN(key) else if(digitalRead(key) == LOW) uiKeyCodeFirst = key

void uiStep() {
  uiKeyCodeSecond = uiKeyCodeFirst;
  int encoderCode = 0;
  bool skipDebounce = 0;

  uiKeyCodeFirst = KEY_NONE;
  
  FIRST_KEY_DOWN(PM_BTN_UP);
  NEXT_KEY_DOWN(PM_BTN_DN);
  NEXT_KEY_DOWN(PM_BTN_RT);
  NEXT_KEY_DOWN(PM_BTN_LT);
  NEXT_KEY_DOWN(PM_BTN_LSET);
  NEXT_KEY_DOWN(PM_BTN_RSET);

  if(submenu_lcd1 > 20){ //Poll ports only when Input is possible
    encoderCode = getEncoder1Code();
    if(encoderCode != 0){
      uiKeyCodeFirst = encoderCode;
      //D(encoderCode);
      skipDebounce = 1;
    }
    encoderCode = getEncoder2Code();
    if(encoderCode != 0){
      uiKeyCodeFirst = encoderCode;
      //D(encoderCode);
      skipDebounce = 1;
    }
    FIRST_KEY_DOWN(P1_BTN_RT);
    NEXT_KEY_DOWN(P1_BTN_OK);
    NEXT_KEY_DOWN(P1_BTN_LT);
    NEXT_KEY_DOWN(P1_BTN_RES);
    NEXT_KEY_DOWN(P1_BTN_ON);
    NEXT_KEY_DOWN(P1_BTN_OFF);
    NEXT_KEY_DOWN(P1_BTN_SW);
    
    FIRST_KEY_DOWN(P2_BTN_RT);
    NEXT_KEY_DOWN(P2_BTN_OK);
    NEXT_KEY_DOWN(P2_BTN_LT);
    NEXT_KEY_DOWN(P2_BTN_RES);
    NEXT_KEY_DOWN(P2_BTN_ON);
    NEXT_KEY_DOWN(P2_BTN_OFF);
    NEXT_KEY_DOWN(P2_BTN_SW);
  }
  if (uiKeyCodeSecond == uiKeyCodeFirst || skipDebounce){ //debounce - skip if encoder value
    uiKeyCode = uiKeyCodeFirst;
    if (uiKeyCode == KEY_NONE){
      last_key_code = uiKeyCode;
      D("RTN");
      return;
    }
    if(last_key_code != uiKeyCode){
      last_key_code = uiKeyCode;
      DR("KEYCODE: ");
      D(uiKeyCode);
      updateMenu();
    }
  }
  else
    uiKeyCode = KEY_NONE;
}

void loadMenuEntries(uint8_t submen) { //load Entries from program storage
  entries = menu_items[submen]; //number of menu entries
  Profile profile;
  int real_entries = entries; //if number of entries in PROGMEM has been edited
  uint8_t start_Field; //Starting x-coordinate for text
  static char cinsert[20]; //buffer for inserting letters (has to be static due to strcat() Method)

  switch(submen){//for entry count manipulation
    case PROFILE:
      real_entries -= 10;
    break;
  }

  for (int i = 0; i < real_entries; i++) {//writing current submenu from programspace to buffer
    strcpy_P(buffer[i], (char*)pgm_read_word(&(string_array[submen][i])));
  }
  if(current_display == 1){
    buffer_profile = active_profile_1;
  }
  else if(current_display == 1){
    buffer_profile = active_profile_2;
  }
  
  switch (submen) {//non-standardized text manipulatuion
    case VERSION:
      strcpy(buffer[1], version); //copy version string
    break;
    case UPTIME:
      strcpy(buffer[1], upTime().c_str()); //copy current uptime (scheduler has a task to refresh this string)
    break;
    case PROCED:
      start_Field = (20-constrain(buffer_profile.name_length,0,16)) / 2; //get start position for Center Alignment
      for(int i=0;i<constrain(buffer_profile.name_length,0,16);i++){//inserting result in buffer
        buffer[0][start_Field+i] = buffer_profile.name[i];
      }
    break;
    case PROFILE:
      readAllProfiles(10); //get profile Names from SD card
    break;
    case SUMMARY:
      for(int i=0;i<constrain(buffer_profile.name_length,0,20);i++){//insert active profile name
        buffer[1][i] = buffer_profile.name[i];
      }
      current_charging_mode = charging_mode[current_display-1];
      current_charging_cycles = charging_cycles[current_display-1];
      switch(current_charging_mode){
        case 1:
        strcpy(cinsert, "Laden");
        break;
        case 2:
        strcpy(cinsert, "Entladen");
        break;
        case 3:
        strcpy(cinsert, "Zyklen: ");
        strcat(cinsert, (char*)(current_charging_cycles));
        break;
      }
      for(int i=0;i<strlen(cinsert);i++){ //insert charging mode
        buffer[2][i] = cinsert[i];
      }
      /*
      insertFloatToBuffer(4, 2, &dummy.charge_voltage, 4, 2);
      insertFloatToBuffer(4, 13, &dummy.charge_current, 6, 2);
      */
    break;
  }

  for(int i = 0; i < 8; i++){//clearing buffer
    current_menuvar[current_display][i] = mv_none;
  }

  
  int a = 0;
  menuvar var;
  for (int i = 0; i < sizeof(substitute_var[submen]); i++) { //loop for remembering editable entries
    var = loadMenuVariables(submen, i); //loading editable variables out of PROGMEM table
    if(var.content == &var_none)break; //next cycle
    if(var.edit == 1){
      current_menuvar[current_display][var.row] = var;
      /*
      D("-----------");
      D(i);
      D(*(float*)var.content);
      D("-----------");
      */
    }
    switch (var.type){//standardized text manipulatuion: converting void datatype into specific readable datatype, inserting it into specific place
        case type_int:
          insertIntToBuffer(var.row, var.column, (int*)var.content, getLength(*var.upper_border));
        break;
        case type_float:
          insertFloatToBuffer(var.row, var.column, (float*)var.content , getLength(*var.upper_border), var.decimals);
        break;
        case type_string:
          insertStringToBuffer(var.row, var.column, (String*)var.content);
        break;
        case type_bool:
          insertBoolToBuffer(var.row, var.column, (bool*)var.content, submen);
        break;
    }
  }
  /*
  for (int s=0; s<8; s++){
    D("-----------------");
    D(s);
    D(current_menuvar[current_display][s].edit);
  }
  */
}

menuvar loadMenuVariables(uint8_t submen, uint8_t entry){// read changing menu variable structs out of progmem
  menuvar var = {};
  menuvar* var_ptr = (menuvar*)pgm_read_word(&substitute_var[submen][entry]);
  var.content = (void*)pgm_read_word(&var_ptr->content);
  if(var.content == &var_none) return var;
  var.type = (uint8_t)pgm_read_word(&var_ptr->type);
  var.decimals = (uint8_t)pgm_read_word(&var_ptr->decimals);
  var.row = (uint8_t)pgm_read_word(&var_ptr->row);
  var.column = (uint8_t)pgm_read_word(&var_ptr->column);
  var.lower_border = (int)pgm_read_word(&var_ptr->lower_border);
  var.upper_border = (int*)pgm_read_word(&var_ptr->upper_border);
  var.edit = (uint8_t)pgm_read_word(&var_ptr->edit);
  
  /*
  D("PROGMEM READ");
  D(*(float*)var.content);
  D(var.type);
  D(var.decimals);
  D(var.row);
  D(var.column);
  D(var.lower_border);
  D(*var.upper_border);
  D(var.edit);
  D("PROGMEM READ END");
  //*/
  
  return var;
}
/*
menuvar loadMenuVariablesByRow(uint8_t submen, uint8_t row){// only working for 1variable per line
  menuvar var = {};
  menuvar* var_ptr;
  int i=0;
  do{
    var_ptr = (menuvar*)pgm_read_word(&substitute_var[submen][i]);
    var.row = (uint8_t)pgm_read_word(&var_ptr->row);
    if (i > sizeof(substitute_var[submen])) break;
    i++;
  }while(var.row != row);
  
  var.content = (void*)pgm_read_word(&var_ptr->content);
  var.type = (uint8_t)pgm_read_word(&var_ptr->type);
  var.decimals = (uint8_t)pgm_read_word(&var_ptr->decimals);
  //var.row = (uint8_t)pgm_read_word(&var_ptr->row);
  var.column = (uint8_t)pgm_read_word(&var_ptr->column);
  var.lower_border = (int)pgm_read_word(&var_ptr->lower_border);
  var.upper_border = (int*)pgm_read_word(&var_ptr->upper_border);
  var.edit = (int)pgm_read_word(&var_ptr->edit);
  
  D(*(float*)var.content);
  D(var.type);
  D(var.decimals);
  D(var.row);
  D(var.column);
  D(var.lower_border);
  D(var.upper_border);
  
  return var;
}
*/
/*
uint8_t countRowMenuVariables(uint8_t submen, uint8_t menu){// counts entries in one row, if there are more than 1 variables in a single row
  menuvar var = {};
  uint8_t count = 0;

  for(int i=0; i < sizeof(substitute_var[submen]); i++){
    menuvar* var_ptr = (menuvar*)pgm_read_word(&substitute_var[submen][i]);
    var.row = (uint8_t)pgm_read_word(&var_ptr->row);
    if(var.row == menu) count++;
  }
  return count;
}
*/

void insertFloatToBuffer(uint8_t row, uint8_t column, float* value, uint8_t max_length, uint8_t decimals){//insert variable into buffer
  static char cinsert[7]; //buffer for char array of the variable
  uint8_t length = getLength(*value);
  uint8_t act_length = length + decimals + 1;
  uint8_t hidden_zero = max_length - length;
  uint8_t zero_start = column - max_length + 1;
  uint8_t one_start = column - (length - 1);
  dtostrf(*value, act_length, decimals, cinsert);//converting variable into char array
  /*
  D("0");
  String s = floatToString(*value, 2);
  D("1");
  DE(s);
  s.toCharArray(cinsert, max_length);
  D("2");
  DE(cinsert);
  */
  //show all possible digits when editing number:
  if(edit_var[current_display]->content == value){//compare addresses, is true when float is being changed
    for(int i=0;i< hidden_zero;i++){
      buffer[row][zero_start+i] = '0';
    }
  }
  
  for(int i=0;i<act_length;i++){//copy char array into the buffer at specific coordinates
    buffer[row][one_start + i] = cinsert[i];
  }
}

void insertIntToBuffer(uint8_t row, uint8_t column, int* value, uint8_t max_length){//insert variable into buffer
  char cinsert[7];
  uint8_t length = getLength(*value);
  uint8_t hidden_zero = max_length - length;
  uint8_t zero_start = column - max_length +1;
  intToString(*value).toCharArray(cinsert, length+1);//converting variable into char array

  //show all possible digits when editing number:
  if(edit_var[current_display]->content == value){//compare addresses, is true when int is being changed
    for(int i=0;i< hidden_zero;i++){
      buffer[row][zero_start+i] = '0';
    }
  }
  for(int i=0;i<length;i++){//copy char array into the buffer at specific coordinates
    buffer[row][column-length+1+i] = cinsert[i];
  }
}

void insertBoolToBuffer(uint8_t row, uint8_t column, bool* value, uint8_t submen){//insert variable into buffer
  char cinsert[6];
  uint8_t size;
  switch(submen){//bool can be displayed in various options, depending on submenu
    default:
      if(*value == 1){//checkbox
        strcpy(cinsert, menu_selected);
        size = sizeof(menu_selected);
      }
      else if (*value == 0){
        strcpy(cinsert, menu_unselected);
        size = sizeof(menu_unselected);
      }
    break;
    case SUMMARY:
      if(*value == 1){//NO/YES
        strcpy(cinsert, str_yes);
        size = sizeof(str_yes);
      }
      else if (*value == 0){
        strcpy(cinsert, str_no);
        size = sizeof(str_no);
      }
    break;
    case CURRENT:
      if(*value == 1){//1.00/2.00
        strcpy(cinsert, str_dishig);
        size = sizeof(str_dishig);
      }
      else if (*value == 0){
        strcpy(cinsert, str_dislow);
        size = sizeof(str_dislow);
      }
    break;
    case CHARGER_INTERFACE:
      goto onoff;
    break;
    case SUPPLY_INTERFACE:
      onoff:
      if(*value == 1){//ON/OFF
        strcpy(cinsert, str_on);
        size = sizeof(str_on);
      }
      else if (*value == 0){
        strcpy(cinsert, str_off);
        size = sizeof(str_off);
      }
    break;
  }
  for(int i=0;i<=(size-2);i++){ //copy and cut off terminating null-character (\0)
    buffer[row][column+i] = cinsert[i];
  }
}

void insertStringToBuffer(uint8_t row, uint8_t column, String* value){//insert string into buffer
  char cinsert[20];
  strcpy(cinsert, (*value).c_str());
  for(int i=0;i <= sizeof(*value);i++){
    buffer[row][column+i] = cinsert[i];
  }
}

void editFloat(float* val, int minval, int maxval, bool direction, int8_t multipler ){ // dir: 0=- 1=+
  if(direction){
    *val = constrain(*val + pow(10, multipler) + 0.00001, minval, maxval);
  }
  else{
    *val = constrain(*val - pow(10, multipler) + 0.00001, minval, maxval);
  }
}

void editInt(int* val, int minval, int maxval, bool direction, int8_t multipler ){ // dir: 0=- 1=+
  if(direction){
    *val = constrain(*val + pow(10, multipler) + 0.00001, minval, maxval);
  }
  else{
    *val = constrain(*val - pow(10, multipler) + 0.00001, minval, maxval);
  }
}

void editBool(bool* val){
  *val = !*val;
}

void cursorPositioning(int8_t direction, uint8_t lcd_number, int8_t* edit_multipler ){ //direction: -1/+1==dir 2==reload 0==init
  uint8_t sub;
  uint8_t men;
  
  if(lcd_number==1){
    sub = submenu_lcd1;
    men = menu_lcd1;
  }
  else if(lcd_number==2){
    sub = submenu_lcd2;
    men = menu_lcd2;
  }
  
  uint8_t max_length = log10(*current_menuvar[current_display][men].upper_border);
  uint8_t cursor;
    
  if(direction == 0){
    *edit_multipler = 0; //starting at multiplier 10^0=1
    cursor = current_menuvar[current_display][men].column;
  }
  else if(direction == 2){
    if(*edit_multipler < 0) cursor  = current_menuvar[current_display][men].column - *edit_multipler + 1;
    else                   cursor  = current_menuvar[current_display][men].column - *edit_multipler;
  } 
  else{
    *edit_multipler = constrain(*edit_multipler + direction, (-1) * current_menuvar[current_display][men].decimals , max_length);
    if(*edit_multipler < 0) cursor  = current_menuvar[current_display][men].column - *edit_multipler + 1;
    else                   cursor  = current_menuvar[current_display][men].column - *edit_multipler;
  }
  if(lcd_number==1){
    updateCursor1(cursor, menu_lcd1%4);
  }
  else if(lcd_number==2){
    updateCursor2(cursor, menu_lcd2%4);
  }
}

void updateMenu() { 
  switch ( uiKeyCode ) {
    //MIDDLE PANEL----------------
    case PM_BTN_DN:
      switch (submenu_dotm) {
        default:
          menu_dotm++;
          Serial.println(menu_dotm);
          if(scroll_mode){
            if (menu_dotm > menu_items[submenu_dotm]-1){
              menu_dotm = 0;
            }
          }
          else{
            menu_dotm = constrain(menu_dotm, 0, menu_items[submenu_dotm]-1);
          }
          Serial.println(menu_dotm);
        break;
      }
      redraw_dotm = 1;
      break;

    case PM_BTN_UP:
      switch (submenu_dotm) {
        default:
          menu_dotm--;
          Serial.println(menu_dotm);
          if(scroll_mode){
            if(menu_dotm == -1){
              menu_dotm = menu_items[submenu_dotm]-1;
            }
          }
          else{
            menu_dotm = constrain(menu_dotm, 0, menu_items[submenu_dotm]-1);
          }
          Serial.println(menu_dotm);
        break;
      }
      redraw_dotm = 1;
      break;
      
    case PM_BTN_RT:
    D("a");
      ui_buffer = pgm_read_byte(&link_next[submenu_dotm][constrain(menu_dotm,0,7)]);
    D(ui_buffer);
      if(ui_buffer == MENU_NONE) break; 
      switch (submenu_dotm) {
        case CHARGER:
          operating_mode = menu_dotm;
          switchMode(operating_mode);
          submenu_lcd1 = PROFILE;
          submenu_lcd2 = PROFILE;
          redraw_lcd1 = 1;
          redraw_lcd2 = 1;
        break;
        case SUPPLY:
          operating_mode = menu_dotm + 3;
          switchMode(operating_mode);
          submenu_lcd1 = SUPPLY_INTERFACE;
          submenu_lcd2 = SUPPLY_INTERFACE;
          redraw_lcd1 = 1;
          redraw_lcd2 = 1;
        break;
        case ABORT:
          if(operating_mode <= 3){
            submenu_dotm = CHARGER_HINT;
            menu_dotm = 0;
          }
          else{
            submenu_dotm = SUPPLY_HINT;
            menu_dotm = 0;
          }
          menu_dotm = 0;
          redraw_lcd1 = 1;
          redraw_lcd2 = 1;
        break;
        //Setting changes:
        case ENCODER:
          if (menu_dotm == 1) {
            encoder_mode = 1;
          }
          else if (menu_dotm == 2) {
            encoder_mode = 0;
          }
          writeSD(file_settings);//Save 
        break;
        case SCROLL:
          scroll_mode = menu_dotm-1;
          writeSD(file_settings);//Save 
        break;
      }
      if(ui_buffer != MENU_SELECT){
        submenu_dotm = ui_buffer;
        menu_dotm = 0;
      }
      redraw_dotm = 1;
    break;

    case PM_BTN_LT:
      current_display = 0;
      ui_buffer = pgm_read_byte(&link_back[submenu_dotm][0]);
      if(menu_dotm != 0 || ui_buffer == MENU_NONE) break;
      switch (submenu_dotm){
        default:
          default_back:
          submenu_dotm = ui_buffer;
        break;
        case CHARGER_HINT:
          if (edit_var[current_display] != &mv_none){//adding editvar2 required
            buzzer.duration = 50;
            buzzer.count = 1;
            return;
          }
          if(output_1 == 1 || output_2 == 1){
            submenu_dotm = ABORT;
            menu_dotm = 4;
            buzzer.duration = 250;
            buzzer.count = 1;
            break;
          }
          submenu_dotm = ui_buffer;
          menu_dotm = operating_mode;
          submenu_lcd1 = DOTTITLE;
          menu_lcd1 = 0;
          submenu_lcd2 = DOTTITLE;
          menu_lcd2 = 0;
          redraw_lcd1 = 1;
          redraw_lcd2 = 1;
          operating_mode = 0;
        break;
        case SUPPLY_HINT:
          if(output_1 == 1 || output_2 == 1){
            submenu_dotm = ABORT;
            menu_dotm = 4;
            buzzer.duration = 250;
            buzzer.count = 1;
            break;
          }
          if (edit_var[current_display] != &mv_none){
            buzzer.duration = 50;
            buzzer.count = 1;
            break;
          }
          submenu_dotm = ui_buffer;
          menu_dotm = operating_mode - 3;
          submenu_lcd1 = DOTTITLE;
          menu_lcd1 = 0;
          submenu_lcd2 = DOTTITLE;
          menu_lcd2 = 0;
          redraw_lcd1 = 1;
          redraw_lcd2 = 1;
          operating_mode = 0;
        break;
        case ABORT:
          submenu_dotm = MAIN;
          menu_dotm = 0;
          submenu_lcd1 = DOTTITLE;
          menu_lcd1 = 0;
          submenu_lcd2 = DOTTITLE;
          menu_lcd2 = 0;
          redraw_lcd1 = 1;
          redraw_lcd2 = 1;
          operating_mode = 0;
          output_1 = 0;
          output_2 = 0;
        break;
      }
      redraw_dotm = 1;
    break;
    
    //PANEL 1-------------------------------
    
    case P1_BTN_OK:
      ok:
      current_display = 1;
      if(current_menuvar[current_display][menu_lcd1].edit == 0)return;
      buffer_profile = active_profile_1;
      switch(current_menuvar[current_display][menu_lcd1].type){
        default:
          if(edit_var[current_display] != &mv_none){//change state by pressing ok
            edit_var[current_display] = &mv_none;
            lcd1.noBlink();
          }
          else{
            edit_var[current_display] = &current_menuvar[current_display][menu_lcd1];
            lcd1.blink();
            cursorPositioning(0, 1, &edit_multipler_1);
          }
        break;
        case type_bool:
          edit_var[current_display] = &current_menuvar[current_display][menu_lcd1];
          editBool((bool*)edit_var[current_display]->content);
          edit_var[current_display] = &mv_none;
        break;
      }
      active_profile_1 = buffer_profile;
      redraw_lcd1 = 1;
    break;
    
    case ENCODER_1_UP:
      current_display = 1;
      if(edit_var[current_display] != &mv_none){//check if edit Mode is active
        buffer_profile = active_profile_1;
        switch(current_menuvar[current_display][menu_lcd1].type){
          case type_int:
            editInt((int*)edit_var[current_display]->content, edit_var[current_display]->lower_border, *edit_var[current_display]->upper_border, 0, edit_multipler_1);
          break;
          case type_float:
            editFloat((float*)edit_var[current_display]->content, edit_var[current_display]->lower_border, *edit_var[current_display]->upper_border, 0, edit_multipler_1);
          break;
          case type_string:
          break;
        }
        redraw_lcd1 = 1;
        active_profile_1 = buffer_profile;
        return;
      }
      
      menu_lcd1--;
      if(scroll_mode){
        if (menu_lcd1 == -1) {
          menu_lcd1 = menu_items[submenu_lcd1]-1;
        }
      }
      else{
        if (menu_lcd1 == -1) menu_lcd1=0;
        menu_lcd1 = constrain(menu_lcd1,0,menu_items[submenu_lcd1]-1);
      }
      
      if(menu_lcd1%4==3){
        redraw_lcd1 = 1;
      }
      else{
        updateCursor1(19, menu_lcd1%4);
      }
      
    break;
    
    case ENCODER_1_DN:
      current_display = 1;
      if(edit_var[current_display] != &mv_none){//check if editFloat Mode is active
        buffer_profile = active_profile_1;
        switch(current_menuvar[current_display][menu_lcd1].type){
          case type_int:
            editInt((int*)edit_var[current_display]->content, edit_var[current_display]->lower_border, *edit_var[current_display]->upper_border, 1, edit_multipler_1);
          break;
          case type_float:
            editFloat((float*)edit_var[current_display]->content, edit_var[current_display]->lower_border, *edit_var[current_display]->upper_border, 1, edit_multipler_1);
          break;
          case type_string:
          break;
        }
        redraw_lcd1 = 1;
        active_profile_1 = buffer_profile;
        return;
      }
      menu_lcd1++;
      if(scroll_mode){
        if (menu_lcd1 > menu_items[submenu_lcd1]-1) {
          menu_lcd1 = 0;
        }
      }
      else{
        menu_lcd1 = constrain(menu_lcd1,0,menu_items[submenu_lcd1]-1);
      }
      
      if(menu_lcd1%4==0){
        redraw_lcd1 = 1;
      }
      else{
        updateCursor1(19, menu_lcd1%4);
      }
    break;

    case P1_BTN_LT:
      current_display = 1;
      ui_buffer = pgm_read_byte(&link_back[submenu_lcd1][menu_lcd1]);
      if(edit_var[current_display] != &mv_none){//check if editFloat Mode is active
        cursorPositioning(1, 1, &edit_multipler_1);
        return;
      }
      if(ui_buffer == MENU_NONE)break;
      switch(submenu_lcd1){
        default:
          submenu_lcd1 = ui_buffer;
          redraw_lcd1 = 1;
        break;
      }
    break;

    case P1_BTN_RT:
      current_display = 1;
      if(edit_var[current_display] != &mv_none){//check if editFloat Mode is active
        cursorPositioning(-1, 1, &edit_multipler_1);
        break;
      }
      goto rotsw;
    break;
    
    case P1_BTN_SW:
      rotsw:
      ui_buffer = pgm_read_byte(&link_next[submenu_lcd1][menu_lcd1]);
      if(ui_buffer == MENU_NONE) goto ok;
      switch(submenu_lcd1){
        default:
          submenu_lcd1 = ui_buffer;
        break;
        case PROFILE:
          if(SD_available && menu_lcd1 != 1){
            ui_buffer = pgm_read_byte(&link_next[submenu_lcd1][1]);
            submenu_lcd1 = ui_buffer;
            active_profile_1 = readProfile(menu_lcd1-2);
          }
          if(menu_lcd1==1){
            ui_buffer = pgm_read_byte(&link_next[submenu_lcd1][menu_lcd1]);
            submenu_lcd1 = ui_buffer;
            strcpy(active_profile_1.name, "Manuell");
            active_profile_1.name_length = 7;
            active_profile_1.number = 255;
            active_profile_1.balancing = 1;
            active_profile_1.charge_voltage = 4.00;
            active_profile_1.discharge_voltage = 2.00;
            active_profile_1.cells = 12;
          }
        break;
        case PROCED:
          current_display = 1;
          charging_mode[current_display-1] = menu_lcd1;
          submenu_lcd1 = ui_buffer;
        break;
      }
      menu_lcd1 = 0;
      redraw_lcd1 = 1;
    break;
    case P1_BTN_ON:
      switch(submenu_lcd1){
        case SUPPLY_INTERFACE:
        output_1 = true;
        break;
        case CHARGER_INTERFACE:
        output_1 = true;
        break;
      }
      if(submenu_dotm == CHARGER_HINT){
        submenu_dotm = GRAPHSELECT;
        redraw_dotm = 1;
      }
      redraw_lcd1 = 1;
    break;
    case P1_BTN_OFF:
      switch(submenu_lcd1){
        case SUPPLY_INTERFACE:
        output_1 = false;
        break;
        case CHARGER_INTERFACE:
        output_1 = false;
        break;
      }
      redraw_lcd1 = 1;
    break;


    

    //PANEL 2-------------------------------
    
    case P2_BTN_OK:
      ok2:
      current_display = 2;
      if(current_menuvar[current_display][menu_lcd2].edit == 0)return;
      buffer_profile = active_profile_2;
      switch(current_menuvar[current_display][menu_lcd2].type){
        default:
          if(edit_var[current_display] != &mv_none){//change state by pressing ok
            edit_var[current_display] = &mv_none;
            lcd2.noBlink();
          }
          else{
            edit_var[current_display] = &current_menuvar[current_display][menu_lcd2];
            lcd2.blink();
            cursorPositioning(0, 2, &edit_multipler_2);
          }
        break;
        case type_bool:
          edit_var[current_display] = &current_menuvar[current_display][menu_lcd2];
          editBool((bool*)edit_var[current_display]->content);
          edit_var[current_display] = &mv_none;
        break;
      }
      active_profile_2 = buffer_profile;
      redraw_lcd2 = 1;
    break;
    
    case ENCODER_2_UP:
      current_display = 2;
      if(edit_var[current_display] != &mv_none){//check if edit Mode is active
        buffer_profile = active_profile_2;
        switch(current_menuvar[current_display][menu_lcd2].type){
          case type_int:
            editInt((int*)edit_var[current_display]->content, edit_var[current_display]->lower_border, *edit_var[current_display]->upper_border, 0, edit_multipler_1);
          break;
          case type_float:
            editFloat((float*)edit_var[current_display]->content, edit_var[current_display]->lower_border, *edit_var[current_display]->upper_border, 0, edit_multipler_1);
          break;
          case type_string:
          break;
        }
        redraw_lcd2 = 1;
        active_profile_2 = buffer_profile;
        return;
      }
      
      menu_lcd2--;
      if(scroll_mode){
        if (menu_lcd2 == -1) {
          menu_lcd2 = menu_items[submenu_lcd2]-1;
        }
      }
      else{
        if (menu_lcd2 == -1) menu_lcd2=0;
        menu_lcd2 = constrain(menu_lcd2,0,menu_items[submenu_lcd2]-1);
      }
      
      if(menu_lcd2%4==3){
        redraw_lcd2 = 1;
      }
      else{
        updateCursor2(19, menu_lcd2%4);
      }
      
    break;
    
    case ENCODER_2_DN:
      current_display = 2;
      if(edit_var[current_display] != &mv_none){//check if editFloat Mode is active
        buffer_profile = active_profile_2;
        switch(current_menuvar[current_display][menu_lcd2].type){
          case type_int:
            editInt((int*)edit_var[current_display]->content, edit_var[current_display]->lower_border, *edit_var[current_display]->upper_border, 1, edit_multipler_1);
          break;
          case type_float:
            editFloat((float*)edit_var[current_display]->content, edit_var[current_display]->lower_border, *edit_var[current_display]->upper_border, 1, edit_multipler_1);
          break;
          case type_string:
          break;
        }
        redraw_lcd2 = 1;
        active_profile_2 = buffer_profile;
        return;
      }
      menu_lcd2++;
      if(scroll_mode){
        if (menu_lcd2 > menu_items[submenu_lcd2]-1) {
          menu_lcd2 = 0;
        }
      }
      else{
        menu_lcd2 = constrain(menu_lcd2,0,menu_items[submenu_lcd2]-1);
      }
      
      if(menu_lcd2%4==0){
        redraw_lcd2 = 1;
      }
      else{
        updateCursor2(19, menu_lcd2%4);
      }
    break;

    case P2_BTN_LT:
      current_display = 2;
      ui_buffer = pgm_read_byte(&link_back[submenu_lcd2][menu_lcd2]);
      if(edit_var[current_display] != &mv_none){//check if editFloat Mode is active
        cursorPositioning(1, 2, &edit_multipler_1);
        return;
      }
      if(ui_buffer == MENU_NONE)break;
      switch(submenu_lcd2){
        default:
          submenu_lcd2 = ui_buffer;
          redraw_lcd2 = 1;
        break;
      }
    break;

    case P2_BTN_RT:
      current_display = 2;
      if(edit_var[current_display] != &mv_none){//check if editFloat Mode is active
        cursorPositioning(-1, 2, &edit_multipler_2);
        break;
      }
      goto rotsw2;
    break;
    
    case P2_BTN_SW:
      rotsw2:
      ui_buffer = pgm_read_byte(&link_next[submenu_lcd2][menu_lcd2]);
      if(ui_buffer == MENU_NONE) goto ok2;
      switch(submenu_lcd2){
        default:
          submenu_lcd2 = ui_buffer;
        break;
        case PROFILE:
          if(SD_available && menu_lcd2 != 1){
            ui_buffer = pgm_read_byte(&link_next[submenu_lcd2][1]);
            submenu_lcd2 = ui_buffer;
            active_profile_2 = readProfile(menu_lcd2-2);
          }
          if(menu_lcd2==1){
            ui_buffer = pgm_read_byte(&link_next[submenu_lcd2][menu_lcd2]);
            submenu_lcd2 = ui_buffer;
            strcpy(active_profile_2.name, "Manuell");
            active_profile_2.name_length = 7;
            active_profile_2.number = 255;
            active_profile_2.balancing = 1;
            active_profile_2.charge_voltage = 4.00;
            active_profile_2.discharge_voltage = 2.00;
            active_profile_2.cells = 12;
          }
        break;
        case PROCED:
          current_display = 2;
          charging_mode[current_display-1] = menu_lcd2;
          submenu_lcd2 = ui_buffer;
        break;
      }
      menu_lcd2 = 0;
      redraw_lcd2 = 1;
    break;
    case P2_BTN_ON:
      switch(submenu_lcd2){
        case SUPPLY_INTERFACE:
        output_2 = true;
        break;
        case CHARGER_INTERFACE:
        output_2 = true;
        break;
      }
      if(submenu_dotm == CHARGER_HINT){
        submenu_dotm = GRAPHSELECT;
        redraw_dotm = 1;
      }
      redraw_lcd2 = 1;
    break;
    case P2_BTN_OFF:
      switch(submenu_lcd2){
        case SUPPLY_INTERFACE:
        output_2 = false;
        break;
        case CHARGER_INTERFACE:
        output_2 = false;
        break;
      }
      redraw_lcd2 = 1;
    break;
  }
}

//MODES
#define CHARGE_INDEP    1
#define CHARGE_SERIAL   2
#define CHARGE_PARALLEL 3
#define SUPPLY_INDEP    4
#define SUPPLY_SERIAL   5
#define SUPPLY_PARALLEL 6

void switchMode(uint8_t opm){
  if(opm % 3 != 0)common_mode = 1;
  else common_mode = 0;
  switch(opm){
    case CHARGE_INDEP:
      //mv_v_upper_border = 5; =const
      mv_c_upper_border = 60;
      mv_cel_upper_border = 12;
    break;
    case CHARGE_SERIAL:
      mv_c_upper_border = 60;
      mv_cel_upper_border = 24;
    break;
    case CHARGE_PARALLEL:
      mv_c_upper_border = 120;
      mv_cel_upper_border = 24;
    break;
    case SUPPLY_INDEP:
      mv_sv_supply_upper_border = 60;
      mv_sc_supply_upper_border = 50;
      mv_sp_supply_upper_border = 3000;
    break;  
    case SUPPLY_SERIAL:
      mv_sv_supply_upper_border = 120;
      mv_sc_supply_upper_border = 50;
      mv_sp_supply_upper_border = 6000;
    break;
    case SUPPLY_PARALLEL:
      mv_sv_supply_upper_border = 60;
      mv_sc_supply_upper_border = 100;
      mv_sp_supply_upper_border = 6000;
    break;
     
  }
}

