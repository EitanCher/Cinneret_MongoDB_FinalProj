#define pinRelay D4
#define pinLDR A0

//bool isPhase1, isPhase2, isPhase3, isPhase4;
//int phase1_duration, phase2_duration, phase3_duration, phase4_duration;

short NumOfPhases = 4, MinDuration = 2, CurrentPhase, NextPhase;

//For each phase: 2 parameters (on/off and duration)
//Amount is "+1" for better code-readability, ignore the zero
short MyVars = new short[NumOfPhases + 1][2];  
for(short i = 1; i <= NumOfPhases; i++) {
  myVars[i][0] = 0;  // Phase off
  myVars[i][1] = MinDuration;
}

unsigned long myTimer = millis();

long int  ph2_val;  // Light measurement for Phase 2. "Long" for case of long duration of Phase 2.
double    ph2_avg;  // Average of light measurements for Phase 2
int       ph2_cnt;  // Amount of light measurements for Phase 2
bool      ph2_bool = true;
int       ph3_output; // PWM value provided on Phase 3

void setup() {
  pinMode(pinRelay, OUTPUT);  // Relay control pin
  //isPhase1 = true;  // Enable Phase 1 on program start
  myVars[1][0] = 1;  // Enable Phase 1 on program start
  
  Serial.begin(9600);
  Serial.println("Started ...");

  wifi_Setup();
}

void loop() {
  // Phase 1 ======================================================
  if(myVars[1][0] == 1) {  
    //Derive the duration value, set current/next variables:
    PhaseSettings(1);

    //Phase staff:
    digitalWrite(pinRelay, LOW);  // Turn the relay on

    //Disable current phase, enable next phase (when time off):
    NextPhase();
  }
  else {
    digitalWrite(pinRelay, HIGH); // Turn the relay off
  }

  // Phase 2 ======================================================
  if(myVars[2][0] == 1) {  
    //Derive the duration value, set current/next variables:
    PhaseSettings(2);

    //Phase staff:
    if (ph2_bool) {
      ph2_val = ph2_val + analogRead(pinLDR); // Perform measurement, add its value to the sum of previous measurements
      ph2_avg = ph2_val / ++ph2_cnt;          // Calculate the average
      ph2_bool = false; // Avoid measurements until "true" again
    }
    else {
      if (millis() - myTimer >= 1000) {
        ph2_bool = true;    // Allow next measurement only 1 second after the previous one.
        myTimer = millis(); // Reset timer
      }
    }

    //Disable current phase, enable next phase (when time off):
    NextPhase();
  }

  // Phase 3 ======================================================
  if(myVars[3][0] == 1) {
    //Derive the duration value, set current/next variables:
    PhaseSettings(3);

    //Phase staff:
    ph3_output = map(ph2_avg, 0, 1023, 0, 255); // Provide a PWM value for external device

    //Disable current phase, enable next phase (when time off):
    NextPhase();
  }

  // Phase 4 ======================================================
  if(myVars[4][0] == 1) {
    //Derive the duration value, set current/next variables:
    PhaseSettings(4);

    //Phase staff:
    //Reset all the variables before next cicle begins:
    ph2_val = 0;
    ph2_cnt = 0;
    ph2_avg = 0;
    ph2_bool = true;
    ph3_output = 0;

    //Disable current phase, enable next phase (when time off):
    NextPhase();
  }
}

void NextPhase(){
  if (millis() - myTimer >= (myVars[CurrentPhase][1] * 1000) {
    MyVars[CurrentPhase][0] = 0;
    MyVars[NextPhase][0] = 1;
    myTimer = millis();
  }
}

void PhaseSettings(short MyPhase){
  CurrentPhase = MyPhase;
  NextPhase = ++MyPhase;
  myVars[CurrentPhase][1] = GetDuration(CurrentPhase); 
}
