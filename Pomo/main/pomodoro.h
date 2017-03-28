#ifndef POMODORO_H
#define POMODORO_H

#include "googlesheet.h"

class Pomodoro {
  public:
    Pomodoro(int work_time, int break_time) {
      this->checked_time_ = 1;
      this->completed_count_ = 0;
      this->work_time_ = work_time;
      this->animate = false;
      this->elapsed_time_ = 0;
      this->paused_time_ = 0;
      this->checked_time_ = -1;
      this->paused_ = false;
      this->distraction_count_ = 0;
    }

    int elapsed_time() {
      int current_time = round(millis() / 1000);
      if(this->checked_time_ < 0)
          return 0;
      if(paused_) {
        this->paused_time_ += current_time - this->checked_time_;
      } else {
        this->elapsed_time_ += current_time - this->checked_time_;
      }
      this->checked_time_ = current_time;

      // Stop if total work time elapsed for Pomodoro
      if(this->elapsed_time_ > this->work_time_ * 60) {
        this->completed_count_ +=1;
        this->Reset();
        this->animate = true;
	this->LogSheetMessage("Completed");
      }
    }

    String GetCompletedCount() {
      return String(byte(this->completed_count_));
    }
    String GetDisplayMinutes(){
      this->elapsed_time();
      unsigned long seconds = 0;
      if(paused_){
        seconds = this->paused_time_;
      } else {
        seconds = (this->work_time_ +1) * 60 - this->elapsed_time_;
      }
      return this->minuteStr(seconds/60 % 60); 
    }

    void Start() {
      if(!IsStarted()) {
	this->checked_time_ = round(millis() / 1000);
	this->elapsed_time();
	this->paused_ = false;
	this->LogSheetMessage("Start");
      }
    }

    void Stop() {
      if(IsStarted()) {
	this->elapsed_time_ = 0;
	this->paused_time_ = 0;
	this->checked_time_ = -1;
	this->paused_ = false;
	this->distraction_count_ = 0;
	this->LogSheetMessage("Stop");
      }
    }

    void Pause() {
      if (IsStarted()) {
        this->elapsed_time();
        this->paused_ = true;
	this->LogSheetMessage("Paused");
      } 
    }

    bool IsPaused() {
      return this->paused_;
    }

    bool IsStarted() {
      return (this->checked_time_ > 0);
    }

    void TogglePause() {
      if (IsStarted()) {
	Pause();
      } else {
	Start();
      }
    }

    void Reset() {
      if(!IsStarted()) {
        this->Start();
        return;
      }
      Stop();
    }

    void IncrementDisctractionCount(){
      distraction_count_++;
      this->LogSheetMessage("Distracted");
    }
    
    int GetDistractionCount(){
      return distraction_count_;
    }
    
    bool animate;

  private:
    unsigned long paused_time_;
    unsigned long elapsed_time_;
    unsigned long work_time_;
    int checked_time_;
    int distraction_count_;
    bool paused_;
    unsigned long completed_count_;
    
    String formatNumber(String inp){
      if (inp.length()==1) {
        return "0" + inp;
      }
      else{
        return inp;
      }
    }
    
    byte minute(unsigned long inp){
     return byte((inp % 60));
    }

    String minuteStr(unsigned long inp){
     return  formatNumber(String(minute(inp)));
    }

    void LogSheetMessage(String message) {
      googlesheet::postData("Pomodoro", message);
    }

};

#endif

