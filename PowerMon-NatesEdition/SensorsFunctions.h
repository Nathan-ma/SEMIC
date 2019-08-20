extern String URL;

int v_SampleTime,v_RptTime,v_RptAccum;
int v_frac,v_Voltage;

//  \-- *** ARRAYS *** --/

int CT_Amps[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};    // CT Amps read from A0 -> A15
int CT_Accum[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};   // Wh accumulated over time period
int CT_TTL[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};     // Wh values ready to send
int val_pin[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};    // 
int scl_val[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

// Start_Column[] is the text name of the column where Unit[i] data begins 
// cell 1 is the unit # (up to 20) and cells 2 through 17 are for 16 CTs of watt-hours data/
// "*" prefix indicates that this is an arrar of arrays, because some items are multiple characters
// Target cell/block to paste data is being sent from Arduino to simplify s/s programming

char *Start_Column[20] = {"D","U","AL","BC","BT","CK","DB","DS","EJ","FA","FR","GI","GZ","HQ","IH","IY","JP","KG","KX","LO"};
char *End_Column[20] = {"T","AK","BB","BS","CJ","DA","DR","EI","EZ","FQ","GH","GY","HP","IG","IX","JO","KF","KW","LN","ME"};
int Start_Col;

// Month Row Starts Arrays
// Array "Row_Start" is for non-leap years
// Array "Row_Start_L" is for leap year
// Index [i] is Month # from v_Month

 int Row_Start[12] = {2,746,1418,2162,2882,3626,4346,5090,5834,6554,7298,8018};
 int Row_Start_L[12] = {2,746,1442,2186,2906,3650,4370,5114,5858,6578,7322,8042};
 
bool Step_Accum = true;  // Accumulate Kwh during SampleTime values in array CT_Accum
bool Step_Xfer = false;  // Report Time is reached: Transfer values in array CT_Accum to CT_Total
bool Step_Send = false;  // Build send string from CT_Total, send URL to target GSheet
bool Leap_Year = false;  // Finding row # in appropriate Row_Start(_L) array
int v_RowNum = 0;            // initial value of RowNum

 // Data Unique to Board/Client
int const Board_Num = 1;  // The CT # of this exact board

void buildSensors();
void readPins();
void transferToArray();

//Function - Set up all the sensors
void buildSensors(){
  v_SampleTime = 1; // Sample period in minutes
  v_RptTime = 60;    // Reporting Period in minutes
  v_RptAccum = 0;   // Accumuluted Minutes in Reporting Period
  v_frac = 60;    // Fraction of hour Wh Accumulated in sample (05-31 : 1 minute)
  v_Voltage = 120;  // Hard-Wired AC Voltage
  
}// ENDFunction

// FREEEEED - Why is URL been filled with chars here?
//Function - read pin values, scale them 
void readPins(int v_Ratio){
  
   for(int i = 0;i <= 15; i++) {
    // val_pin[i] = analogRead(myPin[i]);
    scl_val[i] = map(val_pin[i],0,1023,0,50);
   }

   URL = "CT_ACCUM: ";

   for(int i = 0;i <= 15; i++) {
      CT_Amps[i] = scl_val[i];
    //  Serial.println("CT_Amps[" + string([i])+]: " + String(CT_Amps[i]) + "Scl_Val[i]:  " + String(scl_val[i]));
      CT_Accum[i] = CT_Accum[i] + (CT_Amps[i] * v_Ratio);
      URL += String(CT_Accum[i]) + " ";         
   }

   URL = "";
  v_Min = v_Min_Cur;     // reset vMin to the current minute
  v_RptAccum++;        // increment Report Mins accumulated every time we accum values
}//ENDFunction


//Function - Transfer collected Values to their array
void transferToArray(){
  URL = "CT_TTL: ";
      for(int i = 0;i <= 15; i++) {
          CT_TTL[i] = CT_Accum[i]; // this fills TTL Array that will be sent to GSheets
          CT_Accum[i] = 0;  //clear out CT_accum ...
          URL += String (CT_TTL[i]) + " ";         
     }
      // Serial.println(URL);
      URL = "";
    v_RptAccum = 0;     // reset accumulated minutes to Report Mins to zero
    
    // --- STEP 3  BUILD URL, SEND IT TO GSHEETS ---  Code from original Sketch goes here
    String URL = "";

    // CALCULATE COL FOR UNIT
    Start_Col = 4 + ((Board_Num-1) * 17);  // Board_Num is BOARD ID for this Client/Location
                                           // Offset of 3 rows at the start (Mo/Da/Hr so start at Col 4
                                           // Each Board sends 17 data bits, Board ID + 16 Watt-Hr Values
                                           
    URL += logURL + "?COL=" + String(Start_Col);   

    // Serial.println("MONTH: " + String(v_Month) + "DAY: " + String(v_Day_Cur) + "HOUR: " + String(v_Hr));
    
    // Calculate ROW # FOR THIS SEND (based on Date/Time)
    v_Remainder = v_Year % 4;
    Leap_Year = v_Remainder == 0;
    int v_Index = (v_Month - 1);
    if(Leap_Year == true) {
      v_RowNum = Row_Start_L[v_Index] + ((v_Day_Cur - 1)* 24) + (v_Hr - 1);
    }
    else {
      v_RowNum = Row_Start[v_Index] + ((v_Day_Cur - 1)* 24) + (v_Hr - 1);
    } 
     
    URL += "&ROW=" + String(v_RowNum);   // Row  target
    URL += "&UNIT=" + String (Board_Num); // Board Number for This Client&Location
    
     for(int i = 0;i <= 15; i++) {
          URL += "&VAL="+String(CT_TTL[i],DEC); // this fills TTL Array that will be sent to GSheets       
     }
}//EndFunction
