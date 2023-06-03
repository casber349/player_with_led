#include <arduino-timer.h>

int red = 11;
int blue = 12;
int Buzzer = 13;

auto timer = timer_create_default();

#define ledState_idle 0
#define ledState_driveRedLed 1
#define ledState_driveBlueLed 2
#define ledState_driveBoth 3
#define ledState_offBoth 4
#define ledState_wait 5

int ledState;
int ledLoopCntCurr, ledLoopCntExpected;

bool setNextLedEngineState(void *nextState) {
    ledState = nextState;
    if ((nextState == ledState_driveRedLed) || (nextState == ledState_driveBoth)) ledLoopCntCurr++;
    if (ledLoopCntCurr >= ledLoopCntExpected){
      if (nextState == ledState_driveRedLed) {
        ledState = ledState_driveBoth;
        ledLoopCntCurr = 0;
      }
      if (nextState == ledState_driveBoth) {
        ledState = ledState_idle;
      }
    }
    return false; // to repeat the action - false to stop
}

bool startLedEngine(int loopCnt)
{
  if (ledState == ledState_idle) {
    ledLoopCntExpected = loopCnt;
    ledLoopCntCurr = 0;
    ledState = ledState_driveRedLed;  
    return true;
  }
  return false;
}

void ledEngine()
{
  switch(ledState){
    case ledState_idle:
      break;
    case ledState_driveRedLed:
      digitalWrite(blue, LOW);
      digitalWrite(red, HIGH);
      timer.in(300, setNextLedEngineState, (void *)ledState_driveBlueLed);
      ledState = ledState_wait;
      break;      
    case ledState_driveBlueLed:
      digitalWrite(blue, HIGH);
      digitalWrite(red, LOW);
      timer.in(300, setNextLedEngineState, (void *)ledState_driveRedLed);
      ledState = ledState_wait;
      break;      
    case ledState_driveBoth:
      digitalWrite(blue, HIGH);
      digitalWrite(red, HIGH);
      timer.in(100, setNextLedEngineState, (void *)ledState_offBoth);
      ledState = ledState_wait;
      break;      
    case ledState_offBoth:
      digitalWrite(blue, LOW);
      digitalWrite(red, LOW);
      timer.in(100, setNextLedEngineState, (void *)ledState_driveBoth);
      ledState = ledState_wait;
      break;      
    case ledState_wait:  
      break;
  }
}

void setup() {
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(Buzzer, OUTPUT);

  startLedEngine(10);
}

void loop() {
  timer.tick();
  ledEngine();
  // led();
  //buzzer();
}

/*
void led(){
  int i;
  for(i = 0; i < 10; i++){
    digitalWrite(blue, LOW);
    digitalWrite(red, HIGH);
    delay(300);
    digitalWrite(red, LOW);
    digitalWrite(blue, HIGH);
    delay(300);
  }
  for(i = 0; i < 10; i++){
    digitalWrite(red, HIGH);
    digitalWrite(blue, HIGH);
    delay(100);
    digitalWrite(red, LOW);
    digitalWrite(blue, LOW);
    delay(100);
  }
}

void buzzer(){
  tone(Buzzer, 262, 500);
  tone(Buzzer, 294, 500);
  tone(Buzzer, 330, 500);
  tone(Buzzer, 349, 500);
  tone(Buzzer, 392, 500);
  tone(Buzzer, 440, 500);
  tone(Buzzer, 494, 500);
  tone(Buzzer, 523, 500);
}
*/
