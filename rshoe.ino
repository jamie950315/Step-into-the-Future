//add two vibration motors to the arduino board and connect the positive pin to pin 6 and negative pin to pin 7 for the first motor and positive pin to pin 8 and negative pin to pin 9 for the second motor
int motor1pos = 6;
int motor1neg = 7;
int motor2pos = 8;
int motor2neg = 9;

//add two hc-sr04 ultrasonic sensor using ultrasonic.h to the arduino board and connect the trigger pin to pin 2 and echo pin to pin 3 for the first sensor and trigger pin to pin 4 and echo pin to pin 5 for the second sensor
#include <Ultrasonic.h>
Ultrasonic ultrasonic1(2, 3);
Ultrasonic ultrasonic2(4, 5);
void setup()
{
    Serial.begin(38400);
    pinMode(motor1pos, OUTPUT);
    pinMode(motor1neg, OUTPUT);
    pinMode(motor2pos, OUTPUT);
    pinMode(motor2neg, OUTPUT);
    digitalWrite(motor1pos, LOW);
    digitalWrite(motor1neg, LOW);
    digitalWrite(motor2pos, LOW);
    digitalWrite(motor2neg, LOW);
}
void loop()
{
    //choose the minimum value of the two sensors as the distance
    static unsigned long lastSwitchTime = 0;
    if (millis() - lastSwitchTime >= 30) {
        int val1 = ultrasonic1.read();
        int val2 = ultrasonic2.read();
        Serial.println(min(val1,val2));
        //Serial.println(ultrasonic2.read());
        lastSwitchTime = millis();
    }
    //delay(10);
    //if receive data from serial monitor, identify received data is L or l and make if condition of each other
    if (Serial.available())
    {
        char val = Serial.read();

        //Serial.println(val);

        if (val == 'r')
        {
            //if receive r, turn on the two motors for 3 seconds, then turn off for 1 second, then turn on for 4 seconds, then turn off
            digitalWrite(motor1pos, HIGH);
            digitalWrite(motor1neg, LOW);
            digitalWrite(motor2pos, HIGH);
            digitalWrite(motor2neg, LOW);
            //Serial.println("ron");
            unsigned long startTime = millis();
            while (millis() - startTime < 3000) {
                int val1 = ultrasonic1.read();
                int val2 = ultrasonic2.read();
                Serial.println(min(val1,val2));                
                delay(30);

            }
            digitalWrite(motor1pos, LOW);
            digitalWrite(motor1neg, LOW);
            digitalWrite(motor2pos, LOW);
            digitalWrite(motor2neg, LOW);
            //Serial.println("roff");
            while (millis() - startTime < 4000) {
                int val1 = ultrasonic1.read();
                int val2 = ultrasonic2.read();
                Serial.println(min(val1,val2));                
                delay(30);

            }
            digitalWrite(motor1pos, HIGH);
            digitalWrite(motor1neg, LOW);
            digitalWrite(motor2pos, HIGH);
            digitalWrite(motor2neg, LOW);
            //Serial.println("ron");
            while (millis() - startTime < 7000) {
                int val1 = ultrasonic1.read();
                int val2 = ultrasonic2.read();
                Serial.println(min(val1,val2));                
                delay(30);

            }
            digitalWrite(motor1pos, LOW);
            digitalWrite(motor1neg, LOW);
            digitalWrite(motor2pos, LOW);
            digitalWrite(motor2neg, LOW);
            //Serial.println("roff");
            
            
        }
        else if (val == 'R')
        {
            //if receive R, turn on the two motors for 7 seconds, then turn off
            digitalWrite(motor1pos, HIGH);
            digitalWrite(motor1neg, LOW);
            digitalWrite(motor2pos, HIGH);
            digitalWrite(motor2neg, LOW);
            //Serial.println("RON");
            unsigned long startTime = millis();
            while (millis() - startTime < 7000) {
                int val1 = ultrasonic1.read();
                int val2 = ultrasonic2.read();
                Serial.println(min(val1,val2));                
                delay(30);

            }
            digitalWrite(motor1pos, LOW);
            digitalWrite(motor1neg, LOW);
            digitalWrite(motor2pos, LOW);
            digitalWrite(motor2neg, LOW);
            //Serial.println("ROFF");
            
            
        }
    }
}
