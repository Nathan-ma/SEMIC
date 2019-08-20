
#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0x00, 0x0A, 0x95, 0x9A, 0x68, 0x16 };
char server[] = "www.finance.google.com";    // name address for Google (using DNS)

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(10,0,0,22);

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;



void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);


  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  
  Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.println("GET /finance/info?q=NASDAQ:GOOG HTTP/1.1\r\n");
    client.println("Host: www.finance.google.com");
    client.println("Connection: close");
    client.println();
  } 
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

void loop()
{


String readString = String(1200);
readString = "";

  char tBuffer[1200];
  int tCount = 0;
  
  
  if (client.available()) {
 
    char c = client.read();
   Serial.print(c);
   
  /*if (tCount < 1199){        //    First attempt to create char array to store incoming client data unsuccessful
    tBuffer[tCount] = c;
    tCount++;
    tBuffer[tCount] = 0;
  }
  */
  //Client.read data appended to readString to create string that can be used for strcmp
 readString += c;
 Serial.println(readString);
 //readString displays same thing as c 
  }
  

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
Serial.print(readString);
 
    // do nothing forevermore:
    while(true);
  }
 
}
