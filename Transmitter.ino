//importing libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

//declaring initial values
int flameSensorValue = 0;
int gasSensorValue = 0;

//declaring digital pin numbers
int buzzerPin = 8;

//declaring pin 9 and pin 10 as rx and tx for serial communication
SoftwareSerial gsmSerial(9, 10);

//declaring lcd to be used
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);

void setup() {
  Serial.begin(9600); //start the serial at 9600 baud
  gsmSerial.begin(9600); //start the gsmSerial at 9600 baud
  pinMode(buzzerPin, OUTPUT); //declaring buzzerPin as output
  lcd.init(); //initialise the lcd display
  lcd.backlight(); //turn on the backlight of the display
}

void loop() {
  if (getFlameSensor() < 300) { //if flame sensor value is below 300
    Serial.println("A"); //send alphabet 'A' to the xBee receiver
    displayMessage("Flame detected!", "Message sent!"); //display a message on the lcd
    sendMessage("Flame detected !"); //send an sms with the message inside "..."
    beep(); //turn on the alarm
  } else if (getGasSensor() > 50) { //if the gas sensor value is over 50
    Serial.println("B"); //send alphabet 'B' to the xBee receiver
    displayMessage("Gas detected!", "Message sent!"); //display a message on the lcd
    sendMessage("Gas detected !"); //send an sms with the message inside "..."
    beep(); //turn on the alarm
  } else { //if no flame or gas is detected
    Serial.println("C"); //send alphabet 'C' to the xBee receiver
    lcd.clear(); //removes any messages on the lcd
  }

  delay(500); //add a delay of 500 milliseconds (to avoid the display and values from bugging)

}

//method to display a message on the lcd
void displayMessage(String firstLine, String secondLine) {
  lcd.clear(); //clear the lcd display
  lcd.setCursor(0, 0); //set the lcd cursor on 0,0
  lcd.print(firstLine); //display the message on the first line
  lcd.setCursor(0, 1); //set the lcd cursor on 0,1
  lcd.print(secondLine); //display the message on the second line
  delay(300); //add a 300 milliseconds delay
}

//method to turn on the alarm
void beep() {
  //for loop from 0 to 9 (10 times) - the buzzer will ring 10 times
  for (int i = 0; i < 10; i ++) {
    digitalWrite(buzzerPin, HIGH); //turn on the buzzer
    delay(200); //delay of 200 milliseconds
    digitalWrite(buzzerPin, LOW); //turn off the buzzer
    delay(200); //delay of 200 milliseconds
  }
  //check all the sensors - will repeat until flame or gas is over
  while (allSensorValue()) {
    digitalWrite(buzzerPin, HIGH); //turn on the buzzer
    delay(200); //delay of 200 milliseconds
    digitalWrite(buzzerPin, LOW); //turn off the buzzer
    delay(200); //delay of 200 milliseconds
  }
}

//method to read the flame sensor value
int getFlameSensor() {
  flameSensorValue = analogRead(A0); //read the flame sensor value from analog pin
  return flameSensorValue;
}

//method to read the gas sensor value
int getGasSensor() {
  gasSensorValue = analogRead(A1); //read the gas sensor value from analog pin
  return gasSensorValue;
}

//method to send an sms to the person
void sendMessage(String message) {
  gsmSerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milliseconds
  gsmSerial.println("AT+CMGS=\"+23058272235\"\r"); // Replace with any mobile number
  delay(1000); // Delay of 1000 milliseconds
  gsmSerial.println(message);// The SMS text you want to send
  delay(100); // Delay of 1000 milliseconds
  gsmSerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000); // Delay of 1000 milliseconds
}

//method to read all the sensors' data
bool allSensorValue() {
  //if either flame or gas is still present
  if ((analogRead(A0) < 500) || (analogRead(A1) > 50)) {
    return true; //return true
  } else return false; //if flame or gas is over, return false
}
