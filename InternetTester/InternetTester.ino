
#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoHttpClient.h>

byte mac[] = { 0x00, 0x0A, 0x95, 0x9A, 0x68, 0x16 };
const char *server = "http:/www.google.nl";    // name address for Google (using DNS)

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192,168,0,10);

EthernetClient ether;
HttpClient http = HttpClient(ether, server, 80);

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

  
}

void loop()
{
  Serial.println("making GET request");
  http.get("/");

  // read the status code and body of the response
  int statusCode = http.responseStatusCode();
  String response = http.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
  Serial.println("Wait five seconds");
  delay(5000);
}
