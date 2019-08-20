#include <Ethernet.h>
#include <EthernetClient.h>
#include <ArduinoHttpClient.h>

byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,0,3);

EthernetClient ether;
HttpClient me_client = HttpClient(ether,ip,443);  // *** My router credentials 

const String logURL = "https://script.google.com/macros/s/AKfycbxzEFd3wpJR_JkyeuFY0fz8-uV3RRyMMOsHHuycSCi6_5lKtd79/exec";
String URL = "";

//Function - Send information to Google Sheet
void sendtoGS(){
                                   // ATTN Nathan:  See comments below
    Serial.println(URL);         // *** This URL generated works fine when pasted into a browser
    me_client.get(URL);        // *** Untested
}//ENDFunction
