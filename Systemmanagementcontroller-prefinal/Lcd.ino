//---Display outputs---

void drawTitle(){
  u8g.drawXBMP( 0, (64 - title_xbm_height) / 2 , title_xbm_width, title_xbm_height, title_xbm);
}

void drawMenu() {//graphic menu
  uint8_t i, h;
  u8g_uint_t w, d;
  uint8_t split_multiplier = 0;

   //define font & properties
  u8g.setFont(u8g_font_6x13);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();

  split_multiplier = menu_dotm / MENU_SPLIT;//detecting which page currently is highlighted
  h = u8g.getFontAscent() - u8g.getFontDescent() + 2;//calculate font height
  w = u8g.getWidth(); //get font width

  for ( i = 0; i < entries; i++ ) {//loop for every single row
    u8g.setDefaultForegroundColor();
    if ( i == menu_dotm ) {//highlight active row
      u8g.drawBox(0, (i - split_multiplier * MENU_SPLIT) * h, w, h);
      u8g.setDefaultBackgroundColor();
    }
    d = (w - u8g.getStrWidth(buffer[i])) / 2;//calculate start point for center alignment
    u8g.drawStr(d, (i - split_multiplier * MENU_SPLIT) * h, buffer[i]);//draw string
    if (i == 0 && split_multiplier == 0) {//draw frame of the current submenu heading
      u8g.drawFrame(0 , 0 , 128, 13);
      if (submenu_dotm != 0)u8g.drawStr(2, 1 , menu_back);
    }
    if (submenu_dotm == ENCODER && i > 0 ) {//old manipulation technique of variable data, has not been replaced with standardized one
      if (i == 1) {
        state = encoder_mode;
      }
      else if (i == 2) {
        state = !encoder_mode;
      }
      u8g.drawStr(110, (i - split_multiplier * MENU_SPLIT) * h + 1 , menu_checkbox[state]);
    }
    if (submenu_dotm == SCROLL && i > 0 ) {//old manipulation technique of variable data, has not been replaced with standardized one
      if (i == 1) {
        state = !scroll_mode;
      }
      else if (i == 2) {
        state = scroll_mode;
      }
      u8g.drawStr(110, (i - split_multiplier * MENU_SPLIT) * h + 1 , menu_checkbox[state]);
    }
  }
}

void init_LCD(){
  lcd1.begin(20,4);
  lcd2.begin(20,4);
  lcd1.backlight();
  lcd2.backlight();
  lcd1.setCursor(0, 0);
  lcd1.print(title);
  lcd2.setCursor(0, 0);
  lcd2.print(title);
  lcd1.setCursor(0, 1);
  lcd1.print(SD_check);
  lcd2.setCursor(0, 1);
  lcd2.print(SD_check);
  if(SD_available){
    lcd1.print(ok);
    lcd2.print(ok);
  }
  else{
    lcd1.print(fail);
    lcd2.print(fail);
  }
}

void updateCursor1(uint8_t column, uint8_t row){
  cursorRow1 = row;
  cursorColumn1 = column;
  lcd1.setCursor(cursorColumn1 ,cursorRow1);
  lcd1.cursor();
}

void updateCursor2(uint8_t column, uint8_t row){
  cursorRow2 = row;
  cursorColumn2 = column;
  lcd2.setCursor(cursorColumn2 ,cursorRow2);
  lcd2.cursor();
}

void writeMenu1(){
  uint8_t split_multiplier = 0;
  split_multiplier = menu_lcd1 / 4;
  lcd1.noCursor();
  for (int i = 0; i < 4; i++) {
    lcd1.setCursor(0, i);
    lcd1.print(blank);
    lcd1.setCursor(0, i);
    int m = i+4*split_multiplier;
    if(m<entries){
      lcd1.print(buffer[m]);
    }
    else{
      lcd1.print(blank);
    } 
  }
}

void writeMenu2(){
  uint8_t split_multiplier = 0;
  split_multiplier = menu_lcd2 / 4;
  lcd2.noCursor();
  for (int i = 0; i < 4; i++) {
    lcd2.setCursor(0, i);
    lcd2.print(blank);
    lcd2.setCursor(0, i);
    int m = i+4*split_multiplier;
    if(m<entries){
      lcd2.print(buffer[m]);
    }
    else{
      lcd2.print(blank);
    } 
  }
}

