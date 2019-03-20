#include <Servo.h>

Servo myservoLeft; 
Servo myservoRight;

int PIR = 2;               // choose the input pin (for PIR sensor)
const int greenLED =  4;
const int redLED =  5;
const int button = 6;
const int servoLeft = 9;
const int servoRight = 10;
const int buzzer =  12;
int ledPin = 13;                // choose the pin for the LED

int val;    //varijabla za ocitanje tipke
bool open = false;
bool buttonPress = false;
int buttonPressed = 0;
int light = 0;

int pirState = LOW;             // we start, assuming no motion detected
int PIRval = 0;                    // variable for reading the pin status

// lowest and highest sensor readings:
const int sensorMin = 0;     // sensor minimum
const int sensorMax = 1024;  // sensor maximum
int sensorReading;
int range;

int left = 822;
int right = 2171;

void setup() {
  Serial.begin(9600);
  myservoLeft.attach(servoLeft);  // attaches the servo on pin 9 to the servo object
  myservoRight.attach(servoRight);  // attaches the servo on pin 10 to the servo object
  pinMode(A2,INPUT);
  pinMode(button, INPUT);    //koristimo pull-down otpornik - pritisak tipke oznacava HIGH
  pinMode(greenLED, OUTPUT);  //koristimo otpornik od 220 ohma
  pinMode(redLED, OUTPUT);  //koristimo otpornik od 220 ohma
  pinMode(buzzer, OUTPUT);  //buzzer  
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(PIR, INPUT);     // declare sensor as input 
  //pocetne pozicije servo motora
  myservoLeft.writeMicroseconds(left);
  myservoRight.writeMicroseconds(right);
}

void loop() {
  //READ MOTION
  PIRval = digitalRead(PIR);
  if (PIRval == HIGH) {
    digitalWrite(ledPin, HIGH);
    if (pirState == LOW) { // we have just turned on      
      Serial.println("Motion detected!"); // We only want to print on the output change, not state      
      pirState = HIGH;
    }
  }
  else{
    digitalWrite(ledPin, LOW);
    if (pirState == HIGH){  // we have just turned of      
      Serial.println("Motion ended!");  // We only want to print on the output change, not state      
      pirState = LOW;
    }
  }  
  /****************************READ BUTTON****************************************************/
  val = digitalRead(6);
  if(val==HIGH){
    buttonFunction();
    }
  digitalWrite(buzzer, LOW);
  //CHECH WEATHER CONDITIONS
  if(buttonPressed % 2 == 0 && !buttonPress){
    light = analogRead(A2);
    sensorReading = analogRead(A0);
    Serial.print(F("Ocitanja sa senzora: ")); Serial.print(F("K+kolicina svijetla (")); Serial.print(light);
    range = map(sensorReading, sensorMin, sensorMax, 0, 3);
    while( (light < 400 || range == 0 || range == 1) && !open){
        Serial.print(F("Mod tende: Otvorena - ")); Serial.print(F("K+kolicina svijetla (")); Serial.print(light);
        openAwning();
        delay(1000);
        open = !open;
     }
     while( light > 400 && range == 2 && open){
        Serial.print(F("Mod tende: Zatvorena - ")); Serial.print(F("kolicina svijetla (")); Serial.print(light); Serial.println(F(") + Not Raining"));
        closeAwning();
        delay(1000);
        open = !open;
      }
      switch (range) {
       case 0:    // Sensor getting wet
          Serial.println(F(") + Flood"));
          break;
       case 1:    // Sensor getting wet
          Serial.println(F(") + Rain Warning"));
          break;
       case 2:    // Sensor dry
          Serial.println(F(") + Not Raining"));
          break;
       }
  }  
  delay(200);                         
}

void buttonFunction(){
  Serial.println(F("Tipka je pritisnuta."));
  digitalWrite(buzzer, HIGH);
  if(!open){  //prvi pritisak
    digitalWrite(greenLED, HIGH); digitalWrite(redLED, LOW);
    openAwning();
    open = !open;
    buttonPress = true;
    }
  else if(open){  //drugi pritisak
    digitalWrite(greenLED, LOW); digitalWrite(redLED, HIGH);
    closeAwning();
    open = !open;
    buttonPress = false;
    digitalWrite(greenLED, LOW); digitalWrite(redLED, LOW);
    }
  }

void openAwning(){
    for( int i = 0; i < 110; i++){
    left = left + 2;
    right = right - 3;
    myservoLeft.writeMicroseconds(left);
    myservoRight.writeMicroseconds(right);
    Serial.print(left);  Serial.print('\t');  Serial.println(right);
    delay(50);
    }
  }
  
void closeAwning(){
    for( int i = 0; i < 110; i++){
    left = left - 2;
    right = right + 3;
    myservoLeft.writeMicroseconds(left);
    myservoRight.writeMicroseconds(right);
    Serial.print(left);  Serial.print('\t');  Serial.println(right);
    delay(50);
    }
  }
