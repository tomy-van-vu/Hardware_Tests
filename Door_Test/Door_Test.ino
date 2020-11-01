/*
   Serial Monitor inputs:
   0: nothing
   1: open
   2: close
*/
#include <Servo.h>

Servo my_door;

#define BAUD_RATE         9600
#define DOOR_PIN          7
#define ROTATE_TIME       4000  // milliseconds
#define OPEN_DIRECTION    -1    // may need to swap with CLOSE_DIRECTION
#define CLOSE_DIRECTION   1
#define OPENED            true
#define CLOSED            false
#define NEUTRAL           1500  // pulse width that results in no rotation
#define OPEN_CLOSE_SPEED  100   // pulse width delta from neutral determines speed

bool door_position;
bool door_moving;   // allow tracking of start time, makes sure door servo rotates an exact amount of time for opening/closing
unsigned long action_start_time;


void setup() {
  Serial.begin(BAUD_RATE);
  my_door.attach(DOOR_PIN);
  
  door_position = CLOSED;
  door_moving = false;
  

}

void loop() {
  // put your main code here, to run repeatedly:
  int command = read_msg();
  do_stuff(command);
}

void do_stuff(int command) {
  bool success = false;
  switch (command) {
    case 49:  // key:1 meaning: Open doors
      if (door_position == OPENED) {
        Serial.println("Door is already opened\n");
      } else {
        Serial.println("Open door command received\n");
      }
      while(!door_position == OPENED) {open_door();}
      break;

    case 50:  // key:2 meaning: Close doors
      if (door_position == CLOSED) {
        Serial.println("Door is already closed\n");
      } else {
        Serial.println("Close door command received\n");
      }
      while(!door_position == CLOSED) {close_door();}
      break;
    default:
      // do nothing
      break;
  }
}


void open_door() {
  // already opened
  if (door_position == OPENED) {return ;}
  
  if (door_moving == false) {
    action_start_time = millis();
    door_moving = true;    
  }
  turn_motor(OPEN_DIRECTION);
  
}

void close_door() {  
  // already closed
  if (door_position == CLOSED) {return;}
  
  if (door_moving == false) {
    action_start_time = millis();
    door_moving = true;    
  }
  turn_motor(CLOSE_DIRECTION);
}


void turn_motor(int motor_direction) {
  if (door_moving == false) {return;} // break out if nothing to do
  
  if (motor_direction == OPEN_DIRECTION) {
    // writeMicroseconds (1500 + (-1)*100) = 1400
    my_door.writeMicroseconds(NEUTRAL + motor_direction * OPEN_CLOSE_SPEED);
  } 
  else if (motor_direction == CLOSE_DIRECTION) {
    // writeMicroseconds (1500 + (+1)*100) = 1600
    my_door.writeMicroseconds(NEUTRAL + motor_direction * OPEN_CLOSE_SPEED);
  }

  // door has moved in given direction for given time
  if (millis() - action_start_time >= ROTATE_TIME) {
    door_moving = false;
    if      (motor_direction == OPEN_DIRECTION)  {
      door_position = OPENED;
      Serial.println("Door is now open\n");
    }
    else if (motor_direction == CLOSE_DIRECTION) {
      door_position = CLOSED;
      Serial.println("Door is now closed\n");
    }
  }
}

int read_msg() {
  int msg = -1;
  if (Serial.available()) {
    msg = Serial.read();
  }

  return msg;
}
