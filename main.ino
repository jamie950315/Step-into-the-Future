/*--------------------------------------------------
todo list:
1. read data from rshoe and lshoe   (done
2. read data from phone             (done
3. read data from nano(ctrl)        (done
4. write data to phone(batt)        (done
5. write data to nano(ctrl)         (probably don't need to do this
6. write data to rshoe and lshoe    (done
7. direction vibration              (done
8. limit speed                      (done
9. write data to driver board       (done
10.speed indicator                  (done
11.turn signal                      (will do this later
12.nano battery indicator           (done
13.phone as ctrl                    (will do this later






             hc-06 hc-05      nrf24l01
tx  dirc     batt  naBa       ctrl
rx  batt     dirc  ctrl       naBa<---what is this?

    phone       2560          nano

--------------------------------------------------*/

//Q: please based on the code below, translate the name "step into the future" to chinese of this project, give 10 options, and explain why you choose this name


#include <SPI.h>
#include "RF24.h"

RF24 rf24(7, 8); // CE腳, CSN腳

const byte addr[] = "1Node";
const byte pipe = 1;  // 指定通道編號


char currChar;
char prevChar = '\0';
int  speedValue;
int  pwmOutPin = 11;
int  stopPin = 9;
int  turnPin = 10;
int  constrainValueR = 400;
int  constrainValueL = 400;
int  constrainValue  = 400;
#define battMax 4.5839
#define battMin 3.3137


void setup()
{
    Serial.begin(38400);    // connect to the serial port
    Serial1.begin(38400);    // connect to the Lshoe
    Serial2.begin(9600);    // connect to the phone@hc06bt2@98:D3:51:FE:16:5E
    Serial3.begin(38400);    // connect to the Rshoe

    //setup pwm 2kHz
    TCCR1A = 0b00000011; // 10bit
    TCCR1B = 0b00001010; // x8 fast pwm


    rf24.begin();           // connect to the nano(ctrl) via nf24l01
    rf24.setChannel(100);  // 設定頻道編號
    //rf24.setPALevel(RF24_PA_MIN);
    //rf24.setDataRate(RF24_250KBPS);
    rf24.openReadingPipe(pipe, addr);  // 開啟通道和位址
    rf24.startListening();  // 開始監聽無線廣播
    Serial.println("nRF24L01 ready!");

    /*make pin 4 as gnd, pin 5 as vcc
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    */

    //read battery status
    pinMode(A0, INPUT);     

    //setup ctrl pin
    pinMode(pwmOutPin, OUTPUT); 
    pinMode(stopPin, OUTPUT);
    pinMode(turnPin, OUTPUT);

}

