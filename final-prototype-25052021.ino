/*
The code below has been adapted from
https://randomnerdtutorials.com/arduino-color-sensor-tcs230-tcs3200/
https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-use
https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299
https://www.instructables.com/Capacitive-Sensing-for-Dummies/
I have changed variable types to output strings instead of numbers and adjusted the loop functionality
*/
//#include <SPI.h>


// color sensor setup
#define S0 50
#define S1 51
#define S2 52
#define S3 53
#define sensorOut 49
#define LED 47 
int R = 0;
int G = 0;
int B = 0;



int VIB_PIN = 6; // Vibration sensor port
const int FSR_PIN = A0; // Force sensor for squeezing
const int FSR_PIN1 = A1; // Force sensor port for squeezing
const int BTN_PIN = 3; //Button for hug
const int BTN_PIN1 = A2;


#include <PWMServo.h>
PWMServo servo1; // eyebrow
PWMServo servo2; // eyebrow
PWMServo servo3; // mouth 

#include <Adafruit_NeoPixel.h>
#define LEDPIN1 10 //Heart LED port
#define LEDPIN2 22 //Tear lED 
#define LEDPIN3 23 //Tear LED
#define LEDPIN4 24 //Sweat LED
#define LEDPIN5 25 //Disgust LED 
#define LEDPIN6 26 //Angry LED
#define LEDPIN7 27 //Surprise LED
#define NUMPIXELS 49

