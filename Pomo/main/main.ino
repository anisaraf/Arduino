
#include <TimedAction.h>
#include <time.h>
#include "web.h"
#include "utils.h"
#include "ptm.h"

TimedAction timedCheckButtons = TimedAction(200, ptm::CheckButtons);
TimedAction timedDisplayUpdate = TimedAction(1000, ptm::DisplayUpdate);

void setup(){
  Serial.begin(9600);
  Serial.print("Started");
  web::SetupOTAHandlers();
  //setupClient();
  //animateLed();
}

void loop(){
  web::CheckOTAUpdates();
  timedCheckButtons.check();
  timedDisplayUpdate.check();
}




 
