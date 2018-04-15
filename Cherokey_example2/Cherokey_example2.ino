/*
 # Edited by:  Matt
 # Date:       2015.09.06
 # Version:    1.1
 # Product:    Cherokey 4WD Mobile Platform
 # SKU:        ROB0102/ROB0117

 # Description:
 # Drive 2 motors with this Cherokey 4WD Mobile Platform
 # Connect D4,D5,D6,D7,GND to UNO digital 4,5,6,7,GND

*/
//Motor Definitions
int E1 = 5;     //M1 Speed Control
int E2 = 6;     //M2 Speed Control
int M1 = 4;     //M1 Direction Control
int M2 = 7;     //M2 Direction Control

//DIRECTIONS

//STOP
void stop(void)
{
  digitalWrite(E1, 0);
  digitalWrite(M1, LOW);
  digitalWrite(E2, 0);
  digitalWrite(M2, LOW);
}

//ADVANCE
void advance(char a, char b) // -> changed to turn right
{
  analogWrite (E1, a);
  digitalWrite(M1, HIGH);
  analogWrite (E2, b);
  //digitalWrite(M2, HIGH);
  digitalWrite(M2, LOW);
}

//MOVE BACKWARDS // -> changed to turn left, low d4, ~2.2 instead of 5
void back_off (char a, char b)
{
  analogWrite (E1, a);
  digitalWrite(M1, LOW); // = ~2.2 V @ pin 4, M1
  analogWrite (E2, b);
  //digitalWrite(M2, LOW);
  digitalWrite(M2, HIGH);
}


//TURN LEFT // -> changed to current go forward
void turn_L (char a, char b)
{
  analogWrite (E1, a);
  //digitalWrite(M1, LOW);
  digitalWrite(M1, HIGH);
  analogWrite (E2, b);
  digitalWrite(M2, HIGH);
}

//TURN RIGHT // -> changed to go backward, low d4, ~2.2 instead of 5
void turn_R (char a, char b)
{
  analogWrite (E1, a);
  //digitalWrite(M1, HIGH);
  digitalWrite(M1, LOW);
  analogWrite (E2, b);
  digitalWrite(M2, LOW);
}

void setup(void) {
  int i;
  for (i = 4; i <= 7; i++)
    pinMode(i, OUTPUT);
  Serial.begin(9600);      //Set Baud Rate
  Serial.println("hello. w = forward, d = turn right, a = turn left, s = backward, x = stop, z = hello world"); //Display instructions in the serial monitor
  digitalWrite(E1, LOW);
  digitalWrite(E2, LOW);
}

void loop(void) {
  if (Serial.available()) {
    char val = Serial.read();
    if (val != -1)
    {
      switch (val)
      {
        case 'w'://Move Forward
          Serial.println("going forward");
          advance (255, 255);  //move forward at max speed
          delay (1000);
          stop();
          break;
        case 's'://Move Backward
          Serial.println("going backward");
          back_off (255, 255);  //move backwards at max speed
          delay (1000);
          stop();
          break;
        case 'a'://Turn Left
          Serial.println("turning left");
          turn_L (75, 75);
          delay (1000);
          stop();
          break;
        case 'd'://Turn Right
          Serial.println("turning right");
          turn_R (125, 125);
          delay (1000);
          stop();
          break;
        case 'z':
          Serial.println("hello world!");
          break;
        case 'x':
          Serial.println("stopping");
          stop();
          break;
      }
    }
    else stop();
  }
}
