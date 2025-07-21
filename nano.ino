



#include <SPI.h>
#include "RF24.h"
#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128 // OLED 寬度像素
#define SCREEN_HEIGHT 32 // OLED 高度像素


int data;
int prevChar = '\0';
int stop = 0;
int turn = 0;
#define LED1 8
#define LED2 9
//#define LED3 10
//#define LED4 11
//#define LED5 12

int processedValue = 0;

bool firstTime = true;
bool pressedStop = false;

#define speedLimiterPin 2

int speedLimiterStatus = 0;
int speedLimiterStatusPrev = 0;

RF24 rf24(3, 4); // CE腳, CSN腳

const byte addr[] = "1Node";
//char msg[] = "Hello World!";



// 設定OLED
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//自訂圖形 128x32

static const unsigned char PROGMEM d_sign[] =
{ 
	0x00, 0x00, 0x3f, 0x80, 0x3f, 0xe0, 0x39, 0xf0, 0x38, 0x38, 0x38, 0x38, 0x38, 0x18, 0x38, 0x1c, 
	0x38, 0x1c, 0x38, 0x18, 0x38, 0x38, 0x38, 0x78, 0x3f, 0xf0, 0x3f, 0xc0, 0x00, 0x00

};

static const unsigned char PROGMEM r_sign[] =
{
	0x00, 0x00, 0x00, 0x00, 0x7f, 0xe0, 0x7f, 0xf0, 0x70, 0x70, 0x70, 0x70, 0x70, 0xf0, 0x7f, 0xe0, 
	0x7f, 0x80, 0x71, 0xc0, 0x70, 0xe0, 0x70, 0xf0, 0x70, 0x70, 0x00, 0x00, 0x00, 0x00

};

static const unsigned char PROGMEM n_sign[] =
{
	0x00, 0x00, 0x38, 0x38, 0x38, 0x38, 0x3c, 0x38, 0x3e, 0x38, 0x3e, 0x38, 0x37, 0x38, 0x33, 0xb8, 
	0x31, 0xf8, 0x31, 0xf8, 0x30, 0xf8, 0x30, 0x78, 0x30, 0x78, 0x30, 0x38, 0x00, 0x00

};

static const unsigned char PROGMEM p_sign[] =
{
	0x00, 0x00, 0x3f, 0xe0, 0x3f, 0xf0, 0x38, 0x78, 0x38, 0x38, 0x38, 0x38, 0x3f, 0xf8, 0x3f, 0xf0, 
	0x38, 0x00, 0x38, 0x00, 0x38, 0x00, 0x38, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00

};

static const unsigned char PROGMEM s_sign[] =
{
	0x00, 0x00, 0x0f, 0xc0, 0x1f, 0xe0, 0x38, 0x70, 0x38, 0x70, 0x3c, 0x00, 0x1f, 0xc0, 0x0f, 0xf0, 
	0x00, 0xf0, 0x10, 0x38, 0x38, 0x38, 0x3c, 0x70, 0x1f, 0xf0, 0x0f, 0xc0, 0x00, 0x00

};   

static const unsigned char PROGMEM id_sign[] =
{
    0xff, 0xfe, 0xc0, 0x7e, 0xc0, 0x1e, 0xc6, 0x0e, 0xc7, 0xc6, 0xc7, 0xc6, 0xc7, 0xe6, 0xc7, 0xe2, 
	0xc7, 0xe2, 0xc7, 0xe6, 0xc7, 0xc6, 0xc7, 0x86, 0xc0, 0x0e, 0xc0, 0x3e, 0xff, 0xfe
};

static const unsigned char PROGMEM ir_sign[] =
{
    0xff, 0xff, 0xff, 0xff, 0x80, 0x1f, 0x80, 0x0f, 0x8f, 0x87, 0x8f, 0xc7, 0x8f, 0x8f, 0x80, 0x0f, 
	0x80, 0x3f, 0x8e, 0x3f, 0x8f, 0x1f, 0x8f, 0x0f, 0x8f, 0x87, 0x8f, 0xc7, 0xff, 0xff, 0xff, 0xff
};

static const unsigned char PROGMEM in_sign[] =
{
    0xff, 0xff, 0xc7, 0xe3, 0xc3, 0xe3, 0xc3, 0xe3, 0xc1, 0xe3, 0xc0, 0xe3, 0xc0, 0xe3, 0xc4, 0x63, 
	0xc6, 0x23, 0xc6, 0x23, 0xc7, 0x03, 0xc7, 0x83, 0xc7, 0x83, 0xc7, 0xc3, 0xff, 0xff, 0xff, 0xff
};

