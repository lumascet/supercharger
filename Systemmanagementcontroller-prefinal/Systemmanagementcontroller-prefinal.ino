//---Main Routine---

bool debug_log = 1; //debug logging on sd card
bool debug_print = 1; //debug logging on serial, turn off if using serial communication!

//DEBUG SHORTCUTS
#define D(x) if(debug_print) Serial.println(x) //DEBUG NORMAL
#define DR(x) if(debug_print) Serial.print(x) //DEBUG no return
#define DE(x) if(debug_print) Serial.println("[A]" + (String)x + "[E]") //DEBUG EXTENDED

#include <SPI.h>
#include <SD.h>
#include "ClickEncoder.h"
#include <TimerOne.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "TaskScheduler.h"
#include "U8glib.h"
#include "HardwareAllocation.h"
#include "Serial.h"
#include "GlobalVar.h"
#include "Menu.h"

//Scheduler
Scheduler runner;

void setup() {
  init_Pin();//initialize pin setup
  init_rotary_encoder();//VERY IMPORTANT THAT THIS IS BEFORE TIMER1 INITALISATION OR ELSE ARDUINO WILL CRASH
  Timer1.initialize(1000);//every 10ms
  Timer1.attachInterrupt(timerIsr);
  u8g.setRot180();//rotate display output
  u8g.firstPage(); 
  do{//Draw Title
    drawTitle();//draw the supercharger logo
  }while( u8g.nextPage() );  
  init_SD();
  sch_init();
  Serial.begin(BAUD, SERIAL_CONF);
  Serial1.begin(BAUD, SERIAL_CONF);
  Serial2.begin(BAUD, SERIAL_CONF);
  Serial3.begin(BAUD, SERIAL_CONF);
  readSettings();//read saved settings from sd card
  countEntries();//count menu entries
  init_LCD();//initialize line displays
}

void loop() {
  if(system_halt){
    return;
  }
  runner.execute();
  
  if(redraw_dotm != 0) {
    current_display = 0;//for identifiying current display routine
    loadMenuEntries(submenu_dotm);//load menu into buffer
    u8g.firstPage();//routine for u8g display output
    do{
      drawMenu();
    }while ( u8g.nextPage() );
    updateLight();//update button lights
    redraw_dotm = 0;//reset redraw state
    D("m_dotm: " + (String)menu_dotm + " sub_dotm:" + (String)submenu_dotm);
  }
  
  if(redraw_lcd1 != 0){
    current_display = 1;//for identifiying current display routine
    buffer_profile = active_profile_1;//load profile into buffer
    loadMenuEntries(submenu_lcd1);//load menu into buffer
    writeMenu1();//write buffer to display
    if(edit_var[current_display] == &mv_none){//update cursor
      updateCursor1(19, menu_lcd1%4);
    }
    else{
      cursorPositioning(2, 1, &edit_multipler_1);
    }
    
    updateLight();//update button lights
    redraw_lcd1 = 0;//reset redraw state
    D("m_lcd1: " + (String)menu_lcd1 + " sub_lcd1:" + (String)submenu_lcd1);
  }

  if(redraw_lcd2 != 0){
    current_display = 2;//for identifiying current display routine
    buffer_profile = active_profile_2;//load into buffer
    loadMenuEntries(submenu_lcd2);//load menu into buffer
    writeMenu2();//write buffer to display
    if(edit_var[current_display] == &mv_none){//update cursor
      updateCursor2(19, menu_lcd2%4);
    }
    else{
      cursorPositioning(2, 2, &edit_multipler_2);
    }
    
    updateLight();//update button lights
    redraw_lcd2 = 0;//reset redraw state
    D("m_lcd2: " + (String)menu_lcd2 + " sub_lcd2:" + (String)submenu_lcd2);
  }
}


void timerIsr() {//Interrupt service routine (Timer1)
  encoder1->service();
  encoder2->service();
  buzzer_task();
}

