#ifndef COUNTER_H
#define COUNTER_H


/* Counter: if led_time is set, then returns a bool on whether to
   flash led if led_time has elapsed since last increment.

   If led_time is set to 0, then returns LedOn if count > 9.
 */
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

class Accumulator {
 public:
  Accumulator(int size) {
    accumulators = (time_t*)calloc(size, sizeof(time_t));
    active_start_ = millis()/1000;
    active_ = 0;
  }

  void UpdateAccumulation(){
    accumulators[active_] += millis()/1000 - active_start_;
    Serial.print(accumulators[active_]);
  }

  void SetActive(int n){
    UpdateAccumulation();
    active_ = n;
    active_start_ = millis()/1000;
  };

  int GetActive(){
    return active_;
  }

  time_t GetAccumulatedTime() {
    return accumulators[active_] + (millis()/1000 - active_start_);
  }

 private:
  time_t* accumulators;
  int active_;
  int active_start_;
};

#endif
