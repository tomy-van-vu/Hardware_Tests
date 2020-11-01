
#define BAUD_RATE       9600
#define SOLENOID_PIN    8
#define ON_DUTY_CYCLE   1500  // milliseconds

#define AUTO_OFF        

bool solenoid_active;
unsigned long time_activated;

void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(SOLENOID_PIN, OUTPUT);
  solenoid_active = false;

}

void loop() {
  int command = read_msg();
  do_stuff(command);

  #ifdef AUTO_OFF
  if ((solenoid_active == true) && (millis() - time_activated >= ON_DUTY_CYCLE)) {
    solenoid_off();
  }
  
  #endif
}



void do_stuff(int command) {
  bool success = false;
  switch (command) {
    
    case 49:  // key:1 meaning: 
      if (solenoid_active) {
        Serial.println("Solenoid already active\n");
        break;
      } else{
        solenoid_on();
      }
      break;

    case 50:  // key:2 meaning: 
      if (!solenoid_active) {
        Serial.println("Solenoid already off\n");
        break;
      } else {
        solenoid_off();
      }
      break;
    default:
      // do nothing
      break;
  }
}

void solenoid_on() {
  // already opened
  if (solenoid_active) {return ;}
  time_activated = millis();
  digitalWrite(SOLENOID_PIN, HIGH); 
  solenoid_active = true;
  Serial.println("Solenoid is now active\n");
}

void solenoid_off() {  
  // already closed
  if (!solenoid_active) {return;}
  
  digitalWrite(SOLENOID_PIN, LOW); 
  solenoid_active = false;
  Serial.println("Solenoid is now off\n");
}

int read_msg() {
  int msg = -1;
  if (Serial.available()) {
    msg = Serial.read();
  }

  return msg;
}
