//////////////////////////////////////////
// Code for demo device 
// Made for CSCI3002 @ CU Boulder
// 08/04 Robert Belter
//////////////////////////////////////////

//Note, pinout different than schematics
#define PIN_RLED 5
#define PIN_BLED 3
#define PIN_CATH 4
#define PIN_MOT  2
#define PIN_SPEKN 9
#define PIN_SPEKP 6
#define PIN_BUT   10


// Setup pins and bluetooth device serial
void setup() {
  pinMode(PIN_BUT, INPUT);
  pinMode(PIN_SPEKN, OUTPUT);
  pinMode(PIN_MOT, OUTPUT);
  pinMode(PIN_BLED, OUTPUT);
  pinMode(PIN_CATH, OUTPUT);
  pinMode(PIN_RLED, OUTPUT);
  digitalWrite(PIN_SPEKN, LOW);
  digitalWrite(PIN_MOT, LOW);
  digitalWrite(PIN_BLED, HIGH);
  digitalWrite(PIN_CATH, HIGH);
  digitalWrite(PIN_RLED, HIGH);
  Serial.begin(57600);
}

// the loop function runs over and over again forever
void loop() {
  //A bunch of ugly flags
  static unsigned long internal = millis();
  static bool button_double_press_flag = false;
  static unsigned long button_pressed_at = 0;
  static bool panic = false;
  static bool real_panic = false;

  //Every second tell the phone we are doing good
  if(millis() - internal > 1000){
    Serial.println("AOK");
    internal = millis();
  }

  //If the button has been pressed, start the timeout sequence and vibration
  //The actual way to activate it would be configurable , but for demo, single button press
  if(digitalRead(PIN_BUT) == LOW && !panic){
    panic = true;
    button_pressed_at = millis();
    button_double_press_flag = true;
    digitalWrite(PIN_MOT, HIGH);
  }

  //Wait until the button is release to enable the cancel
  if(panic && digitalRead(PIN_BUT) == HIGH){
    button_double_press_flag = false;
  }

  //Cancel feature
  if(panic && !button_double_press_flag && digitalRead(10) == LOW){
    //double pressed button, cancel panic
    panic = false;
    digitalWrite(PIN_MOT, LOW);
    
    //wait for button release
    while(digitalRead(PIN_BUT) == LOW);
    delay(20);
  }

  //Go into a while loop if were are supposed to PANIC, and do our thing
  if(panic && (millis() - button_pressed_at) > 10000){
    digitalWrite(PIN_MOT, LOW);
    while(true){
      tone(PIN_SPEKP, 10000);
      digitalWrite(PIN_RLED, HIGH);
      digitalWrite(PIN_BLED, LOW);
      delay(500);
      Serial.println("PANIC");
      digitalWrite(PIN_RLED, LOW);
      digitalWrite(PIN_BLED, HIGH);
      tone(PIN_SPEKP, 6000);
      delay(500);
    }
  }
}