static const unsigned char PROGMEM ip_sign[] =
{
    0xff, 0xfe, 0xc0, 0x1e, 0xc0, 0x0e, 0xc7, 0x86, 0xc7, 0xc6, 0xc7, 0xc6, 0xc0, 0x06, 0xc0, 0x0e, 
	0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xc7, 0xfe, 0xff, 0xfe, 0xff, 0xfe
};

static const unsigned char PROGMEM is_sign[] =
{
    0xff, 0xfe, 0xf0, 0x3e, 0xe0, 0x1e, 0xc7, 0x8e, 0xc7, 0x8e, 0xc3, 0xfe, 0xe0, 0x3e, 0xf0, 0x0e, 
	0xff, 0x0e, 0xef, 0xc6, 0xc7, 0xc6, 0xc3, 0x8e, 0xe0, 0x0e, 0xf0, 0x3e, 0xff, 0xfe
};


static const unsigned char PROGMEM right_sign[] =
{ 
0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00
,0x00,0x01,0x00,0x00
,0x00,0x01,0x80,0x00
,0x00,0x01,0xc0,0x00
,0x00,0x01,0xe0,0x00
,0x00,0x01,0xf0,0x00
,0x0f,0xff,0xf8,0x00
,0x0f,0xff,0xfc,0x00
,0x0f,0xff,0xfe,0x00
,0x0f,0xff,0xff,0x00
,0x0f,0xff,0xff,0x80
,0x0f,0xff,0xff,0xc0
,0x0f,0xff,0xff,0xc0
,0x0f,0xff,0xff,0x80
,0x0f,0xff,0xff,0x00
,0x0f,0xff,0xfe,0x00
,0x0f,0xff,0xfc,0x00
,0x0f,0xff,0xf8,0x00
,0x00,0x01,0xf0,0x00
,0x00,0x01,0xe0,0x00
,0x00,0x01,0xc0,0x00
,0x00,0x01,0x80,0x00
,0x00,0x01,0x00,0x00
,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00
,0x00,0x00,0x00,0x00  };



void setup()
{
    pinMode(7, INPUT_PULLUP);      //stop button
    pinMode(6, INPUT_PULLUP);      //turn button
    pinMode(A0, INPUT);
    Serial.begin(38400);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    //pinMode(LED3, OUTPUT);
    //pinMode(LED4, OUTPUT);
    //pinMode(LED5, OUTPUT);
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    //digitalWrite(LED3, HIGH);
    //digitalWrite(LED4, HIGH);
    //digitalWrite(LED5, LOW);
    pinMode(speedLimiterPin, INPUT_PULLUP);

    rf24.begin();
    rf24.setChannel(100);       // 設定頻道編號
    rf24.openWritingPipe(addr); // 設定通道位址
    //rf24.setPALevel(RF24_PA_MIN);   // 設定廣播功率
     //rf24.setDataRate(RF24_250KBPS); // 設定傳輸速率
    rf24.stopListening();       // 停止偵聽；設定成發射模式

    // 偵測是否安裝好OLED了
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // 一般1306 OLED的位址都是0x3C
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }

    // 顯示Adafruit的LOGO，算是開機畫面
    display.display();
    delay(500); // 停1秒

    // 清除畫面
    display.clearDisplay();

    //testdrawstyles();    // 測試文字

    //testdrawbitmap();    // 顯示圖形

}