Adafruit_NeoPixel ledstrip1(NUMPIXELS, LEDPIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ledstrip2(NUMPIXELS, LEDPIN2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ledstrip3(NUMPIXELS, LEDPIN3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ledstrip4(NUMPIXELS, LEDPIN4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ledstrip5(NUMPIXELS, LEDPIN5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ledstrip6(NUMPIXELS, LEDPIN6, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ledstrip7(NUMPIXELS, LEDPIN7, NEO_GRB + NEO_KHZ800);

#include <FastCapacitiveSensor.h>
const int CON1_PIN = A3;
const int CON2_PIN = A4;
FastCapacitiveSensor sensor;


//#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

//SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
//Serial1(19,18); //RX, TX
DFRobotDFPlayerMini myDFPlayer;

void setup(){
  Serial1.begin(9600);
  Serial.begin(9600);
  
  //Serial.println();
  //Serial.println(F("DFRobot DFPlayer Mini Demo"));
  //Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(Serial1)) {  //Use softwareSerial to communicate with mp3.
    //Serial.println(F("Unable to begin:"));
    //Serial.println(F("1.Please recheck the connection!"));
    //Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  //Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.volume(22);  //Set volume value. From 0 to 30
  myDFPlayer.play(1);  //Play the first mp3
 
  
  pinMode(FSR_PIN, INPUT);
  pinMode(FSR_PIN1, INPUT);
  pinMode(VIB_PIN, INPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(BTN_PIN1, INPUT_PULLUP);

  pinMode(CON1_PIN, OUTPUT);
  pinMode(CON2_PIN, INPUT);
  sensor.begin(CON1_PIN, CON2_PIN, 5.0, 10, 10, 0.2);
  
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  digitalWrite(LED,HIGH);
  
  servo1.attach(13); //eyebrow
  servo2.attach(12);  //eyebrow
  servo3.attach(11);  //mouth

  
  ledstrip1.begin();
  ledstrip1.show();
  ledstrip1.setBrightness(50);

  ledstrip2.begin();
  ledstrip2.show();
  ledstrip2.setBrightness(50);

  ledstrip3.begin();
  ledstrip3.show();
  ledstrip3.setBrightness(50);
  
  ledstrip4.begin();
  ledstrip4.show();
  ledstrip4.setBrightness(50);

  ledstrip5.begin();
  ledstrip5.show();
  ledstrip5.setBrightness(50);

  ledstrip6.begin();
  ledstrip6.show();
  ledstrip6.setBrightness(50);

  ledstrip7.begin();
  ledstrip7.show();
  ledstrip7.setBrightness(50);
  
  //happy face initial
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);
  
}

bool pressureDetected = false;
bool shakeDetected = false;

int buttonState = 0;
int buttonState1 = 0;
int emotion = 0;

// led light strip commend (color lighting)
void colorWipe1(uint32_t color, int wait)
{
   for(int i=0; i<ledstrip1.numPixels(); i++)
  {
    ledstrip1.setPixelColor(i, color);
    ledstrip1.show();
    delay(wait);
  }
  
}

void colorWipe2(uint32_t color, int wait)
{
  for(int i=0; i<ledstrip2.numPixels(); i+=2)
  {
    ledstrip2.setPixelColor(i, color);
    ledstrip2.show();
    delay(wait);
  }
}

void colorWipe3(uint32_t color, int wait)
{
  for(int i=0; i<ledstrip3.numPixels(); i++)
  {
    ledstrip3.setPixelColor(i, color);
    ledstrip3.show();
    delay(wait);
  }
}

void colorWipe4(uint32_t color, int wait)
{
  for(int i=0; i<ledstrip4.numPixels(); i++)
  {
    ledstrip4.setPixelColor(i, color);
    ledstrip4.show();
    delay(wait);
  }
}

void colorWipe5(uint32_t color, int wait)
{
  for(int i=0; i<ledstrip5.numPixels(); i++)
  {
    ledstrip5.setPixelColor(i, color);
    ledstrip5.show();
    delay(wait);
  }
}

void colorWipe6(uint32_t color, int wait)
{
  for(int i=0; i<ledstrip6.numPixels(); i++)
  {
    ledstrip6.setPixelColor(i, color);
    ledstrip6.show();
    delay(wait);
  }
}

void colorWipe7(uint32_t color, int wait)
{
  for(int i=0; i<ledstrip7.numPixels(); i++)
  {
    ledstrip7.setPixelColor(i, color);
    ledstrip7.show();
    delay(wait);
  }
}

void clearlight() {
    //colorWipe1(ledstrip1.Color(0,0,0),0);
    colorWipe2(ledstrip2.Color(0,0,0),0);
    colorWipe3(ledstrip3.Color(0,0,0),0);
    colorWipe4(ledstrip4.Color(0,0,0),0);
    colorWipe5(ledstrip5.Color(0,0,0),0);
    colorWipe6(ledstrip6.Color(0,0,0),0);
    colorWipe7(ledstrip7.Color(0,0,0),0);
}

void happyexp(){
    clearlight();
    colorWipe1(ledstrip1.Color(255,255,0),0);
//    colorWipe2(ledstrip2.Color(0,0,0),0);
//    colorWipe3(ledstrip3.Color(0,0,0),0);
//    colorWipe4(ledstrip4.Color(0,0,0),0);
//    colorWipe5(ledstrip5.Color(0,0,0),0);
//    colorWipe6(ledstrip6.Color(0,0,0),0);
//    colorWipe7(ledstrip7.Color(0,0,0),0);
    shakeDetected = false;
    pressureDetected = false;
    
    servo1.write(0);
    servo2.write(0);
    servo3.write(0);
    
    emotion = 1;
}

void happy(){
  
  if (R >= 15 && R <= 40 && G >= 15 && G <= 45) {
     happyexp();
     myDFPlayer.play(3);
//  }else if (R >= 28 && R <= 40 && G >= 15 && G <= 25) {
//     happyexp(); 
     
  }
  
}

void comfort(){

  buttonState1 = digitalRead(BTN_PIN1);
  if (buttonState1 == LOW) {
     happyexp();
     //delay(400);
  } 
//  if (sensor.touch() < 500) {
//    happyexp();
//  }
//  Serial.println(sensor.touch());
}

void sad(){
  if (R > 50) {
    
    // Serial.println("Blue - Sad");
    emotion = 2;
    servo2.write(160);
    servo1.write(30);
    servo3.write(180);
    
    colorWipe1(ledstrip1.Color(0,0,0),0);
    colorWipe2(ledstrip2.Color(0,0,255),0);
    colorWipe3(ledstrip3.Color(0,0,255),0);
    colorWipe4(ledstrip4.Color(0,0,0),0);
    colorWipe5(ledstrip5.Color(0,0,0),0);
    colorWipe6(ledstrip6.Color(0,0,0),0);
    colorWipe7(ledstrip7.Color(0,0,0),0);
 
    myDFPlayer.play(4);
    
  }
 
}

void angry() {
  int fsrADC1 = analogRead(FSR_PIN);
  int fsrADC2 = analogRead(FSR_PIN1);
  
  if (fsrADC1 > 200 or fsrADC2 > 200) {
      //Serial.println("pressed");
      pressureDetected = true;
      
      colorWipe1(ledstrip1.Color(0,0,0),0);
      colorWipe2(ledstrip2.Color(0,0,0),0);
      colorWipe3(ledstrip3.Color(0,0,0),0);
      colorWipe4(ledstrip4.Color(0,0,0),0);
      colorWipe5(ledstrip5.Color(0,0,0),0);
      colorWipe6(ledstrip6.Color(255, 0, 0), 10);
      colorWipe7(ledstrip7.Color(0,0,0),0);
      
      servo2.write(35);
      servo1.write(155);
      servo3.write(180);

      myDFPlayer.play(5);
  }
}

void surprise() {
  if (R <= 50 && R >= 30 && G <= 75 && G >= 55 && B<= 40 && B >= 28) {

    colorWipe1(ledstrip1.Color(0,0,0),0);
    colorWipe2(ledstrip2.Color(0,0,0),0);
    colorWipe3(ledstrip3.Color(0,0,0),0);
    colorWipe4(ledstrip4.Color(0,0,0),0);
    colorWipe5(ledstrip5.Color(0,0,0),0);
    colorWipe6(ledstrip5.Color(0,0,0),0);
    colorWipe7(ledstrip7.Color(255,255,0),0);

    myDFPlayer.play(7);
    
    servo1.write(0);
    servo2.write(0);
    servo3.write(0); 
  }
}

int measurement = 0;
void scared() {
  measurement = digitalRead(VIB_PIN);
  Serial.println(measurement);
  if (measurement == 0 ) {
    shakeDetected = true;
    colorWipe1(ledstrip1.Color(0,0,0),0);
    colorWipe2(ledstrip2.Color(0,0,0),0);
    colorWipe3(ledstrip3.Color(0,0,0),0);
    colorWipe4(ledstrip4.Color(255,255,255),0);
    colorWipe5(ledstrip5.Color(0,0,0),0);
    colorWipe6(ledstrip6.Color(0,0,0),0);
    colorWipe7(ledstrip7.Color(0,0,0),0);

    delay(50);
    servo1.write(0);
    servo2.write(0);
    servo3.write(180); 
    
    myDFPlayer.play(6);
  }
}

bool disgustDetected = false;
void disgust(){
  //R <= 60 && R >= 45 && 
  if (G <= 70 && G >= 40 && B <= 65 && B >= 40) {
     colorWipe1(ledstrip1.Color(0,0,0),0);
     colorWipe2(ledstrip2.Color(0,0,0),0);
     colorWipe3(ledstrip3.Color(0,0,0),0);
     colorWipe4(ledstrip4.Color(0,0,0),0);
     colorWipe5(ledstrip5.Color(0,255,0),0);
     colorWipe6(ledstrip6.Color(0,0,0),0);
     colorWipe7(ledstrip7.Color(0,0,0),0);
     
     servo2.write(160);
     servo1.write(30);
     servo3.write(180); 
     
     emotion = 3;
     myDFPlayer.play(2);  
     
  } 
}
void getRead(){

  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  R = pulseIn(sensorOut, LOW, 500);
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  G = pulseIn(sensorOut, LOW, 500);
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  B = pulseIn(sensorOut, LOW, 500);
  
  Serial.println(R);
  Serial.println(G);
  Serial.println(B);

}

int pos = 0;

void loop(){

//  Serial.println(fsrADC1);
//  Serial.println(fsrADC2);
  
  
  buttonState = digitalRead(BTN_PIN);
  if (buttonState == LOW) {
    getRead();
    happy();
    sad();
    surprise();
    disgust();
  }
  
  comfort();
  scared();
  angry();
    
    
  
    
}  
