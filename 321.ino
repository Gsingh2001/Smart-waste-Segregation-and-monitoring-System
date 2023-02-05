#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLkpUFwVcg"
#define BLYNK_DEVICE_NAME "Quickstart Template"

#include <BlynkSimpleShieldEsp8266.h>
#include <SimpleTimer.h>
char auth[] = "gvi6ta4DTCjMG06HCMz8WW9jRtJQUkzF";
char ssid[] = "Honor";
char pass[] = "12345678";
SimpleTimer timer;
#define ESP8266_BAUD 115200
ESP8266 wifi(&Serial);
////////////////////////////////////////////
#include <LiquidCrystal.h>
LiquidCrystal lcd(8,9,10,11,12,13);//RS,EN,D4,D5,D6,D7
#include <Servo.h>
#define ir1 A1
#define sen1 A0 // moisture
#define buzzer 2
#define trigPin1 A4////right
#define echoPin1 A5

#define trigPin2 A2  //// front
#define echoPin2 A3

#define green 4
#define blue 5
int lvl1=0;
 int lvl2=0;
long duration, distance,sensor1,sensor2; // us variable
int onetime=0,onetime1=0 ;
int wet=0,moisture=0,object=0,cabin2=0,c1=0,c2=0;
Servo myservo; 
void setup() 
{
 lcd.begin(16, 2);//initializing LCD
 lcd.setCursor(0,0); 
 lcd.print("Automatic WASTE");
 lcd.setCursor(0,1); 
 lcd.print("Segregation sys");
 delay(3000);
 pinMode(ir1,INPUT);
 pinMode(sen1,INPUT);
 pinMode(buzzer,OUTPUT);
 pinMode(trigPin1, OUTPUT);
 pinMode(echoPin1, INPUT);
 pinMode(trigPin2, OUTPUT);
 pinMode(echoPin2, INPUT);
 pinMode(green, OUTPUT);
 pinMode(blue, OUTPUT);
 digitalWrite(green,HIGH);
 digitalWrite(blue,HIGH);
 myservo.attach(6);  // attaches the servo on pin  to the servo object
 myservo.write(75);
 Serial.begin(ESP8266_BAUD);
 delay(10);
 lcd.clear();
 lcd.print("connecting.....");  
 Blynk.begin(auth, wifi, ssid, pass);
 timer.setInterval(2000L, sendSensor); 
 lcd.clear();
 lcd.print("connected....."); 
 delay(1000); 
 lcd.clear();
}

void loop() 
{
 Blynk.run();
 timer.run();
 moisture=analogRead(sen1);
 Serial.print("moisture = ");
 Serial.println(moisture);
 delay(500);
 //////////////////////////////////////////////////////
 ultrasensor(trigPin1, echoPin1);
 sensor1 = distance;
 delay(10);
 ultrasensor(trigPin2, echoPin2);
 sensor2 = distance;    
 delay(10);
 Serial.println(sensor1);
 Serial.println(sensor2);
 lvl1=(17-sensor1)*6.5;
 lvl2=(17-sensor2)*6.5;
 Serial.println(lvl1);
 Serial.println(lvl2);
 if(lvl1<0){lvl1=0;}
 if(lvl2<0){lvl2=0;}
 if(lvl1>100){lvl1=100;}
 if(lvl2>100){lvl2=100;}
 if(lvl1>70){Blynk.notify("dustbin is full:- id-0001");}
 if(lvl2>70){Blynk.notify("dustbin is full:- id-0001");}
 lcd.clear();
  lcd.setCursor(0,0); 
  lcd.print("moisture: ");
  lcd.setCursor(12,0); 
  lcd.print(moisture);
  lcd.setCursor(0,1); 
  lcd.print("DL= ");
  lcd.setCursor(4,1); 
  lcd.print(lvl1);
  lcd.setCursor(8,1); 
  lcd.print("WL= ");
  lcd.setCursor(13,1); 
  lcd.print(lvl2);
 delay(1000);
 
 /////////////////////////////////////////////////////
 object=digitalRead(ir1);
 moisture=analogRead(sen1);
 if(object==LOW)
 {
  lcd.clear();
  moisture=analogRead(sen1);
  lcd.setCursor(0,0); 
  lcd.print("-GARBAGE SENSE-");
  lcd.setCursor(0,1); 
  lcd.print("moisture: ");
  lcd.setCursor(12,1); 
  lcd.print(moisture);
  digitalWrite(buzzer,HIGH);
  delay(150);
  digitalWrite(buzzer,LOW);
  delay(1000);
  moisture=analogRead(sen1);
  Serial.print("moisture = ");
  Serial.println(moisture);
  delay(500);
  moisture=analogRead(sen1);
  Serial.print("moisture = ");
  Serial.println(moisture);
  delay(500);
  if(moisture>=40)                         
  {
   lcd.clear();
   lcd.setCursor(0,0); 
   lcd.print(" WET  GARBAGE");
   myservo.write(0);
   delay(5000);
   myservo.write(75);
   delay(2000);
   Serial.println("wet");
  }
   else if(moisture<100)
   {  
    lcd.clear();
    lcd.setCursor(0,0); 
    lcd.print(" dry  GARBAGE");
    myservo.write(135); 
    delay(5000);
    myservo.write(75);
    delay(2000);
    Serial.println("moisture");
    lcd.clear();
   }    
  }    
 }
void sendSensor()
{ 
 Blynk.virtualWrite(V5, lvl1); // Humidity for gauge
 Blynk.virtualWrite(V6, lvl2); // Temperature for gauge
}
 void ultrasensor(int trigPin,int echoPin)
 { 
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
 }
