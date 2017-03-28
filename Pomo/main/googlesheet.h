#include <HTTPSRedirect.h>

const char *GScriptId = "AKfycbyHwjxAg8eJwlVwt71-o94I9M4sAzgAsg2xwnmb4El9QtzjC77u";

const char* host = "script.google.com"; 
const char* googleRedirHost = "script.googleusercontent.com";

const int httpsPort =     443; 
HTTPSRedirect client(httpsPort);

// Prepare the url (without the varying data) 
String url = String("/macros/s/") + GScriptId + "/exec?";

const char* fingerprint = "F0 5C 74 77 3F 6B 25 D7 3B 66 4D 43 2F 7E BC 5B E9 28 86 AD";

void setupClient() {
  bool flag = false; 
    for (int i=0; i<5; i++){ 
            int retval = client.connect(host, httpsPort); 
            if (retval == 1) { 
                        flag = true; 
                        break; 
            } 
            else 
                    Serial.println("Connection failed. Retrying…"); 
    }

    // Connection Status, 1 = Connected, 0 is not. 
    Serial.println("Connection Status: " + String(client.connected())); 
    Serial.flush(); 
     
    if (!flag){ 
            Serial.print("Could not connect to server: "); 
            Serial.println(host); 
            Serial.println("Exiting…"); 
            Serial.flush(); 
            return; 
    }

    // Data will still be pushed even certification don’t match. 
    if (client.verify(fingerprint, host)) { 
            Serial.println("Certificate match."); 
    } else { 
            Serial.println("Certificate mis-match"); 
    } 
}

// This is the main method where data gets pushed to the Google sheet 
void postData(){ 
    Serial.print("start post");
    if (!client.connected()){ 
            Serial.println("Connecting to client again…"); 
            client.connect(host, httpsPort); 
    } 
    String urlFinal = url; //+ “tag=” + tag + “&value=” + String(value); 
    client.printRedir(urlFinal, host, googleRedirHost); 
    Serial.print("test");
}
