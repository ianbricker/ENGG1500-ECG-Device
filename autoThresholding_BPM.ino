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
unsigned long countAvg = 1;
unsigned long sumECG = 0;
unsigned long runTime;
bool mainAlgorithm = false;
///////////////////////////////////////////////BPM Variables
unsigned long time1 =0; 
unsigned long time2 = 0; 
unsigned long period1, period2, period3, period4, periodAvg;
unsigned long BPM = 0; 
char state = 0; 
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
    
    sumECG = sumECG + rawECG;
    avgECG = (int)(sumECG/countAvg);
    countAvg++;
    
    maxECG = max(rawECG, maxECG);
    minECG = min(rawECG, minECG);
    rawECG = analogRead(A0);
    runTime = millis();
    
  }

  if(mainAlgorithm == false) 
  {
    maxRange = maxECG - avgECG;
    minRange = avgECG - minECG;

    if(maxRange > minRange) 
      threshold = int((avgECG + 3*maxECG)/4.0);

    else if(maxRange < minRange) 
      threshold = int((avgECG + 3*minECG)/4.0);
   
    mainAlgorithm = true;
  }

  if(mainAlgorithm = true) 
  {
    rawECG = analogRead(A0);
     
    if(rawECG < threshold) 
      state = true;

      else if (state == true) 
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
