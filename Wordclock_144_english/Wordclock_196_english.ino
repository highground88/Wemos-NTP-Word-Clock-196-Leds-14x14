#include <NTPtimeESP.h>
//NTPtime NTPch("pool.ntp.org");   // Choose server pool as required
NTPtime NTPch("uk.pool.ntp.org");   // Choose server pool as required
strDateTime dateTime;
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiManager.h>   
#include <DNSServer.h>
#include <Adafruit_NeoPixel.h>
#define PIN            D5
#define NUMPIXELS     197

int passFlag = 0;  //For stuck 'Twelve / Oclock' reset loop.

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;

//change update values if debugging is on to make debugging quicker
const unsigned long period = 10000;
//change to 0 for debugging 1 is normal operation 
int debugging = 1;                                 



Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
 
byte second, minute, hour, dayOfWeek, day, month, year;  
byte decToBcd(byte val)
{
  return ( (val / 10 * 16) + (val % 10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return ( (val / 16 * 10) + (val % 16) );
}


//Descriptors
int WordIts[] = {187,186,185, -1};
int WordItIs[] = {187,186,183,182, -1};
int WordIt[] = {187,186, -1};
int WordIs[] = {183,182, -1};
int WordItjustS[] = {185, -1}; //Just the ['S]
int WordAbout[] = {197, -1};
int WordMinute[] = {83,82,81,80,79,78, -1};
int WordMinutes[] = {83,82,81,80,79,78,77, -1};
int WordMinSonly[] = {77, -1};
int WordMinuteTo[] = {83,82,81,80,79,78,73,72, -1};
int WordMinutesTo[] = {83,82,81,80,79,78,77,73,72, -1};
int WordTo[] = {73,72, -1};
int Wordo[] = {72, -1};  //Fix weird TO bug after oclock.
int WordA[] = {173, -1};
int WordPast[] = {76,75,74,73, -1};
int WordPas[] = {76,75,74, -1};
int WordOclock[] = {8,9,10,11,12,13, -1};

//Minutes [UPPER]
int WordOne[] = {151,152,153, -1};
int Wordne[] = {152,153, -1}; //FIX faulty TWO
int WordTwo[] = {149,150,151, -1};
int WordTw[] = {149,150, -1};  //FIX faulty ONE
int WordThree[] = {84,85,86,87,88, -1};
int WordFour[] = {112,113,114,115, -1};
int WordFive[] = {122,123,124,125, -1};
int WordSix[] = {139,138,137, -1};
int WordSeven[] = {140,141,142,143,144, -1};
int WordEight[] = {88,89,90,91,92, -1};
int WordNine[] = {133,132,131,130, -1};
int WordNin[] = {133,132,131,130, -1};  //FIX TEN
int WordTen[] = {149,130,121, -1};
int WordTn[] = {149,121, -1};  //FIX NINE
int WordEleven[] = {106,105,104,103,102,101, -1};
int Wordleven[] = {105,104,103,102,101, -1};  //FIX TWELVE
int WordTwelve[] = {111,110,109,108,107,106, -1};
int WordTwelv[] = {111,110,109,108,107, -1};
int WordThirteen[] = {161,160,159,158,157,156,155,154, -1};
int WordFourteen[] = {112,113,114,115,116,117,118,119, -1};
int WordQuarter[] = {175,176,177,178,179,180,181, -1};
int WordSixteen[] = {139,138,137,136,135,134,133, -1};
int WordSeventeen[] = {140,141,142,143,144,145,146,147,148, -1};
int WordEighteen[] = {88,89,90,91,92,93,94,95, -1};
int WordNineteen[] = {133,132,131,130,129,128,127,126, -1};
int WordTwenty[] = {167,166,165,164,163,162, -1};
int WordHalf[] = {168,169,170,171, -1};

//Hours [LOWER]
int WordHOne[] = {58,59,60, -1};
int WordHne[] = {59,60, -1}; //FIX TWO
int WordHTwo[] = {56,57,58, -1};
int WordHTw[] = {56,57, -1}; //FIX ONE
int WordHThree[] = {99,96,71,68,43, -1};
int WordHFour[] = {33,34,35,36, -1};
int WordHFive[] = {55,54,53,52, -1};
int WordHSix[] = {98,97,70, -1};
int WordHSeven[] = {37,38,39,40,41, -1};
int WordHEight[] = {52,51,50,49,48, -1};
int WordHNine[] = {65,66,67,68, -1};
int WordHTen[] = {69,42,41, -1};
int WordHEleven[] = {60,61,62,63,64,65, -1};
int WordHTwelve[] = {48,47,46,45,44,43, -1};
int WordHTwelv[] = {48,47,46,45,44, -1};   // FIX leftover TWELVE to prevent cross with THREE
int WordHWelve[] = {47,46,45,44,43, -1};   // FIX leftover TWELVE to prevent cross with EIGHT
//BONUS WORDS
int WordCoffee[] = {23,22,21,20,19,18, -1};
int WordTea[] = {17,16,15, -1};
int WordBeer[] = {27,26,25,24, -1};
int WordJo[] = {195,194, -1};
int WordJoanne[] = {195,194,193,192,191,190, -1};
int Wordanne[] = {193,192,191,190, -1};
int WordJoannes[] = {195,194,193,192,191,190,189 -1};
int WordAt[] = {175,174, -1};
int WordIn[] = {120,121, -1};
int WordNot[] = {101,100,99, -1};
int WordHappy[] = {28,29,30,31,32, -1};
int WordBirthday[] = {0,1,2,3,4,5,6,7, -1};
int WordHappyBirthday[] = {0,1,2,3,4,5,6,7,28,29,30,31,32, -1};
int WordQuestion[] = {14, -1};
int WordWifi[] = {110,54,122,66,14, -1};

int WordAllLetters[] = {0 - 197, -1};



int flag = 0; //used for display effects to stop it showing more than once

//define colours
uint32_t Black = pixels.Color(0, 0, 0);
uint32_t White = pixels.Color(255, 255, 255);
uint32_t Green = pixels.Color(0, 255, 0);
uint32_t Red = pixels.Color(255, 0, 0);
uint32_t Red3 = pixels.Color(235, 0, 0);
uint32_t Red2 = pixels.Color(215, 0, 0);
uint32_t Red1 = pixels.Color(205, 0, 0);
uint32_t Gold = pixels.Color(255, 204, 0);
uint32_t Grey = pixels.Color(30, 30, 30);
uint32_t Blue = pixels.Color(0, 0, 255);
uint32_t Brown = pixels.Color(153, 102, 051);
uint32_t whiteblue = pixels.Color(255, 255, 255);
uint32_t lightblue = pixels.Color(153, 204, 255);
uint32_t midblue = pixels.Color(0, 102, 204);
uint32_t darkblue = pixels.Color(0, 0, 255);
uint32_t Pink = pixels.Color(255, 153, 153);
uint32_t Turquoise = pixels.Color(0, 213, 255);
uint32_t Purple = pixels.Color(255, 0, 205);
uint32_t Orange = pixels.Color(255, 100, 0);
uint32_t Yellow = pixels.Color(255, 247, 0);





//values for brightness
int dayBrightness = 100;
int nightBrightness = 30;

//values for debugging hour/day  i increment 
byte i;
byte h ;
byte d ;

void setup()
{
  //used for manual debugging/ set start time 
   i = 57 ;
   h = 9;
   d = 1;
//   i = 0 ;
//   h = 8;
//   d = 1;   
  //start pixels
  pixels.begin();
  blank();
  //start serial
  Serial.begin(115200);
  Serial.println();
  lightup(WordWifi, Red);
  Serial.println("System Booted");
  Serial.println("Connecting to Wi-Fi....");
// We start by connecting to a WiFi network
  WiFiManager MyWifiManager;
  MyWifiManager.autoConnect("Word Clock Setup");
  Serial.println("....WiFi connected!");
  pixels.setBrightness(dayBrightness);

  startMillis = millis();  //initial start time
  pixels.clear();
 //   blank();  //??
//Serial print if debug is on 
  if (debugging == 0)
  {
    Serial.println("Debug on");
  }
  else {
    Serial.println("Debug off");
 //   test(); //run basic screen tests    /////CHANGED (NO TEST)

    }

}

void loop()
{
  //pixels.clear();
  TimeOfDay(); //set brightness dependant on time of day
  displayTime(); // display the real-time clock data on the Serial Monitor  and the LEDS,

  //home time serial for debugging if statements
  if ((dayOfWeek != 6) && (dayOfWeek != 7)) {  //WEEKDAYS & SUN
    if ((hour > 5) && (hour < 10)) {
      lightup(WordQuestion, Brown);
      lightup(WordCoffee, Brown);
      Serial.println("coffee time");
    }
    else if ((hour == 10) && (minute == 0)) {
      lightup(WordQuestion, Black);
      lightup(WordCoffee, Black);
      Serial.println("clear the drinks");
    }
    else if (hour == 15) {
      lightup(WordTea, Green);
      lightup(WordQuestion, Green);
      Serial.println("tea time");
    }
    else if ((hour == 16)  && (minute == 0)) {

      lightup(WordTea, Black);
      lightup(WordQuestion, Black);
      Serial.println("clear the drinks");
    }
  }
  
  if ((dayOfWeek == 6) && (dayOfWeek == 7)) {    /// FRI,SAT
    if ((hour > 5) && (hour < 10)) {
      lightup(WordQuestion, Brown);
      lightup(WordCoffee, Brown);
      Serial.println("coffee time");
    }
    else if ((hour == 10) && (minute == 0)) { 
      lightup(WordQuestion, Black);
      lightup(WordCoffee, Black);
      Serial.println("clear the drinks");
    }
    else if (hour == 15) {
      lightup(WordTea, Green);
      lightup(WordQuestion, Green);
      Serial.println("tea time");
    }
    else if ((hour == 16) && (minute == 0)) {

      lightup(WordTea, Black);
      lightup(WordQuestion, Black);
      Serial.println("clear the drinks");
    }
    else if ((hour > 16) && (hour < 21)) {
      lightup(WordBeer, Gold);
      lightup(WordQuestion, Gold);

      Serial.println("beer time");
    }
    else if ((hour == 22) && (minute == 0)) {
      lightup(WordBeer, Black);
      lightup(WordQuestion, Black);
      Serial.println("clear the drinks");
    }
  }

 if ((day == 1) && (month == 2)) {  // BIRTHDAY (01 FEB)
      lightup(WordHappy, Purple);
      delay(150);
      lightup(WordBirthday, Orange);
      delay(5000);
      lightup(WordHappy, Orange);
      delay(150);
      lightup(WordBirthday, Purple);
      Serial.println("bday!");
    }
    else if ((day != 12) && (month != 2)) {
      lightup(WordHappy, Black);
      lightup(WordBirthday, Black);
    }


  if ((minute == 0) 
      | (minute == 15)
      | (minute == 30)
      | (minute == 45)) {
    lightup(WordIts, Blue);
    lightup(WordIs, Black);
    lightup(WordJoanne, Pink);
  }
  else {
      lightup(WordItjustS, Black);
      lightup(WordItIs, midblue);
      lightup(Wordanne, Black);
      lightup(WordJo, Red);
    }


//// NEW YEARS EVE COUNTDOWN
//   if ( (day == 12) && (month == 2) && (minute == 36) && (hour == 7) && (second >= 45) ) {
//      Serial.print("NYE1");
//      delay(1000);      
//      lightup(WordHTen, Purple);   
//      delay(1000);
//      lightup(WordHNine, Yellow);   
//      lightup(WordHTen, Black);         
//      delay(1000);
//      lightup(WordHEight, Turquoise);   
//      lightup(WordHNine, Black);   
//      delay(1000);
//      lightup(WordHSeven, Blue);   
//      lightup(WordHEight, Black);  
//      delay(1000);
//      lightup(WordHSix, Pink); 
//      lightup(WordHSeven, Black);    
//      delay(1000);
//      lightup(WordHFive, Orange); 
//      lightup(WordHSix, Black);  
//      delay(1000);
//      lightup(WordHFour, Green);  
//      lightup(WordHFive, Black);  
//      delay(1000);
//      lightup(WordHThree, Red); 
//      lightup(WordHFour, Black);   
//      delay(1000);
//      lightup(WordHTwo, Turquoise);  
//      lightup(WordHThree, Black);  
//      delay(1000);
//      lightup(WordHOne, Yellow); 
//      lightup(WordHTw, Black);  
//      delay(1000);
//      lightup(WordHOne, Black);   
//      theaterChaseRainbow(50);     
//      }


//// NEW 12 O'CLOCK INITAL WIPE FIX LOOP ROUTINE
  if ((minute != 0) && (passFlag <= 3))  {   // Correct time check counter
      passFlag++; 
      Serial.println("NTP time received. Flag set.");
   }

  if (passFlag <= 3) {
      lightup(WordHTwelve, Black);
      lightup(WordOclock, Black);
      theaterChaseRainbow(98);      
      pixels.clear();

      Serial.println("12 O'Clock wipe routine run");
      Serial.print("Flag #");
      Serial.print(passFlag)  ;
      Serial.println(" of 3");}

   
  
   
  if (minute <= 30) {
    //Set hour if minutes are less than 30 (current hour remains)
    switch (hour) {
      case 1:
      case 13:
        lightup(WordHOne, White);
        lightup(WordHTwelve, Black);
        Serial.println("Break 113");
        break;
      case 2:
      case 14:
        lightup(WordHne, Black);
        lightup(WordHTwo, White);
        Serial.println(" Break 214");
        break;
      case 3:
      case 15:
        lightup(WordHTwo, Black);
        lightup(WordHThree, White);
        Serial.println("Break 315");
        break;
      case 4:
      case 16:

        lightup(WordHThree, Black);
        lightup(WordHFour, White);
        Serial.println("Break 416");
        break;
      case 5:
      case 17:
        lightup(WordHFour, Black);
        lightup(WordHFive, White);
        Serial.println("Break 517");
        break;
      case 6:
      case 18:
        lightup(WordHFive, Black);
        lightup(WordHSix, White);
        Serial.println("Break 618");
        break;
      case 7:
      case 19:
        lightup(WordHSix, Black);
        lightup(WordHSeven, White);
        Serial.println("Break 719");
        break;
      case 8:
      case 20:
        lightup(WordHSeven, Black);
        lightup(WordHEight, White);
        Serial.println("Break 820");
        break;
      case 9:
      case 21:
        lightup(WordHEight, Black);
        lightup(WordHNine, White);
        Serial.println("Break 921");
        break;
      case 10:
      case 22:
        lightup(WordHNine,Black);
        lightup(WordHTen, White);
        Serial.println("Break 1022");
        break;
      case 11:
      case 23:
        lightup(WordHTen, Black);
        lightup(WordHEleven, White);
        Serial.println("Break 1123");
        break;
      case 00:
      case 12:
        lightup(WordHEleven, Black);
        lightup(WordHTwelve, White);
        Serial.println("Break 0012");
        break;
    }// end of case statement
  }//end of if statement

  else if (minute >= 31) {
    //Set hour if minutes are greater than 34, bump hour up
    switch (hour) {
      case 1:
      case 13:
        lightup(WordHne, Black);
        lightup(WordHTwo, White);
        Serial.println("Half Break 113");
        break;
      case 2:
      case 14:
        lightup(WordHTwo, Black);
        lightup(WordHThree, White);
        Serial.println("Half Break 214");        
        break;
      case 3:
      case 15:
        lightup(WordHThree, Black);
        lightup(WordHFour, White);
        Serial.println("Half Break 315");
        break;
      case 4:
      case 16:
        lightup(WordHFour, Black);
        lightup(WordHFive, White);
        Serial.println("Half Break 416");        
        break;
      case 5:
      case 17:
        lightup(WordHFive, Black);
        lightup(WordHSix, White);
        Serial.println("Half Break 517 ");
        break;
      case 6:
      case 18:
        lightup(WordHSix, Black);
        lightup(WordHSeven, White);
        Serial.println("Half Break 618");
        break;
      case 7:
      case 19:
        lightup(WordHSeven, Black);
        lightup(WordHWelve, Black);  
        lightup(WordHEight, White);       // FIX leftover TWELVE
        Serial.println("Half Break 719");
        break;
      case 8:
      case 20:
        lightup(WordHEight, Black);
        lightup(WordHNine, White);
        Serial.println("Half Break 820");
        break;
      case 9:
      case 21:
        lightup(WordHNine,Black);
        lightup(WordHTen, White);
        Serial.println("Half Break 921");
        break;
      case 10:
      case 22:
        lightup(WordHTen, Black);
        lightup(WordHEleven, White);
        Serial.println("Half Break 1022");
        break;
      case 11:
      case 23:
        lightup(WordHEleven, Black);
        lightup(WordHTwelve, White);
        Serial.println("Half Break 1123");
        break;
      case 00:
      case 12:
        lightup(WordHOne, White);
        lightup(WordHTwelve, Black);
        Serial.println("Half Break 0012");
        break;
    }// end of case statement
  } // end of if statement to test for greater than 30


/// MINUTES
        
  if (minute == 0) {
    lightup(WordOne, Black);
    lightup(WordTo, Black);
    lightup(WordMinute, Black); 
    lightup(WordPast, Black); 
    lightup(WordOclock, darkblue);      
  }
  else if (minute == 1) {
    lightup(Wordo, Black);
    lightup(WordOne, White);
    lightup(WordMinute, White); 
    lightup(WordPast, White); 
    lightup(WordOclock, Black);

  }
  else if (minute == 2) {
    lightup(WordTwo, White);
    lightup(Wordne, Black);
    lightup(WordPast, White); 
    lightup(WordMinute, Black); 
  }
  else if (minute == 3) {
    lightup(WordTwo, Black);
    lightup(WordThree, White);
    lightup(WordPast, White); 
  }
  else if (minute == 4) {
    lightup(WordThree, Black);
    lightup(WordFour, White);
    lightup(WordPast, White); 
  }
  else if (minute == 5) {
    lightup(WordFour, Black);
    lightup(WordFive, White);
    lightup(WordPast, White); 
  }
  else if (minute == 6) {
    lightup(WordFive, Black);
    lightup(WordSix, White);
    lightup(WordPast, White); 

  }
  else if (minute == 7) {
    lightup(WordSix, Black);
    lightup(WordSeven, White);
    lightup(WordPast, White); 

  }
  else if (minute == 8) {
    lightup(WordSeven, Black);
    lightup(WordEight, White);
    lightup(WordPast, White); 
  }
  else if (minute == 9) {
    lightup(WordEight, Black);
    lightup(WordNine, White);
    lightup(WordPast, White); 
  }
  else if (minute == 10) {
    lightup(WordNin, Black);
    lightup(WordTen, White);
    lightup(WordPast, White); 
  }
  else if (minute == 11) {
    lightup(WordTen, Black);
    lightup(WordEleven, White);
    lightup(WordPast, White);  
  }
  else if (minute == 12) {
    lightup(Wordleven, Black);
    lightup(WordTwelve, White);
    lightup(WordPast, White); 
  }
  else if (minute == 13) {
    lightup(WordTwelve, Black);
    lightup(WordThirteen, White);
    lightup(WordPast, White); 
  }
  else if (minute == 14) {
    lightup(WordThirteen, Black);
    lightup(WordFourteen, White);
    lightup(WordPast, White); 
  }
  else if (minute == 15) {
    lightup(WordFourteen, Black);
    lightup(WordQuarter, White);
    lightup(WordPast, White); 
    lightup(WordA, White);
  }
  else if (minute == 16) {
    lightup(WordQuarter, Black);
    lightup(WordSixteen, White);
    lightup(WordPast, White); 
    lightup(WordA, Black);
  }
  else if (minute == 17) {
    lightup(WordSixteen, Black);
    lightup(WordSeventeen, White);
    lightup(WordPast, White); 
  }
  else if (minute == 18) {
    lightup(WordSeventeen, Black);
    lightup(WordEighteen, White);
    lightup(WordPast, White); 
  }
  else if (minute == 19) {
    lightup(WordEighteen, Black);
    lightup(WordNineteen, White);
    lightup(WordPast, White); 
  }
  else if (minute == 20) {
    lightup(WordNineteen, Black);
    lightup(WordTwenty, White);
    lightup(WordPast, White); 
  }
  else if (minute == 21) {
    lightup(WordOne, White);
    lightup(WordTwenty, White);
    lightup(WordPast, White); 
  }
  else if (minute == 22) {
    lightup(Wordne, Black);
    lightup(WordTwo, White);
    lightup(WordTwenty, White); 
    lightup(WordPast, White);        
  }
  else if (minute == 23) {
    lightup(WordTwo, Black);
    lightup(WordThree, White);
    lightup(WordTwenty, White);
    lightup(WordPast, White); 
  }
  else if (minute == 24) {
    lightup(WordThree, Black);
    lightup(WordFour, White);
    lightup(WordTwenty, White);
    lightup(WordPast, White); 
  }
  else if (minute == 25) {
    lightup(WordFour, Black);
    lightup(WordFive, White);
    lightup(WordTwenty, White);
    lightup(WordPast, White); 
  }
  else if (minute == 26) {
    lightup(WordFive, Black);
    lightup(WordSix, White);
    lightup(WordTwenty, White);
    lightup(WordPast, White); 
  }
  else if (minute == 27) {
    lightup(WordSix, Black);
    lightup(WordSeven, White);
    lightup(WordTwenty, White);
    lightup(WordPast, White); 
  }
  else if (minute == 28) {
    lightup(WordSeven, Black);
    lightup(WordEight, White);
    lightup(WordTwenty, White);
    lightup(WordPast, White); 
  }
  else if (minute == 29) {
    lightup(WordEight, Black);
    lightup(WordNine, White);
    lightup(WordTwenty, White); 
    lightup(WordPast, White); 
  }
  else if (minute == 30) {
    lightup(WordNine, Black);
    lightup(WordTwenty, Black);
    lightup(WordHalf, White);    
    lightup(WordPast, White); 
  }
  else if (minute == 31) {
    lightup(WordNine, White);
    lightup(WordTwenty, White);
    lightup(WordHalf, Black);   
    lightup(WordTo, midblue);
    lightup(WordMinutes, White);    
    lightup(WordPas, Black); 
   }
  else if (minute == 32) {
    lightup(WordEight, White);
    lightup(WordNine, Black);
    lightup(WordTwenty, White);
    lightup(WordTo, midblue);
    lightup(WordMinutes, White);    
  }
  else if (minute == 33) {
    lightup(WordSeven, White);
    lightup(WordEight, Black);
    lightup(WordTwenty, White);
    lightup(WordTo, midblue);
    lightup(WordMinutes, White);       
  }
  else if (minute == 34) {
    lightup(WordSix, White);
    lightup(WordSeven, Black);
    lightup(WordTwenty, White);
    lightup(WordTo, midblue);
    lightup(WordMinutes, White);    
  }
  else if (minute == 35) {
    lightup(WordFive, White);
    lightup(WordSix, Black);
    lightup(WordTwenty, White);
    lightup(WordTo, midblue);
    lightup(WordMinutes, White);    
  }
  else if (minute == 36) {
    lightup(WordFour, White);
    lightup(WordFive, Black);
    lightup(WordTwenty, White);
    lightup(WordTo, midblue);
    lightup(WordMinutes, White);    
  }
  else if (minute == 37) {
    lightup(WordThree, White);
    lightup(WordFour, Black);
    lightup(WordTwenty, White);
    lightup(WordTo, midblue);
    lightup(WordMinutes, White);    
  }
  else if (minute == 38) {
    lightup(WordTwo, White);
    lightup(WordThree, Black);
    lightup(WordTwenty, White);
    lightup(WordTo, midblue);
    lightup(WordMinutes, White);    
  }
  else if (minute == 39) {
    lightup(WordOne, White);
    lightup(WordTw, Black);
    lightup(WordTwenty, White);
    lightup(WordTo, midblue);
    lightup(WordMinutes, White);    
  }
  else if (minute == 40) {
    lightup(WordOne, Black);
    lightup(WordTwenty, White);
    lightup(WordTo, midblue);
    lightup(WordMinutes, Black);    
  }
  else if (minute == 41) {
    lightup(WordNineteen, White);
    lightup(WordTwenty, Black);
    lightup(WordTo, midblue);
    lightup(WordMinutes, White);    
  }
  else if (minute == 42) {
    lightup(WordEighteen, White);
    lightup(WordNineteen, Black);
    lightup(WordTo, midblue);
    lightup(WordMinutes, White);    
  }
  else if (minute == 43) {
    lightup(WordSeventeen, White);
    lightup(WordEighteen, Black);
    lightup(WordTo, midblue);
    lightup(WordMinutes, White);    
  }
  else if (minute == 44) {
    lightup(WordSixteen, White);
    lightup(WordSeventeen, Black);
    lightup(WordTo, midblue);
    lightup(WordMinutes, White);    
  }
  else if (minute == 45) {
    lightup(WordQuarter, White);
    lightup(WordA, White);
    lightup(WordSixteen, Black);
    lightup(WordTo, midblue);
    lightup(WordMinutes, Black);    
  }
  else if (minute == 46) {
    lightup(WordFourteen, White);
    lightup(WordQuarter, Black);
    lightup(WordA, Black);
    lightup(WordTo, midblue);
    lightup(WordMinutes, White);    
  }
  else if (minute == 47) {
    lightup(WordThirteen, White);
    lightup(WordFourteen, Black);
    lightup(WordTo, midblue);
    lightup(WordMinutes, White);    
  }
  else if (minute == 48) {
    lightup(WordTwelve, White);
    lightup(WordThirteen, Black);
    lightup(WordTo, midblue);
    lightup(WordMinutes, White); 
  }
  else if (minute == 49) {
    lightup(WordEleven, White);
    lightup(WordTwelv, Black);
    lightup(WordTo, midblue);
    lightup(WordMinutes, White);    
  }
  else if (minute == 50) {
    lightup(WordTen, White);
    lightup(WordEleven, Black);
    lightup(WordTo, midblue);
     lightup(WordMinutes, Black);    
  }
  else if (minute == 51) {
    lightup(WordNine, White);
    lightup(WordTn, Black);
    lightup(WordTo, midblue);
    lightup(WordMinutes, White);    
  }
  else if (minute == 52) {
    lightup(WordEight, White);
    lightup(WordNine, Black);
    lightup(WordTo, midblue);
    lightup(WordMinutes, White);    
  }
  else if (minute == 53) {
    lightup(WordSeven, White);
    lightup(WordEight, Black);
    lightup(WordTo, midblue);
    lightup(WordMinutes, White);    
  }
  else if (minute == 54) {
    lightup(WordSix, White);
    lightup(WordSeven, Black);
    lightup(WordTo, midblue);
    lightup(WordMinutes, White);    
  }
  else if (minute == 55) {
    lightup(WordFive, White);
    lightup(WordSix, Black);
    lightup(WordTo, midblue);
    lightup(WordMinutes, Black);  
  }
  else if (minute == 56) {
    lightup(WordFour, White);
    lightup(WordFive, Black);
    lightup(WordTo, midblue);
    lightup(WordMinutes, White);    
  }
  else if (minute == 57) {
    lightup(WordThree, White);
    lightup(WordFour, Black);
    lightup(WordTo, midblue);
    lightup(WordMinutes, White);    
  }
  else if (minute == 58) {
    lightup(WordTwo, White);
    lightup(WordThree, Black);
    lightup(WordTo, midblue);
    lightup(WordMinutes, White);    
  }
  else if (minute == 59) {
    lightup(WordOne, White);
    lightup(WordTw, Black);
    lightup(WordTo, midblue);
    lightup(WordMinSonly, Black);  
    lightup(WordMinute, White);    
  }
  
//  // Fill along the length of the strip in various colors...
//  colorWipe(pixels.Color(255,   0,   0), 50); // Red
//  colorWipe(pixels.Color(  0, 255,   0), 50); // Green
//  colorWipe(pixels.Color(  0,   0, 255), 50); // Blue
//
//  // Do a theater marquee effect in various colors...
//  theaterChase(pixels.Color(127, 127, 127), 50); // White, half brightness
//  theaterChase(pixels.Color(127,   0,   0), 50); // Red, half brightness
//  theaterChase(pixels.Color(  0,   0, 127), 50); // Blue, half brightness

  //rainbow(10);             // Flowing rainbow cycle along the whole strip
  //theaterChaseRainbow(50); // Rainbow-enhanced theaterChase variant
}


// Some functions of our own for creating animated effects -----------------

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<pixels.numPixels(); i++) { // For each pixel in strip...
    pixels.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    pixels.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      pixels.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<pixels.numPixels(); c += 3) {
        pixels.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      pixels.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<pixels.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (pixels.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / pixels.numPixels());
      // pixels.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through pixels.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue)));
    }
    pixels.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      pixels.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of pixels in increments of 3...
      for(int c=b; c<pixels.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / pixels.numPixels();
        uint32_t color = pixels.gamma32(pixels.ColorHSV(hue)); // hue -> RGB
        pixels.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      pixels.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}

void TimeOfDay() {
  //Used to set brightness dependant of time of day - lights dimmed at night

  //monday to thursday and sunday

  if ((dayOfWeek == 6) | (dayOfWeek == 7)) {
    if ((hour > 0) && (hour < 8)) {
      pixels.setBrightness(nightBrightness);
    }
    else {
      pixels.setBrightness(dayBrightness);
    }
  }
  else {
    if ((hour < 6) | (hour >= 22)) {
      pixels.setBrightness(nightBrightness);
    }
    else {
      pixels.setBrightness(dayBrightness);
    }
  }
}

void displayTime()
{
    
  //rate limit the time check 
  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
    Serial.println("Time Check rate reached");
    readtime(&second, &minute, &hour, &dayOfWeek, &day, &month, &year);  
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.

  }
    Serial.print("Time: ");
        if (hour < 10) {
      Serial.print("0");
    }
    Serial.print(hour);
    Serial.print(":");
  
    if (minute < 10) {
      Serial.print("0");
    }
    Serial.print(minute);
    
    if (second < 10) {
    Serial.print("0");
    }
    Serial.print(second);
    
    Serial.print("   Day of week: ");
    Serial.println(dayOfWeek);
    Serial.print("   Date: ");
    Serial.print(day);
    Serial.print("   Month: ");
    Serial.print(month);
    Serial.print("   Day of week: ");
    Serial.println(dayOfWeek);
    delay(1000);
    


}


