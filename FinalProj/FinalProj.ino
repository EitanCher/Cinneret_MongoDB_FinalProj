#define pinRelay D4
#define pinSophisticatedDevice D5
#define pinLDR A0
#define NumOfPhases 4
#define MinDuration 2
const int NP = NumOfPhases + 1;

int CurrentPhase, NextPhase, tmp_cnt, tmp_dur;
unsigned long TimerPhase, TimerFollowUp, TimerStaff;
long int ph2_val;  // Light measurement for Phase 2. "Long" for case of long duration of Phase 2.
double ph2_avg;    // Average of light measurements for Phase 2
int ph2_cnt;       // Amount of light measurements for Phase 2
bool followup_bool, tmp_bool = true, ph2_bool = true;
int ph3_output;  // PWM value provided on Phase 3

//Dynamic data for each phase (instead of hard-coded variables):
//Amount of phases (NP) is "NumOfPhases + 1" - for better code-readability
//Each phase has 2 parameters: (1)on/off (2)duration
int MyVars[NP][2];

void setup() {
  pinMode(pinRelay, OUTPUT);  // Relay control pin
  pinMode(pinSophisticatedDevice, OUTPUT);  // Output device on  phase 3

  for (int i = 1; i <= NumOfPhases; i++) {
    MyVars[i][0] = 0;  // Phase off
    MyVars[i][1] = MinDuration;
  }
  
  MyVars[1][0] = 1;  // Enable Phase 1 on program start

  Serial.begin(9600);
  Serial.println("Started ...");

  wifi_Setup();
  TimerPhase = millis();
}

void loop() {
  #pragma region PHASE_1
  
  if (MyVars[1][0] == 1) {
    //Derive the duration value, set current/next variables:
    PhaseSettings(1);

    //Follow up:    
    FollowUp(1);

    //Phase staff:
    digitalWrite(pinRelay, LOW);  // Turn the relay on

    //Disable current phase, enable next phase (when time off):
    SetNextPhase();
  } 
  else {
    digitalWrite(pinRelay, HIGH);  // Turn the relay off
  }
  #pragma endregion
  #pragma region PHASE_2
  
  if (MyVars[2][0] == 1) {
    //Derive the duration value, set current/next variables:
    PhaseSettings(2);
    
    //Follow up:    
    FollowUp(2);

    //Phase staff:
    if (ph2_bool) {
      ph2_val = ph2_val + analogRead(pinLDR);  // Perform measurement, add its value to the sum of previous measurements
      ph2_avg = ph2_val / ++ph2_cnt;           // Calculate the average
      ph2_bool = false;                        // Avoid measurements until "true" again
    } 
    else {
      if (millis() - TimerStaff >= 1000) {
        ph2_bool = true;     // Allow next measurement only 1 second after the previous one.
        TimerStaff = millis();  // Reset timer
      }
    }

    //Disable current phase, enable next phase (when time off):
    SetNextPhase();
  }
  #pragma endregion
  #pragma region PHASE_3
  
  if (MyVars[3][0] == 1) {
    //Derive the duration value, set current/next variables:
    PhaseSettings(3);

    //Follow up:    
    FollowUp(3);

    //Phase staff:
    ph3_output = map(ph2_avg, 0, 1023, 0, 255);  // Provide a PWM value for external device
    analogWrite(pinSophisticatedDevice, ph3_output);  // Set the duty cycle for the "sophisticated device "

    //Disable current phase, enable next phase (when time off):
    SetNextPhase();
  }
  #pragma endregion
  #pragma region PHASE_4
  
  if (MyVars[4][0] == 1) {
    //Derive the duration value, set current/next variables:
    PhaseSettings(4);

    //Follow up:    
    FollowUp(4);

    //Phase staff:
    //Reset all the variables before next cicle begins:
    ph2_val = 0;
    ph2_cnt = 0;
    ph2_avg = 0;
    ph2_bool = true;
    ph3_output = 0;

    //Disable current phase, enable next phase (when time off):
    SetNextPhase();
  }
  #pragma endregion
}

void SetNextPhase() {
  if (millis() - TimerPhase >= (MyVars[CurrentPhase][1] * 1000)) {
    MyVars[CurrentPhase][0] = 0;
    MyVars[NextPhase][0] = 1;
    tmp_bool = true;
    tmp_cnt = 0;
  }
}

void PhaseSettings(int MyPhase) {
  if(tmp_bool) {
    tmp_bool = false;
    CurrentPhase = MyPhase;

    if(CurrentPhase == NumOfPhases)
      NextPhase = 1;
    else
      NextPhase = ++MyPhase;
    
    tmp_dur = GetDuration(CurrentPhase);
    if (tmp_dur == -1)
      MyVars[CurrentPhase][1] = MinDuration;
    else
      MyVars[CurrentPhase][1] = tmp_dur;

    TimerPhase = millis();
  }

}

void FollowUp(int MyPhase){
  if(followup_bool) {
    if(tmp_cnt == 0) 
      tmp_cnt = MyVars[MyPhase][1];
    
    Serial.print("Phase ");
    Serial.print(CurrentPhase);
    Serial.print(" --- Time left: ");
    Serial.println(tmp_cnt--);
    TimerFollowUp = millis();
    followup_bool = false;
  }

  if(millis() - TimerFollowUp >= 1000) {
    followup_bool = true;
  }
}
