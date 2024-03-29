#include <Wire.h>
#include <SPI.h>
#include <EmonLib.h>
#include <DS3231.h>
#include "RTC.h"
#include "SensorsFunctions.h"
#include "WebBuilder.h"

  EnergyMonitor emon1;

/* following is the address of the Web App that is generated by the script in Sheets 
 * It is the start of the URL that will be sent to the Web App to move the CT data to Sheets
 * Items apppended to URL are :
 *   - Arduino board # (1 to 20)
 *   - Item 1 is Row # for data (####)
 *   - Item 2 is Col# where insert data items 3 thru 20 starts (string of 1 or 2 char)
 *   - Item 3 is Arduino Board #
 *   - Item 4 thru 20 are CT values in watt-hours
 */





// Char *MyPin[16] = {"A0","A1","A2","A3","A4","A5","A6","A7","A8","A9","A10","A11","A12","A13","A14","A15"};

void setup() {
  
  Serial.begin(9600);
  Wire.begin();
  Ethernet.begin(mac, ip);

  buildRTC();
  v_Hr = v_Hr_Cur;
  v_Min = v_Min_Cur;

  buildSensors();
  int v_Ratio = (v_Voltage / v_frac);  // to get around precedence calc later
}  // -- end of: void setup()

   // ------------------------------------LOOP STARTS HERE---------------------------

void loop() {

  // start by updating time right now
  readTime();
  int  v_Ratio = 2;
 
   /* IF: It's a New Minute:
    * Go Get the CT Values ... 
    * ... Calc the Kwh Equivalents and ...
    * ... add to the ACCUM Array
    */
    
   if (v_Min != v_Min_Cur) {      //if The minute changed, time to record CT->Kwh Values
    readPins(v_Ratio);             
  }


   // --- STEP 2  RECORDING OVER; TRANSFER ACC VALUES TO TTL ARRAY ---
   // --- ZERO OUT ACCUM ARRAY SO THAT ACCUM CAN START AGAIN ---------

   if (v_RptAccum == (v_RptTime)) { //Accum Time = reporting time,now get ready to send accumulated Values

     transferToArray();
     sendtoGS();
  }

}  // -- end of: void loop()
