class Pomodoro {
  public:
    Pomodoro(int work_time, int break_time) {
      this->checked_time_ = 1;
      this->completed_count_ = 0;
      this->work_time_ = work_time;
      this->animate = false;
      this->Reset();
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
      this->checked_time_ = round(millis() / 1000);
      this->elapsed_time();
      this->paused_ = false;
    }

    bool IsPaused() {
      return this->paused_;
    }

    bool IsStarted() {
      return (this->checked_time_ > 0);
    }

    void TogglePause() {
      if (IsStarted()) {
        this->elapsed_time();
        this->paused_ = !this->paused_;
      }
    }

    void Reset() {
      if(this->checked_time_ < 0) {
        this->Start();
        return;
      }
      this->elapsed_time_ = 0;
      this->paused_time_ = 0;
      this->checked_time_ = -1;
      this->paused_ = false;
      this->distraction_count = 0;
    }
    
    int distraction_count;
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
};

class Counter {
  public:
    Counter(unsigned int led_time){
      this->led_time_ = led_time * 60;
      this->last_press_ = round(millis() / 1000);
    }

    unsigned int GetCount() {
      return this->count_ % 10;
    }

   void Increment() {
      this->last_press_ = round(millis() / 1000);
      this->count_++;
    }

    bool LedOn() {
     int current_time = round(millis() / 1000);
     if(this->led_time_ > 0) {
      return ((current_time - this->last_press_) > this->led_time_);    
     } else {
      return (this->count_ /10) % 2 > 0;
     }
    }

  private:
    unsigned int count_ = 0;
    unsigned int last_press_ = 0;
    unsigned int led_time_;
};

