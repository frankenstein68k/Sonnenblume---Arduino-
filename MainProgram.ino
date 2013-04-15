#include <Servo.h>

//holds the average of the readings from groupings of 2 sensors
int a_top;
int b_bottom;
int c_left;
int d_right;

//holds the difference between the reading from the top 2 sensors and bottom 2 sensors
int topVbottom;
//holds the difference between the reading from the left 2 sensnors and the right 2 sensors
int leftVright;

//holds the value of when a the servo should move or not move
const int threshold = 75;

//names attached to servos so that I can address them easier
Servo servoLR;
Servo servoUD;

//minimum and maximum values for the horizontal and vertical servos that define range of motion
int minServLR = 0;
int maxServLR = 145;
int minServUD = 5;
int maxServUD = 85;

//holds the variable on how much to move the servo, 1 = 1 degree movement
int defMove = 1;

//holds the location last written to the servo
int lastUDwrite;
int lastLRwrite;

//variables to hold the readings from the left top (a_lt), right top (b_rt), left bottom (c_lb) and right bottom (d_rb) sensors
int a_lt;
int b_rt;
int c_lb;
int d_rb;

//a delay to that is used by the arduino to control the speed of the servos
int delayT=5;

void setup()
{
  Serial.begin(9600); // Set up connection back to the computer to output useful data
  servoUD.attach(9);  // Set up down servo to digital pin 9 - yellow
  servoLR.attach(10);  // Set left right servo to digital pin 10 - orange
  homePosition();
}

void loop()
{
  //Takes reading form sensors
  a_lt = analogRead(0); //Orange
  b_rt = analogRead(1); //Green
  c_lb = analogRead(2); //Yellow
  d_rb = analogRead(3); //Purple

  //Averages the readings from groups of sensors. Top, bottom, left and right.
  a_top = (a_lt + b_rt)/2;
  b_bottom = (c_lb+d_rb)/2;
  c_left = (a_lt + c_lb)/2;
  d_right = (b_rt + d_rb)/2;



//Prints useful information about the individual sensor readings and the last position of the servos
  Serial.print("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
  Serial.print("top left\t");
  Serial.print(a_lt);
  Serial.print("\t\tServo UP down position\t\t");
  Serial.println(lastUDwrite);

  Serial.print("top right\t");
  Serial.print(b_rt);
  Serial.print("\t\tServo left right position\t");
  Serial.println(lastLRwrite);
  Serial.print("bottom left\t");
  Serial.print(c_lb);
  Serial.print("\nbottom right\t");
  Serial.println(d_rb);
  delay(delayT);


  //Difference between top and bottom, the converted to an absolute number
  topVbottom = a_top-b_bottom;
  topVbottom = abs(topVbottom);

  //Difference between left and right, then converted to an absolute number
  leftVright = c_left-d_right;
  leftVright = abs(leftVright);

  //Top vs. bottom comparison
  if (topVbottom > threshold){
    delay(delayT);
    if (a_top < b_bottom && topVbottom>threshold){
      moveUp();
      Serial.print("move up !");
      delay(delayT);
    }
    if (a_top > b_bottom && topVbottom>threshold){
      moveDown();
      Serial.print("move down !");
      delay(delayT);
    }
  }

  //Left vs. right comparison
  if (leftVright > threshold){
    delay(delayT);
    if (c_left < d_right && leftVright>threshold){
      moveLeft();
      Serial.print("move left !");
      delay(delayT);
    }
    if (c_left > d_right && leftVright>threshold){
      moveRight();
      Serial.print("move right !");
      delay(delayT);
    }
  }
}

//A safe position to start and stop the servos at
void homePosition() {
  servoLR.write(50);
  servoUD.write(75);
  lastUDwrite = 50;
  lastLRwrite = 75;
}

//Moves the up down servo up
void moveUp(){
  lastUDwrite = lastUDwrite - defMove;

  if (lastUDwrite >= maxServUD){
    lastUDwrite = maxServUD; 
  }

  servoUD.write(lastUDwrite);
}

//Moves the up down servo down
void moveDown(){
  lastUDwrite = lastUDwrite + defMove;

  if (lastUDwrite <= minServUD){
    lastUDwrite = minServUD; 
  }

  servoUD.write(lastUDwrite);
}

//Moves the left right servo down
void moveLeft(){
  lastLRwrite = lastLRwrite - defMove;

  if (lastLRwrite >= maxServLR){
    lastLRwrite = maxServLR; 
  }

  servoLR.write(lastLRwrite);
}

//Moves the left right servo down
void moveRight(){
  lastLRwrite = lastLRwrite + defMove;

  if (lastLRwrite < minServLR){
    lastLRwrite = minServLR; 
  }

  servoLR.write(lastLRwrite);
}