void loop()
{
    if(Serial.available() > 0) {
        data = Serial.parseInt();
        
    }

    if (digitalRead(7)== LOW) {
        stop=10000;
    }else if (digitalRead(7)== HIGH) {
        stop=0;
    }
    if (digitalRead(6) == LOW) {
        turn=20000;
    }else if (digitalRead(6) == HIGH) {
        turn=0;
    }

    int analogValue;
    


    if (firstTime || pressedStop) {
        if (analogRead(A0) > 0) {
            //do nothing
            display.drawBitmap(22, 16, s_sign, 15, 15, 1);
            display.drawBitmap(39, 16, d_sign, 15, 15, 1);
            //display.drawBitmap(56, 16, in_sign, 15, 15, 1);
            display.drawBitmap(73, 16, p_sign, 15, 15, 1);
            display.drawBitmap(90, 16, r_sign, 15, 15, 1);
            analogValue = 0;
        }
        else {
            analogValue = analogRead(A0);
            firstTime = false;
            pressedStop = false;
        }
    }
    else {
        analogValue = analogRead(A0);
        display.drawBitmap(56, 16, n_sign, 15, 15, 1);
    }

    if (digitalRead(speedLimiterPin) == LOW) {
        speedLimiterStatus = 0;
        analogValue = map(analogValue, 1, 1023, 1, 512); 
    }else if (digitalRead(speedLimiterPin) == HIGH) {
        speedLimiterStatus = 1;
        analogValue = constrain(analogValue, 1, 1023); 
    }
    speedLimiterStatusPrev = speedLimiterStatus;


    /*
    int i = 0;

    if (speedLimiterStatusPrev != speedLimiterStatus) {
        if (speedLimiterStatus == 1) {
            //slowly change max analogValue to 1023
            analogValue = analogRead(A0);
            analogValue = map(analogValue, 1, 1023, 1, 512); 
            if (analogValue < 1022) {
                analogValue = analogValue + i;
                i++;
            }


        }
                        
    }*/

    
    
    
    processedValue = analogValue + stop + turn;

    
    Serial.println(processedValue);
    //rf24.write(&msg, sizeof(msg));  // 傳送資料
    //Serial.println(" ");
    delay(15);
    //rf24 write processedValue
    rf24.write(&processedValue, sizeof(processedValue));  // 傳送資料

    display.clearDisplay();
    display.setTextSize(2);             // 設定文字大小
    display.setTextColor(1);        // 1:OLED預設的顏色(這個會依該OLED的顏色來決定)
    display.setCursor(0,0);             // 設定起始座標
    display.print(" ");
    display.print(map(analogValue,1,1010,0,20));        // 要顯示的字串
    //display.print(data);
    display.print(" KM/H");

    if (digitalRead(7)== LOW) {
        display.drawBitmap(73, 16, ip_sign, 15, 15, 1);
        pressedStop = true;
     }else if (digitalRead(7)== HIGH || firstTime == true) {
        display.drawBitmap(73, 16, p_sign, 15, 15, 1);
    }

    if (digitalRead(6) == LOW) {
        display.drawBitmap(90, 16, ir_sign, 15, 15, 1);
    }else if (digitalRead(6) == HIGH || firstTime == true) {
        display.drawBitmap(90, 16, r_sign, 15, 15, 1);
    }

    if (speedLimiterStatus == 1) {
        if (digitalRead(7)== LOW || digitalRead(6) == LOW || firstTime == true || pressedStop == true) {
            display.drawBitmap(22, 16, s_sign, 15, 15, 1);
        }else{
            display.drawBitmap(22, 16, is_sign, 15, 15, 1);
        }
    }else if (speedLimiterStatus == 0) {
        display.drawBitmap(22, 16, s_sign, 15, 15, 1);
    }

    if (digitalRead(7)== LOW || digitalRead(6) == LOW || speedLimiterStatus == 1 || firstTime == true || pressedStop == true) {
        display.drawBitmap(39, 16, d_sign, 15, 15, 1);
    }else if (digitalRead(7)== HIGH && digitalRead(6) == HIGH && speedLimiterStatus == 0 && firstTime == false) {
        display.drawBitmap(39, 16, id_sign, 15, 15, 1);
    }

    
    //display.drawBitmap(22, 16, s_sign, 15, 15, 1);
    //display.drawBitmap(56, 16, n_sign, 15, 15, 1);

    if (firstTime == true || pressedStop == true) {
        display.drawBitmap(56, 16, in_sign, 15, 15, 1);
    }else{
        display.drawBitmap(56, 16, n_sign, 15, 15, 1);
    }






        /*
        display.drawBitmap(22, 16, s_sign, 15, 15, 1);
        display.drawBitmap(39, 16, d_sign, 15, 15, 1);
        display.drawBitmap(56, 16, n_sign, 15, 15, 1);
        display.drawBitmap(73, 16, p_sign, 15, 15, 1);
        display.drawBitmap(90, 16, r_sign, 15, 15, 1);
        */
    display.display();                  // 要有這行才會把文字顯示出來

 
    /*
    if (digitalRead(10)== LOW) {
        hc06.println("stop");
    }
    if (digitalRead(11) == LOW) {
        hc06.println("turn");
    }
    hc06.println(analogRead(A0));
    delay(50);
    */
}

