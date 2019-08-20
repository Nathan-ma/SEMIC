// removed RTCLib.h to avoid barf "re-definition of Class DateTime in DS3231.h" error

  RTClib rtc;  // ... somehow this works even though 'RTCLib' has been removed as a librar ...

// *** clock values from RTC ***
int v_Year,v_Month,v_Day_Cur,vHr_Cur,v_Hr,v_Hr_Cur,v_Min_Cur,v_Min;
int v_Remainder; // for calculating if it's a Leap Year

void resetRTC();
void readTime();

// Function - initialize date/time variables 
void resetRTC(){
  v_Year = 0;       // Current Year
  v_Month = 0;      // Current Month
  v_Day_Cur = 0;    // Current Day
  v_Hr_Cur = 0;     // Current Hour
  v_Hr = 0;         // "Stored" Hour
  v_Min_Cur = 0;    // Current Minute
  v_Min = 0;        // "Stored" Minute
} // ENDFunction

//Function - Read Time
void readTime(){
  DateTime now = rtc.now();  // Read Time
  v_Year = now.year();
  v_Month = now.month();
  v_Day_Cur = now.day();
  v_Hr_Cur = now.hour();
  v_Min_Cur = now.minute();

}// ENDFunction

//Function - Build RTC
void buildRTC(){
  resetRTC();
  readTime();
}//ENDFunction
