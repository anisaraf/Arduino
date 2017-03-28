#ifndef GOOGLESHEET_H
#define GOOGLESHEET_H

#include <HTTPSRedirect.h>

namespace googlesheet {
  const char *GScriptId = "AKfycbyHwjxAg8eJwlVwt71-o94I9M4sAzgAsg2xwnmb4El9QtzjC77u";
  const char* host = "script.google.com"; 
  const char* googleRedirHost = "script.googleusercontent.com";
  const int httpsPort = 443; 
  HTTPSRedirect client(httpsPort);

  // Prepare the url (without the varying data) 
  String url = String("/macros/s/") + GScriptId + "/exec?";

  // This is the main method where data gets pushed to the Google sheet
  // id = sheet name
  void postData(String id, String value){ 
    Serial.print("start post");
    if (!client.connected()){ 
      Serial.println("Connecting to client again…"); 
      client.connect(host, httpsPort); 
    } 
    String urlFinal = url + "id=" + id + "&value=" + String(value); 
    client.printRedir(urlFinal, host, googleRedirHost); 
    Serial.print(urlFinal);
  }
}

#endif 
