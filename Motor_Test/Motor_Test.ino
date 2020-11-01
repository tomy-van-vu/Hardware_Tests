/*
   Serial Monitor inputs:
   0: nothing
   1: change direction to left
   2: change direction to right
   3: stop
   4: change speed to slow
   5: change speed to fast
*/

#include <Servo.h>

#define BAUD_RATE   9600
#define MOTOR_PIN   9

#define NEUTRAL     1500  // pulse to motor that will produce a neutral state
#define STOPPED     0
#define SLOW        100   // +/- from neutral to get a slow motor rotation
#define FAST        400   // +/- from neutral to get a fast motor rotation
#define LEFT        -1
#define RIGHT       1
#define INCREMENT   1     // increments/size of change of each new pulse sent to the motor


Servo my_motor;

int motor_speed;
int motor_direction;
int current_pulse;
int pulse_margin;


void setup() {
  Serial.begin(BAUD_RATE);

  // initialise motor
  my_motor.attach(MOTOR_PIN);
  current_pulse = NEUTRAL;
  motor_speed = STOPPED;
  motor_direction = LEFT;
  pulse_margin = INCREMENT;

}

void loop() {
  int command = read_msg();
  do_stuff(command);
  update_pulse();

  my_motor.writeMicroseconds(current_pulse);
  Serial.println(current_pulse);
}


void update_pulse() {

  int target_pulse = NEUTRAL + motor_speed * motor_direction;

  if (current_pulse > (target_pulse)) {
    current_pulse = current_pulse - INCREMENT;
    return;
  }
  else if (current_pulse < (target_pulse)) {
    current_pulse = current_pulse + INCREMENT;
    return;
  }

  return;
}
void do_stuff(int command) {
  switch (command) {
    case 48:  // key:0 meaning:
      break;

    case 49:  // key:1 meaning:DIRECTION_LEFT
      motor_direction = LEFT;
      break;

    case 50:  // key:2 meaning:DIRECTION_RIGHT
      motor_direction = RIGHT;
      break;

    case 51:  // key:3 meaning:TO_NEUTRAL/STOP
      motor_speed = STOPPED;
      break;

    case 52:  // key:4 meaning:SPEED_SLOW
      motor_speed = SLOW;
      break;

    case 53:  // key:5 meaning:SPEED_FAST
      motor_speed = FAST;
      break;

    default:
      // do nothing / keep performing same task
      break;
  }
}

int read_msg() {
  int msg = -1;
  if (Serial.available()) {
    msg = Serial.read();
  }

  return msg;
}
