#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// OLED display TWI address
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(-1);
#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup() {
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setTextSize(1);
  pinMode(A0, INPUT);

}
//////////////////////////////////////////////////Thresholding Variables
int rawECG = 0;
int avgECG = 0;
int maxECG = 0;
int maxRange = 0;
int minECG = 0;
int minRange = 0;
int threshold = 0;
unsigned long countAvg = 1;
unsigned long sumECG = 0;
unsigned long runTime;
bool mainAlgorithm = false;
///////////////////////////////////////////////BPM Variables
bool BPMalgorithm = false;
unsigned long time1 = 0; 
unsigned long time2 = 0; 
unsigned long period1 = 0, period2 = 0, period3 = 0, period4 = 0, periodAvg;
unsigned char switchBPM = 1;
unsigned char BPM = 0; 
///////////////////////////////////////////////Display Variables
unsigned long a = 0;
unsigned long lasta = 0;
unsigned long lastb = 0;

/************************************************************************************************************************/

void loop() {
  rawECG = analogRead(A0);
  runTime = millis();

  while(runTime <= 10000) 
  {
    
    sumECG = sumECG + rawECG;
    avgECG = (int)(sumECG/countAvg);
    countAvg++;
    
    maxECG = max(rawECG, maxECG);
    minECG = min(rawECG, minECG);
    rawECG = analogRead(A0);
    runTime = millis();
    
  }


/************************************************************************************************************************/

  if(mainAlgorithm == false) 
  {
    maxRange = maxECG - avgECG;
    minRange = avgECG - minECG;

    if(maxRange > minRange) 
      threshold = int((avgECG + 2*maxECG)/3.0);

    else if(maxRange < minRange) 
      threshold = int((avgECG + 2*minECG)/3.0);
   
    mainAlgorithm = true;
  }


/************************************************************************************************************************/

  if(mainAlgorithm = true) 
  {
    rawECG = analogRead(A0);
    display.setTextColor(WHITE);
    int b = 22-rawECG*22/650;
    Serial.println(rawECG);
    display.drawLine(lasta,lastb,a,b,WHITE);
    lastb=b;
    lasta=a;

    if(rawECG < threshold)
      BPMalgorithm = true;

    if(BPMalgorithm == true) 
    {
      if(rawECG >= threshold)
      {
        time1 = time2;
        time2 = millis();
        delay(100); //this may cause problems with display? 100ms might be too small to see at such a low resolution, but i could change to anything between [40,100]
        
        switch(switchBPM) //having the switch case inside this if structure may cause the first 4 BPM values calculated to be very low as each period initialised at 0
        {
          case 1: 
            period1 = time2 - time1;
            break;
          case 2:
            period2 = time2 - time1;
            break; 
          case 3: 
            period3 = time2 - time1; 
            break; 
          case 4:
            period4 = time2 - time1; 
            break; 
        }
        switchBPM++;
        if(switchBPM > 4) 
          switchBPM = 1;
      }

    periodAvg = (period1 + period2 + period3 + period4)/4.0;
    BPM = (unsigned char)(60000.0/periodAvg);
    
    //Serial.println(BPM); 
    //Serial.print(threshold);
    
    display.writeFillRect(0,25,128,8,BLACK);
    display.setCursor(0,25);
    display.print("BPM:");
    display.print(BPM);
    display.display();
    a++;


    //Serial.print("\t");
    //Serial.println(rawECG);
    //delay(20);
  }
}
}
