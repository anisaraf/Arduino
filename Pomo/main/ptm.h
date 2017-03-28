#ifndef PTM_H
#define PTM_H

#include <TM1638.h>
#include "pomodoro.h"
#include "counter.h"
#include "googlesheet.h"

namespace ptm {
  TM1638 tm(D2, D3, D4);
  Accumulator accumulate(4);
  boolean blink_state = true;
  Pomodoro p(25 /*work_time*/, 10);
  Counter water_counter(30);
  Counter last_pomodoro(60);
  boolean moduleOff=false;
  String water_log_sheet = "WaterLog";
  String pomodoro_log_sheet = "Pomodoro";

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

  void DisplayUpdate(){
    blink_state = !blink_state;  
    int water_counter_led = 0;
    int pause_led = 6;
    int last_pomodoro_led = 7;

    if(p.animate) {
      p.animate = false;
      animateLed();
    }

    int active_counter = accumulate.GetActive();
    tm.setLED((p.IsStarted() & p.IsPaused()) ? TM1638_COLOR_RED : TM1638_COLOR_GREEN, pause_led);
    tm.setLED(water_counter.LedOn() & blink_state ? TM1638_COLOR_RED : TM1638_COLOR_GREEN, water_counter_led);
    tm.setLED( active_counter == 1 ? TM1638_COLOR_RED : TM1638_COLOR_GREEN, 1);
    tm.setLED( active_counter == 2 ? TM1638_COLOR_RED : TM1638_COLOR_GREEN, 2);
    tm.setLED( active_counter == 3 ? TM1638_COLOR_RED : TM1638_COLOR_GREEN, 3);
    // Flash LED if more than 60 minutes, otherwise show red if not in Pomodoro.
    if(last_pomodoro.LedOn()) {
      tm.setLED(blink_state ? TM1638_COLOR_RED : TM1638_COLOR_GREEN, last_pomodoro_led);
    } else {
      tm.setLED(!p.IsStarted() ? TM1638_COLOR_RED : TM1638_COLOR_GREEN, last_pomodoro_led);
    }


    tm.clearDisplay();
    if (!moduleOff){
      tm.setDisplayToString(utils::getTimeString(utils::getCurrentTime()),int(blink_state)*79,0);
      if( active_counter > 0) {
	tm.setDisplayToString(utils::getTimeString(accumulate.GetAccumulatedTime()),int(blink_state)*79,4);
      } else {
	// Pomodoro mode
	tm.setDisplayToString(p.GetCompletedCount(),0,4);
	tm.setDisplayToString(String(p.GetDistractionCount()), 0 ,5);
	tm.setDisplayToString(p.GetDisplayMinutes(),0,6);
      }
    }
  }

  //------------------------------- Buttons Pressed -------------------------------  

  void Button0(){
    water_counter.Increment();
    googlesheet::postData(water_log_sheet, "drank");
  }

  void Button1(){
    p.Stop();
    accumulate.SetActive(1);
    googlesheet::postData("Pomodoro", "Meeting");
  }

  void Button2(){
    p.Stop();
    accumulate.SetActive(2);
    googlesheet::postData("Pomodoro", "Social");
  }

  void Button3(){
    p.Stop();
    accumulate.SetActive(3);
    googlesheet::postData("Pomodoro", "Unfocused");	
  }

  void Button4(){
    moduleOff = !moduleOff;
    if (moduleOff) {
      tm.clearDisplay();
    }
  }

  void Button5(){
    p.IncrementDisctractionCount();
    googlesheet::postData(water_log_sheet, "drank");
  }

  void Button6(){
    Serial.print("TogglePause");
    p.TogglePause();
  }

  void Button7(){
    Serial.print("Reset");
    p.Reset();
    if(p.IsStarted()){
      accumulate.SetActive(0);
      accumulate.SetActive(0);
      last_pomodoro.Increment();
    }
  }

  // function type
  typedef void (*FunctionCallback)();
  FunctionCallback button_functions[] = {&Button0,&Button1,&Button2,&Button3,&Button4,&Button5,&Button6,&Button7};

  void CheckButtons(){
    byte buttons=tm.getButtons();
    int pressed = -1;
    for(int i = 0; i < 8; i++){
      if(buttons &  (1 << i)) {
	pressed = i;
      }
    }
    if(pressed > -1) {
      Serial.print(pressed);
      button_functions[pressed]();
    }
  }

}
#endif
