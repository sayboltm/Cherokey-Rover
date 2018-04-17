// Code by Marcus Eliasson of the Bitcraze team
// pin ctrl
// #define PINCS           6           // all mos cs
#define PINCS           3 // No longer used
#define PINM1R          4
#define PINM1F          5
#define PINM2F          7
#define PINM2R          8
#define PINPWMA         9           // PWM channel A (motor A speed)
#define PINPWMB         10          // PWM channel B (motor B speed)

String txtMsg = "";
char s;
int motorSpeed = 0;

void setup()
{
    Serial.begin(19200); // This is same as in firmware. Must be UART -> serial. Need to find std serial interface pins for an arduino if exists
    Serial.println("HERCULES 4WD TEST");

    pinMode(PINCS, OUTPUT); // 6
    pinMode(PINM1F, OUTPUT);// 5
    pinMode(PINM1R, OUTPUT);// 4
    pinMode(PINM2F, OUTPUT);// 7
    pinMode(PINM2R, OUTPUT);// 8

    motorSetSpeed(255);
}


void loop()
{
    while (Serial.available() > 0)
    {
        s = (char)Serial.read();
        if (s == '\n')
        {
            Serial.print("Command: ");
            Serial.print(txtMsg);

            Serial.print(" Char: ");
            char charBuf[5];
            txtMsg.toCharArray(charBuf, 5);
            Serial.print(charBuf);
            
            Serial.print(" SpeedA: ");
            int speedA = charBuf[2]; // Charbuf 2 == speed A
            speedA = ( speedA - 48 ) * 28;
            Serial.print( speedA );
            
            Serial.print(" SpeedB: ");
            int speedB = charBuf[3]; // Charbuf 3 == speed B
            speedB = (speedB - 48) * 28;
            Serial.print( speedB );    
      
            analogWrite(PINPWMA, speedA); // Write speedA to PINPWMA, these must be the pwm channels for the motors
            analogWrite(PINPWMB, speedB);      

            if ( charBuf[0] == 'S' && charBuf[1] == 'S') // movement appearing to come through the charBuf
            {
                motorStop(0);
                Serial.print(" Action: Stop");
            }

            if ( charBuf[0] == 'F' && charBuf[1] == 'F')
            {
                motorForward(0);
                Serial.print(" Action: Forward");
            }

            if ( charBuf[0] == 'B' && charBuf[1] == 'B')
            {
                motorReverse(0);
                Serial.print(" Action: Reverse");
            }

            if ( charBuf[0] == 'B' && charBuf[1] == 'F')
            {
                motorTurnLeft(0);
                Serial.print(" Action: Left");
            }

            if ( charBuf[0] == 'F' && charBuf[1] == 'B')
            {
                motorTurnRight(0);
                Serial.print(" Action: Right");
            }

            Serial.println(";");
            txtMsg = "";
        }
        else
        {
            txtMsg += s;
        }
    }
}


void motorSetSpeed(int power)
{
    motorSpeed = power;
    analogWrite(PINPWMA, motorSpeed);
    analogWrite(PINPWMB, motorSpeed);
}

void motorStop(int duration)
{
    digitalWrite(PINCS, LOW); // < PINCS low for a stop command
    digitalWrite(PINM1F, LOW);

    digitalWrite(PINM1R, LOW);
    digitalWrite(PINM2F, LOW);

    digitalWrite(PINM2R, LOW);
    delay(duration);
}

void motorForward(int duration)
{
    digitalWrite(PINM1R, HIGH);
    digitalWrite(PINM1F, LOW);

    digitalWrite(PINM2R, HIGH);
    digitalWrite(PINM2F, LOW);

    digitalWrite(PINCS, HIGH); // < PINCS  high for movement
    delay(duration);
}

void motorReverse(int duration)
{
    digitalWrite(PINM1R, LOW);
    digitalWrite(PINM1F, HIGH);

    digitalWrite(PINM2R, LOW);
    digitalWrite(PINM2F, HIGH);

    digitalWrite(PINCS, HIGH); // < PINCS  high for movement
    delay(duration);
}

void motorTurnLeft(int duration)
{
    digitalWrite(PINM1R, LOW);
    digitalWrite(PINM1F, HIGH);

    digitalWrite(PINM2R, HIGH);
    digitalWrite(PINM2F, LOW);

    digitalWrite(PINCS, HIGH); // < PINCS  high for movement
    delay(duration);
}


void motorTurnRight(int duration)
{
    digitalWrite(PINM1R, HIGH);
    digitalWrite(PINM1F, LOW);

    digitalWrite(PINM2R, LOW);
    digitalWrite(PINM2F, HIGH);

    digitalWrite(PINCS, HIGH); // < PINCS  high for movement
    delay(duration);
}

/* REVEGR analysls:
PINCS is hercules specific, similar to enable pin?? (but no holding torque with dc motors right?)

*/