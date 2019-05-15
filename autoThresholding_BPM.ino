void setup() {
  Serial.begin(115200);
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
int totalRange = 0;
double x = 1;
double y = 0;
unsigned long runTime;
bool stateCheck = false;
///////////////////////////////////////////////BPM Variables
unsigned long time1 =0; 
unsigned long time2 = 0; 
unsigned long period1, period2, period3, period4, periodAvg;
unsigned long BPM = 0; 
int state = 0; 
unsigned long switchCount = 1;


//BEGIN
  //WHILE first 10 seconds
    //running average
    //find max
    //find min
    //refresh ECG signal and runTime variables
  //IF stateCheck false
    //max and avg distance
    //min and avg distance
    //find largest distance
    //IF 

void loop() {
  rawECG = analogRead(A0);
  runTime = millis();

  while(runTime <= 10000) 
  {
    
    y = y + rawECG;
    avgECG = (int)(y/x);
    x++;
    
    maxECG = max(rawECG, maxECG);
    minECG = min(rawECG, minECG);
    rawECG = analogRead(A0);
    runTime = millis();
    
  }

  if(stateCheck == false) 
  {
    //totalRange = maxECG - minECG;
    maxRange = maxECG - avgECG;
    minRange = avgECG - minECG;

    if(maxRange > minRange) 
      threshold = int((avgECG + 3*maxECG)/4.0);

    else if(maxRange < minRange) 
      threshold = int((avgECG + 3*minECG)/4.0);
   
    stateCheck = true;
  }

  if(stateCheck = true) 
  {
    rawECG = analogRead(A0);
     
    if(rawECG < threshold) 
      state = 1;

      else if (state == 1) 
      { 
        if(rawECG >= threshold) 
        {
          time1 = time2;
          time2 = millis();
        }
      }

    switch(switchCount) 
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

    switchCount++;
    if(switchCount > 4) 
    switchCount = 1;

    periodAvg = (period1 + period2 + period3 + period4)/4.0;
    BPM = 60000.0/periodAvg;
    state = 0;
    Serial.println(BPM);
    //delay(20); 
    
    //Serial.print(threshold);


    Serial.print("\t");
    Serial.println(rawECG);
    delay(20);
  }
}
