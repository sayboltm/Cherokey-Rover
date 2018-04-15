/*
Code by M. Saybolt
//
Hacking evoggy's code is too involved. The hercules skeleton bot's operation is much
more different from Cherokey's than anticipated. Not much can be saved, but using
the scope yielded useful information about the communication protocol being used,
and how evoggy's code controls the hercules bot. Analyzing the Cherokey example,
a translation layer can be made to control the Cherokey rover in a similar manner
and still using the chars sent over serial from evoggy's crazyflie firmware 
modification.
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
    motorSetSpeed(0);
}


void loop()
{
    while (Serial.available() > 0)
    {
        c = (char)Serial.read();
        if (c == '\n') // The packet coming over the line is finished,
        {
            // pass, insert cases to handle over serial line

            Serial.println(";"); // Notify user of end
            charQueue = ""; // Reset the char queue
        }
        else
        {
            // IF not the end of a packet,
            // Add the chars into a char queue
            charQueue += c;
        }
    }
}

void motorSetSpeed(int power)
{
}

void motorStop(int duration)
{
}

void motorForward(int duration)
{
}

void motorReverse(int duration)
{
}

void motorTurnLeft(int duration)
{
}


void motorTurnRight(int duration)
{
}