void readtime(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *day, byte *month, byte *year) {
  if (debugging == 1){
      dateTime = NTPch.getNTPtime(0.0, 1);
      if(dateTime.valid){
      *second = dateTime.second;
      *minute = dateTime.minute;
      *hour = dateTime.hour;
      *dayOfWeek = dateTime.dayofWeek;
      *month = dateTime.month;
      *day = dateTime.day;
      *year = dateTime.year;
        }
      }
    else {
      //  For manual time testing. you must comment out datetime.valid and ntpch to get time checking faster  
      if (i <= 59){  
        if (i == 59){
           h++;
           *hour = h;
           i = 0;
        }
        i++;
        *minute = i;
        *hour = h;
      }


      if (h >23){
        h =0;}
      
      *second = dateTime.second;
      *dayOfWeek = d;
      *month = dateTime.month;
      *year = dateTime.year;
      *day = dateTime.day;   
      }
 
}

void lightup(int Word[], uint32_t Colour) {   ///                                   ?????????????MODIFY
  
  for (int x = 0; x < pixels.numPixels() + 1; x++) {
    if (Word[x] == -1) {
      pixels.show();
      break;
    } //end of if loop
    else {
      pixels.setPixelColor(Word[x], Colour);
      pixels.show();
    } // end of else loop
  } //end of for loop
}

