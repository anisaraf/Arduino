#include <TimedAction.h>
#include <TM1638.h>
#include "Pomodoro.h"
// define a module on data pin 8, clock pin 9 and strobe pin 10
TM1638 tm(8, 9, 10);
boolean dots=0;
boolean moduleOff=0;
int work_time = 25;
int blink_state = true;
 
Pomodoro p(work_time, 10);
Counter water_counter(60);
Counter productive_counter(0);
Counter unproductive_counter(0);
Counter social_counter(0);


void checkTime(){
  dots = !dots;
  drawToModule();
}

void checkButtons(){
 buttonEvent();
}

void drawToModule(){
  int water_counter_led = 0;
  int productive_counter_led = 1;
  int unproductive_counter_led = 2;
  int social_counter_led = 3;


  int pause_led = 7;
  tm.setLED( (p.IsStarted() & p.IsPaused()) ? TM1638_COLOR_RED : TM1638_COLOR_GREEN, pause_led);
  tm.setLED(water_counter.LedOn() & blink_state ? TM1638_COLOR_RED : TM1638_COLOR_GREEN, water_counter_led);
  tm.setLED(productive_counter.LedOn() ? TM1638_COLOR_RED : TM1638_COLOR_GREEN, productive_counter_led);
  tm.setLED(unproductive_counter.LedOn() ? TM1638_COLOR_RED : TM1638_COLOR_GREEN, unproductive_counter_led);
  tm.setLED(social_counter.LedOn() ? TM1638_COLOR_RED : TM1638_COLOR_GREEN, social_counter_led);


  blink_state = !blink_state;  
  
  tm.clearDisplay();
  if (!moduleOff){
    tm.setDisplayToString(String(water_counter.GetCount()),0,0);
    tm.setDisplayToString(String(productive_counter.GetCount()),0,1);
    tm.setDisplayToString(String(unproductive_counter.GetCount()),0,2);
    tm.setDisplayToString(String(social_counter.GetCount()),0,3);

    tm.setDisplayToString(p.GetCompletedCount(),0,4);
    tm.setDisplayToString(String(p.distraction_count),(dots*255),5);
    tm.setDisplayToString(p.GetDisplayMinutes(),0,6);
  }
}
 
int findFirstSet(byte num) {
  for(int i = 0; i < 8; i++){
    if(num &  (1 << i))
      return i;
  }
  return -1;
}
void buttonEvent(){
  byte buttons=tm.getButtons();
  int inp = findFirstSet(buttons);
  switch (inp) {
  case 0:
    water_counter.Increment();
    break;
  case 1:  
    productive_counter.Increment();
    break;
  case 2:
    unproductive_counter.Increment();
    break;
  case 3:   
    social_counter.Increment();
    break;
  case 4:
    moduleOff = !moduleOff;
    if (moduleOff) tm.clearDisplay();     
    break;
  case 5:
    p.distraction_count +=1;
    break;
  case 6:
    Serial.print("Reset");
    p.Reset();
    break;
  case 7:
    Serial.print("TogglePause");
    p.TogglePause();
    break;
  }
}


TimedAction timedCheckButtons = TimedAction(200, checkButtons);
TimedAction timedUpdate = TimedAction(1000, checkTime);


void setup(){
  Serial.begin(9600);   
  p.Start();
  
}

void loop(){
  timedCheckButtons.check();
  timedUpdate.check();
}




 