void loop()
{
    delay(10);



    
    if (Serial.available()) {
        currChar = Serial.read();
        if (currChar == 'l' || currChar == 'L') {
            Serial1.write(currChar);
            //Serial1.println();
        }
    }
    

    
    //read phone data
    if (Serial2.available())
    {
        currChar = Serial2.read();
        Serial.write(currChar);
        Serial.println();
        
        /*write battery data to phone
        if (currChar == 200){   
            Serial2.write(120);         //120 is the start byte to phone
            Serial2.write(map(analogRead(A0),0,1023,0,100)); //write battery data to phone
              
                                                     //200 is the start byte from phone
            int volSensorValue = analogRead(A0);                            //read battery voltage
            float battVol = (volSensorValue / 1023.0) * 5.0;                //convert to voltage
            int battPercent = map(battVol, battMin, battMax, 0, 100);     //convert to percentage
            battPercent = constrain(battPercent, 0, 100);                   //constrain the value to 0~100
            
            
            
        }
        */
        
        //write data to rshoe if currChar is r or R
        if (currChar == 'r' || currChar == 'R') {
            Serial3.write(currChar);
            Serial3.println();
        }
        

        //write data to lshoe if currChar is l or L
        if (currChar == 'l' || currChar == 'L') {        
            Serial1.write(currChar);
            Serial1.println();
            
        }

        
        
        /* only print when the data is different from the previous one
        if (currChar != prevChar) {
            Serial.write(currChar);
            prevChar = currChar;
            Serial.println();
        }
        */

    }

    
    /*
    //write data to phone
    if (Serial.available()) {
        Serial2.write(Serial.read());
        
    }
    */


    
    //read Rshoe data
    if (Serial3.available()) 
    {
        constrainValueR = (Serial3.parseInt());
    }
    
    
    //read Lshoe data
    if (Serial1.available())
    {
        constrainValueL = (Serial1.parseInt());
    }
    
    //-----------------------------------------------
    // .        DONT FORGET TO REENABLE THIS         .
    // .        DONT FORGET TO REENABLE THIS         .
    // .        DONT FORGET TO REENABLE THIS         .
    // .        DONT FORGET TO REENABLE THIS         .
    // .        DONT FORGET TO REENABLE THIS         .
    // .        DONT FORGET TO REENABLE THIS         .
    // .        DONT FORGET TO REENABLE THIS         .
    // .        DONT FORGET TO REENABLE THIS         .
    // .        DONT FORGET TO REENABLE THIS         .
    // .        DONT FORGET TO REENABLE THIS         .
    //constrainValue = (min(constrainValueL,constrainValueR));
    //constrainValue = constrainValueR;


    /*
    if (rf24.available(&pipe)) {
        int msg;
        rf24.read(&msg, sizeof(msg));
        Serial.println(msg); // 顯示訊息內容 
    }
*/
    
    //read nano data
    if (rf24.available(&pipe))
    {
        //Serial.println("ok");  
        int statusSpeedValue;
        //int msg;   
        rf24.read(&speedValue, sizeof(speedValue));   
        statusSpeedValue = speedValue;
        //0.3v ~ 4.5v = 65 ~ 930

        if ((statusSpeedValue - 10000) < 0) {
            speedValue = speedValue;
        } else if (((statusSpeedValue - 10000) > 0) && ((statusSpeedValue - 10000) < 10000)) {
            speedValue = speedValue-10000;
        } else if (((statusSpeedValue - 20000) > 0) && ((statusSpeedValue - 20000) < 10000)) {
            speedValue = speedValue-20000;          
        } else if (((statusSpeedValue - 30000) > 0) && ((statusSpeedValue - 30000) < 10000)) {
            speedValue = speedValue-30000;
        }

        Serial.println(constrainValue);
        if (constrainValue <= 10) {
            speedValue = constrain(speedValue, 0,   2);
            digitalWrite(stopPin, LOW);
            //digitalWrite(turnPin, LOW);
            //analogWrite(pwmOutPin, 300);
        } else if (constrainValue > 10 && constrainValue <= 20) {
            speedValue = constrain(speedValue, 0, 4);
            digitalWrite(stopPin, LOW);
            //analogWrite(pwmOutPin, 300);
        } else if (constrainValue > 20 && constrainValue <= 30) {
            speedValue = constrain(speedValue, 0, 30);
            digitalWrite(stopPin,LOW);
        } else if (constrainValue > 30 && constrainValue <= 40) {
            speedValue = constrain(speedValue, 0, 80);
            digitalWrite(stopPin, HIGH);
        } else if (constrainValue > 40 && constrainValue <= 50) {
            speedValue = constrain(speedValue, 0, 140);
            digitalWrite(stopPin, HIGH);
        } else if (constrainValue > 50 && constrainValue <= 300) {
            speedValue = constrain(speedValue, 0, 400);
            digitalWrite(stopPin, HIGH);
        } else if (constrainValue > 300 && constrainValue <= 355) {
            speedValue = constrain(speedValue, 0, 500);
            digitalWrite(stopPin, HIGH);
        } else {
            speedValue = constrain(speedValue, 0, 1023);
            //digitalWrite(stopPin, HIGH);
        }
        //speedValue/2 means lower the speed
        if ((statusSpeedValue - 10000) < 0) {
            Serial.println(speedValue/2);
            analogWrite(pwmOutPin, speedValue/2);
            digitalWrite(stopPin, HIGH);
            digitalWrite(turnPin, HIGH);
        } else if (((statusSpeedValue - 10000) > 0) && ((statusSpeedValue - 10000) < 10000)) {
            Serial.print("stop: ");
            Serial.println(speedValue/2);
            analogWrite(pwmOutPin, speedValue/2);
            digitalWrite(stopPin, LOW);
            digitalWrite(turnPin, HIGH);
        } else if (((statusSpeedValue - 20000) > 0) && ((statusSpeedValue - 20000) < 10000)) {
            Serial.print("turn: ");
            Serial.println(speedValue/2);
            analogWrite(pwmOutPin, speedValue/2);
            digitalWrite(stopPin, HIGH);
            digitalWrite(turnPin, LOW);
        } else if (((statusSpeedValue - 30000) > 0) && ((statusSpeedValue - 30000) < 10000)) {
            Serial.print("stop+turn: ");
            Serial.println(speedValue/2);
            analogWrite(pwmOutPin, speedValue/2);
            digitalWrite(stopPin, LOW);
            digitalWrite(turnPin, LOW);
        }
    }
}
