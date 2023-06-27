#define pinRelay D4
#define pinLDR A0

bool isPhase1, isPhase2, isPhase3, isPhase4;
String phase1 = "1", phase2 = "2", phase3 = "3", phase4 = 4;
int phase1_duration, phase2_duration, phase3_duration, phase4_duration;
unsigned long myTimer = millis();

long int  ph2_val;  // Light measurement for Phase 2. "Long" for case of long duration of Phase 2.
double    ph2_avg;  // Average of light measurements for Phase 2
int       ph2_cnt;  // Amount of light measurements for Phase 2
bool      ph2_bool = true;
int       ph3_output; // PWM value provided on Phase 3

void setup() {
  pinMode(pinRelay, OUTPUT);  // Relay control pin
  Serial.begin(9600);
  Serial.println("Started ...");

  wifi_Setup();
}

void loop() {
  if (isPhase1) {
    phase1_duration = GetDuration(phase1); 

    digitalWrite(pinRelay, LOW);  // Turn the relay on
    MovePhase(phase1_duration, isPhase1, isPhase2);
  }
  else {
    digitalWrite(pinRelay, HIGH); // Turn the relay off
  }

  if (isPhase2) {
    phase2_duration = GetDuration(phase2);

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

    MovePhase(phase2_duration, isPhase2, isPhase3);
  }

  if (isPhase3) {
    phase3_duration = GetDuration(phase3); 
    ph3_output = map(ph2_avg, 0, 1023, 0, 255); // Provide a PWM value for external device

    MovePhase(phase3_duration, isPhase3, isPhase4);
  }

  if (isPhase4) {
    phase4_duration = GetDuration(phase4); 

    // Reset all the variables before next cicle begins:
    ph2_val = 0;
    ph2_cnt = 0;
    ph2_avg = 0;
    ph2_bool = true;
    ph3_output = 0;

    MovePhase(phase4_duration, isPhase4, isPhase1);
  }
}

void MovePhase(int& duration, bool& current, bool& next){
  if (millis() - myTimer >= (duration * 1000) {
    current = false;
    next = true;
    myTimer = millis();
  }
}
