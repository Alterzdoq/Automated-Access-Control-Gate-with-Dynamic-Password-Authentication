#include <Servo.h>

//initialize variables
String password = "";
String guess = "";

bool passwordSet = false; 
bool sensor = false;

const int trigPin = 6;
const int echoPin = 5;
long duration;
int distance;

Servo myServo; 
const int servoPin = 13;

void turnOnSensor(){ //turns on ultrasonic sensor (distance in cm)
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; //.034 is speed of sound (cm/microseconds), 2 because it sound wave goes from sensor --> target --> back to sensor
  if(distance <= 6){
    distance = 6;
  }
  if(distance >= 50){
    distance = 50;
  }
  delay(500);
}

void setup(){ //initialize pins
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP); 
  pinMode(12, INPUT_PULLUP);

  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);  

  myServo.attach(servoPin);
  myServo.write(90); //start at 90 degrees for servo balance
  
  Serial.println("Please enter your new password using buttons 1-4.");
}

void loop(){
  if(digitalRead(8) == 0 && !passwordSet){ //if slide switch is toggled and password is not locked in, reset password
    while(digitalRead(8) == 0){}
    password = "";
    guess = "";
    Serial.println("Password reset!");
    passwordSet = false;
    sensor = false;
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
  }
  if(!passwordSet){
    digitalWrite(11, LOW); 
    if(digitalRead(2) == 0){
      while(digitalRead(2) == 0){} 
      delay(50);
      password += "1";
      Serial.print("Password: "); 
      Serial.println(password);
    }
    if(digitalRead(4) == 0){
      while(digitalRead(4) == 0){} 
      delay(50);
      password += "2";
      Serial.print("Password: "); 
      Serial.println(password);
    }
    if(digitalRead(7) == 0){
      while(digitalRead(7) == 0){} 
      delay(50);
      password += "3";
      Serial.print("Password: "); 
      Serial.println(password);
    }
    if(digitalRead(12) == 0){
      while(digitalRead(12) == 0){} 
      delay(50);
      password += "4";
      Serial.print("Password: "); 
      Serial.println(password);
    }
    if(digitalRead(3) == 0){ // Press button to submit and lock in the password
      while(digitalRead(3) == 0){} 
      delay(50);
      if(password.length() > 3){
        passwordSet = true; // LOCKS user out of password mode 
        Serial.println("Password locked in!");
        Serial.println("\nPassword phase closed. Now enter your guess.");
      } 
      else{
        Serial.println("Password must be greater than 3 characters.");
      }
    }
  } 
  else{
    digitalWrite(11, HIGH); // Yellow LED changes state to indicate guess mode
    if(digitalRead(2) == 0){
      while(digitalRead(2) == 0){} 
      delay(50);
      guess += "1";
      Serial.print("Password guess: "); 
      Serial.println(guess);
    }
    if(digitalRead(4) == 0){
      while(digitalRead(4) == 0){} 
      delay(50);
      guess += "2";
      Serial.print("Password guess: "); 
      Serial.println(guess);
    }
    if(digitalRead(7) == 0){
      while(digitalRead(7) == 0){} 
      delay(50);
      guess += "3";
      Serial.print("Password guess: "); 
      Serial.println(guess);
    }
    if(digitalRead(12) == 0){
      while(digitalRead(12) == 0){} 
      delay(50);
      guess += "4";
      Serial.print("Password guess: "); 
      Serial.println(guess);
    }
    if(digitalRead(3) == 0){ // Press button to check if the guess matches the set password
      while(digitalRead(3) == 0){} 
      delay(50);  
      if(password == guess){
        digitalWrite(10, HIGH); // Green LED On
        digitalWrite(9, LOW); // Red LED Off
        sensor = true;
        Serial.println("Access granted! Sensor activated.");
      }
      else{ 
        digitalWrite(10, LOW);  
        digitalWrite(9, HIGH);  
        sensor = false;
        Serial.println("Access denied. Try again.");
        guess = "";             
      }
    }
  }
  if(sensor){
    turnOnSensor(); 
    // Serial.print("Distance: "); //prints distance ultrasonic sensor reads
    // Serial.println(distance);
    if(distance <= 10){ //open gate, wait 5 seconds, close gate, move onto new password generation
      myServo.write(0);
      Serial.println("Gate open!");
      delay(5000); 
      myServo.write(90);
      Serial.println("Gate closed!");
      password = "";
      guess = "";
      Serial.println("Please enter your new password using buttons 1-4.");
      passwordSet = false;
      sensor = false;
      digitalWrite(10, LOW);
      digitalWrite(9, LOW);
    } 
  }
  else{
    myServo.write(90);
  }
  delay(10);
}