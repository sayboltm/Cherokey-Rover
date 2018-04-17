/*
Code by M. Saybolt
//
Hacking evoggy's code is too involved. The hercules skeleton bot's operation is much
more different from Cherokey's than anticipated. Not much can be saved, but using
the scope yielded useful information about the communication protocol being used,
and how evoggy's code controls the hercules bot. Analyzing the Cherokey example,
a translation layer can be made to control the Cherokey rover in a similar manner
and still using the chars sent over serial from evoggy's crazyflie firmware 
modification. This is a hack of Evoggy's code to control the Cherokey Rover using
a similar archetecture. It works well enough for proof of concept! Couldn't have done
it without you, evoggy.

Wire it up just like the Cherokey example 2, but the rx/tx lines can be ignored.
*/

#define PM1_EN  4   // M1 direction, low, high
#define PM1_PWM 5   // M1 PWM, 0-255
#define PM2_PWM 6   // M2 PWM, 0-255
#define PM2_EN  7   // M2 direction, low, high

// Setup copied from evoggy with slight modification
String  charQueue= "";
char c;
int motorSpeed = 0;

void setup()
{
    // Open the serial connection to the CF2
    Serial.begin(19200); 
    Serial.println("Cherokey 4WD TEST");

    pinMode(PM1_EN, OUTPUT);
    pinMode(PM1_PWM, OUTPUT);
    pinMode(PM2_EN, OUTPUT);
    pinMode(PM2_PWM, OUTPUT);

    // PWM of 0 gives 0 speed to Cherokey motors
    motorInit(0);
}


void loop()
{
    while (Serial.available() > 0)
    {
        c = (char)Serial.read();
        if (c == '\n') // The packet coming over the line is finished,
        {
            // pass, insert cases to handle over serial line
            Serial.println("Command: ");
            Serial.println(charQueue);

            Serial.println("Char: ");
            // Declare char array, currently 5 elements is enough
            // to handle actions from CF2/controller
            char charArr[5];
            charQueue.toCharArray(charArr, 5);
            Serial.println(charQueue);

            // Acquire speeds from serial packet to set and tell user
            Serial.print("SpeedM1: ");
            int speedM1 = charArr[2];
            // speedM1 = (speedM1 - 48) * 28; // Copied verbatim from evoggy, not sure what this is for exactly
            // See picture of scope screen
            // Need to map these 0-9-Null values to 0-255, unless thats what this does!
            speedM1 = speedM1 * 28; // Scale so 9 is ~255
            Serial.print("SpeedM2: ");
            int speedM2 = charArr[3];
            // speedM2 = (speedM2 - 48) * 28; //dunno why this, but 28 showed up in my version too!
            speedM2 = speedM2 * 28; // Scale so 9 is ~255

            // Print the new speed values for user on Serial monitor
            Serial.print("SpeedM1 adjusted: ");
            Serial.println(speedM1);
            Serial.print("SpeedM2 adjusted: ");
            Serial.println(speedM2);
           
            // Write values to PWM
            // analogWrite(PM1_PWM, speedM1);
            // analogWrite(PM2_PWM, speedM2);
            // Need to pass into cases for Cherokey

            // Unlike example, need separate if tree (notice no elifs) so each case
            // is tested every time as the loop progresses, vs one action per cycle of loop
            // which is fine for simple keyboard input/test

            // Process direction.
            // charArr[0] and [1] hold direction for two motors. For me
            // watching the scope, B was forward and F was backward.
            // This should be changed in the CF firmware to make it easier to read, unless I'm missing something
            if (charArr[0] == 'S' && charArr[1] == 'S') // Never actually seen this case but copying evoggy
            {
                motorStop(0);
                Serial.print("Stop");
            }
            if (charArr[0] == 'F' && charArr[1] == 'F') // Never actually seen this case but copying evoggy
            {
                // Something in the CF firmware makes FW on the controller (with default drone mapping
                // in cfclient) send Bs, and backward send F.. so easily flipped them here.
                motorReverse(speedM1, speedM2, 0);
                Serial.print("Forward");
            }
            if (charArr[0] == 'B' && charArr[1] == 'B') // Never actually seen this case but copying evoggy
            {
                motorForward(speedM1, speedM2, 0);
                Serial.print("Reverse");
            }
            if (charArr[0] == 'B' && charArr[1] == 'F') // Never actually seen this case but copying evoggy
            {
                motorTurnLeft(speedM1, speedM2, 0);
                Serial.print("TurnLeft");
            }
            if (charArr[0] == 'F' && charArr[1] == 'B') // Never actually seen this case but copying evoggy
            {
                motorTurnRight(speedM1, speedM2, 0);
                Serial.print("TurnRight");
            }

            Serial.println(";"); // Notify user of end
            charQueue = ""; // Reset the char queue
        }
        else
        {
            // IF not the end of a packet,
            // Add the chars into a char queue
            charQueue += c;
            Serial.println(c);
        }
    }
}

void motorInit(int power)
{
    // This function seems pointless/only used once
    analogWrite(PM1_PWM, power);
    analogWrite(PM2_PWM, power);
}

//void motor1update(
void motorStop(int duration)
{
    digitalWrite(PM1_PWM, 0);
    digitalWrite(PM2_PWM, 0);
    digitalWrite(PM1_EN, LOW);
    digitalWrite(PM2_EN, LOW);
    delay(duration);
}

void motorForward(int speedM1, int speedM2, int duration)
{
    // Attempt to shut off motors if no input
    if (speedM1 == 0 && speedM2 == 0)
    {
        digitalWrite(PM1_EN, LOW);
        digitalWrite(PM2_EN, LOW);
        analogWrite(PM1_PWM, speedM1);
        analogWrite(PM2_PWM, speedM2);
    }
    else
    {
        digitalWrite(PM1_EN, HIGH);
        digitalWrite(PM2_EN, LOW);
        analogWrite(PM1_PWM, speedM1);
        analogWrite(PM2_PWM, speedM2);
    }
    delay(duration);
}

void motorReverse(int speedM1, int speedM2, int duration)
{
    if (speedM1 == 0 && speedM2 == 0)
    {
        digitalWrite(PM1_EN, LOW);
        digitalWrite(PM2_EN, LOW);
        analogWrite(PM1_PWM, speedM1);
        analogWrite(PM2_PWM, speedM2);
    }
    else
    {
        digitalWrite(PM1_EN, LOW);
        digitalWrite(PM2_EN, HIGH);
        analogWrite(PM1_PWM, speedM1);
        analogWrite(PM2_PWM, speedM2);
    }
    delay(duration);
}

void motorTurnLeft(int speedM1, int speedM2, int duration)
{
    if (speedM1 == 0 && speedM2 == 0)
    {
        digitalWrite(PM1_EN, LOW);
        digitalWrite(PM2_EN, LOW);
        analogWrite(PM1_PWM, speedM1);
        analogWrite(PM2_PWM, speedM2);
    }
    else
    {
        digitalWrite(PM1_EN, HIGH);
        digitalWrite(PM2_EN, HIGH);
        analogWrite(PM1_PWM, speedM1);
        analogWrite(PM2_PWM, speedM2);
    }
    delay(duration);
}


void motorTurnRight(int speedM1, int speedM2, int duration)
{
    // This one doesn't need the hack since both motors are set LOW to
    // achieve this action
    digitalWrite(PM1_EN, LOW);
    digitalWrite(PM2_EN, LOW);
    analogWrite(PM1_PWM, speedM1);
    analogWrite(PM2_PWM, speedM2);
    delay(duration);
}
