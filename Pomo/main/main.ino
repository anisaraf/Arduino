
#include <TimedAction.h>
#include <TM1638.h>
#include <time.h>
#include "googlesheet.h"
#include "Pomodoro.h"
#include "web.h"
#include "ota.h"


// define a module on data pin 8, clock pin 9 and strobe pin 10
TM1638 tm(D2, D3, D4);
boolean dots=0;
boolean moduleOff=0;
int work_time = 25;
int blink_state = true;
 
Pomodoro p(work_time, 10);
Counter water_counter(30);
Counter last_pomodoro(60);
Counter productive_counter(0);
Counter unproductive_counter(0);
Counter social_counter(0);


void checkWeb() {
  postData();
}
void checkTime(){
  dots = !dots;
  drawToModule();
}

void checkButtons(){
 buttonEvent();
}

void drawToModule(){
  int water_counter_led = 0;
  int pause_led = 6;
  int pomodoro_led = 0;


  if(p.animate) {
    p.animate = false;
    animateLed();
  }
  
  tm.setLED( (p.IsStarted() & p.IsPaused()) ? TM1638_COLOR_RED : TM1638_COLOR_GREEN, pause_led);
  tm.setLED(water_counter.LedOn() & blink_state ? TM1638_COLOR_RED : TM1638_COLOR_GREEN, water_counter_led);
  tm.setLED(last_pomodoro.LedOn() & blink_state ? TM1638_COLOR_RED : TM1638_COLOR_GREEN, pomodoro_led);


  blink_state = !blink_state;  
  
  tm.clearDisplay();
  if (!moduleOff){
    tm.setDisplayToString(getCurrentTime(),dots*79,0);
    //tm.setDisplayToString(String(productive_counter.GetCount()),0,1);
    //tm.setDisplayToString(String(unproductive_counter.GetCount()),0,2);
    //tm.setDisplayToString(String(social_counter.GetCount()),0,3);

    tm.setDisplayToString(p.GetCompletedCount(),0,4);
    tm.setDisplayToString(String(p.distraction_count), 0 ,5);
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
    Serial.print("TogglePause");
    p.TogglePause();
    break;
  case 7:
    Serial.print("Reset");
    p.Reset();
    if(p.IsStarted()){
      last_pomodoro.Increment();
    }
    break;
  }
}

String formatNumber(String inp){
      if (inp.length()==1) {
        return "0" + inp;
      }
      else{
        return inp;
      }
}

time_t start_time = 0;
String getCurrentTime() {
  time_t now = start_time + millis()/1000;
  struct tm *tmp = gmtime(&now);
  return formatNumber(String(tmp->tm_hour %12 +1)) + formatNumber(String(tmp->tm_min));
}

TimedAction timedCheckButtons = TimedAction(200, checkButtons);
TimedAction timedUpdate = TimedAction(1000, checkTime);
TimedAction webUpdate = TimedAction(1000*60, checkWeb);

void animateLed() {
  for(int i = 0; i < 8; i++) {
   tm.setLED(TM1638_COLOR_RED, i);
   if(i > 0) {
    tm.setLED(TM1638_COLOR_GREEN, i-1);
   }
   delay(500);
  }
  tm.setLED(TM1638_COLOR_GREEN, 7);
}

void setup(){
  //pinMode(BUILTIN_LED, OUTPUT);  // set onboard LED as output
  //digitalWrite(BUILTIN_LED, 1);  // sets the LED to the button's value

  Serial.begin(9600);
  Serial.print("Started");  
  setupWiFi(); 
  setupClient();
  //postData();
  p.Start();
  start_time = getCurrentTimeNTP();
  Serial.print(getCurrentTime());
  SetupOTAHandlers();
  animateLed();
}

void loop(){
  timedCheckButtons.check();
  timedUpdate.check();
  ArduinoOTA.handle();
  // jpostData();
}




 
