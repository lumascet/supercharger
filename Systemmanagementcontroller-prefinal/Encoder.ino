//---Rotary encoder---

// #include <ClickEncoder.h>
// #include <TimerOne.h>
// ClickEncoder *encoder1;
// ClickEncoder *encoder2;

int16_t last1, value1;
int16_t last2, value2;

void init_rotary_encoder(){
  encoder1 = new ClickEncoder(P1_BTN_DT, P1_BTN_CLK, P1_BTN_SW, 2);// DT, CLK, SW, steps per notch
  encoder2 = new ClickEncoder(P2_BTN_DT, P2_BTN_CLK, P2_BTN_SW, 2);
  
  encoder1->setAccelerationEnabled(0);
  encoder2->setAccelerationEnabled(0);
  
  last1 = -1;
  last2 = -1;
}

int getEncoder1Code(){
  int keyCode = 0;
  switch(encoder1->getValue()){
    case 1:
      keyCode = ENCODER_1_DN;
    break;
    case -1:
      keyCode = ENCODER_1_UP;
    break;
  }
  /*
  ClickEncoder::Button b = encoder1->getButton();
  if (b != ClickEncoder::Open) {
  switch (b) {
    case ClickEncoder::Pressed:
    break;
    case ClickEncoder::Held:
    break;
    case ClickEncoder::Released:
    break;
    case ClickEncoder::Clicked:
      keyCode = P1_BTN_SW;
    break;
    case ClickEncoder::DoubleClicked:
    break;
    }
  }
  */
  return keyCode;
}

int getEncoder2Code(){
  int keyCode = 0;
  switch(encoder2->getValue()){
    case 1:
      keyCode = ENCODER_2_DN;
    break;
    case -1:
      keyCode = ENCODER_2_UP;
    break;
  }
  /*
  ClickEncoder::Button b = encoder2->getButton();
  if (b != ClickEncoder::Open) {
  switch (b) {
    case ClickEncoder::Pressed:
    break;
    case ClickEncoder::Held:
    break;
    case ClickEncoder::Released:
    break;
    case ClickEncoder::Clicked:
      keyCode = P2_BTN_SW;
    break;
    case ClickEncoder::DoubleClicked:
    break;
    }
  }
  */
  return keyCode;
}

void rotary_encoder1(){
  bool direction;
  value1 += encoder1->getValue();
  value1 = constrain(value1, 0, menu_items[submenu_lcd1]-1);
  if (value1 != last1) {
    if(value1-last1==1){
      direction = 1;
    }
    else{
      direction = 0;
    }
    last1 = value1;
    menu_lcd1 = value1;
    if((direction == 0 && menu_lcd1%4==3)||(direction == 1 && menu_lcd1%4==0)){
      //Serial.println("a");
      redraw_lcd1 = 1;
    }
    else{
      //Serial.println("b");
      updateCursor1(19, menu_lcd1%4);
    }
    Serial.println("Encoder 1 Value:\t" + (String) value1);
  }
  /*
  ClickEncoder::Button b = encoder1->getButton();
  if (b != ClickEncoder::Open) {
    switch (b) {
      case ClickEncoder::Pressed:
      break;
      case ClickEncoder::Held:
      break;
      case ClickEncoder::Released:
      break;
      case ClickEncoder::Clicked:
      break;
      case ClickEncoder::DoubleClicked:
      break;
    }
  }
  */
}

void rotary_encoder2(){
  bool direction;
  encoder2->service();
  value2 += encoder2->getValue();
  value2 = constrain(value2, 0, menu_items[submenu_lcd2]-1);
  if (value2 != last2) {
    if(value2-last2==1){
      direction = 1;
    }
    else{
      direction = 0;
    }
    last2 = value2;
    menu_lcd2 = value2;
    if((direction == 0 && menu_lcd2%4==3)||(direction == 1 && menu_lcd2%4==0)){
      //Serial.println("a");
      redraw_lcd2 = 1;
    }
    else{
      //Serial.println("b");
      updateCursor2(19, menu_lcd2%4);
    }
    Serial.println("Encoder 2 Value:\t" + (String) value2);
  }
  /*
  ClickEncoder::Button b = encoder2->getButton();
  if (b != ClickEncoder::Open) {
    switch (b) {
      case ClickEncoder::Pressed:
      break;
      case ClickEncoder::Held:
      break;
      case ClickEncoder::Released:
      break;
      case ClickEncoder::Clicked:
      break;
      case ClickEncoder::DoubleClicked:
      break;
    }
    */
}
