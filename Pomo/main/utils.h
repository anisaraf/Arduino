#ifndef UTILS_H
#define UTILS_H
#include <time.h>
#include "web.h"

namespace utils {
  // Add leading zero to string if needed
  String formatNumber(String inp){
    if (inp.length()==1) {
      return "0" + inp;
    }
    else{
      return inp;
    }
  }

  // Return current time in seconds.
  time_t getCurrentTime() {
    if(!web::start_time) {
      web::setStartTimeNTP();
    }
    return web::start_time + millis()/1000;
  }

  // return 4 digit number HHMM
  String getTimeString(time_t now) {
    struct tm *tmp = gmtime(&now);
    return formatNumber(String(tmp->tm_hour %12)) + formatNumber(String(tmp->tm_min));
  }

}

#endif