void blank() {
  //clear the decks
  for (int x = 0; x < NUMPIXELS; ++x) {
    pixels.setPixelColor(x, Black);
  }
  pixels.show();

}

void wipe() {

  for (int x = 0; x < NUMPIXELS; ++x) {
    pixels.setPixelColor(x, Blue);
    delay(10);
    pixels.show();
  }
  delay(30);
  for (int x = NUMPIXELS; x > -1; --x) {
    pixels.setPixelColor(x, Black);
    delay(10);
    pixels.show();
  }

  for (int x = 0; x < NUMPIXELS; ++x) {
    pixels.setPixelColor(x, Green);
    delay(10);
    pixels.show();
  }
  delay(30);
  for (int x = NUMPIXELS; x > -1; --x) {
    pixels.setPixelColor(x, Black);
    delay(10);
    pixels.show();
  }

  for (int x = 0; x < NUMPIXELS; ++x) {
    pixels.setPixelColor(x, Red);
    delay(10);
    pixels.show();
  }
  delay(30);
  for (int x = NUMPIXELS; x > -1; --x) {
    pixels.setPixelColor(x, Black);
    delay(10);
    pixels.show();
  }

  for (int x = 0; x < NUMPIXELS; x = x + 2) {
    pixels.setPixelColor(x, Red);
    delay(30);
    pixels.show();
  }
  delay(700);
  blank();

}

void test() {
  blank();
  wipe();
  blank();

}
