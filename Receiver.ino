//importing libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <Stepper.h>

//define the number of steps of the stepper motor
#define STEPS 32

//declaring initial values
int received = 0;

//declaring digital pin numbers
int fanRelay = 12;

//declaring number of steps
int numberSteps = 1200;

//declaring stepper to be used
Stepper stepper (STEPS, 8, 10, 9, 11);

//declaring lcd to be used
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);

void setup() {
  Serial.begin(9600);  //start the serial at 9600 baud
  pinMode(fanRelay, OUTPUT);  //declaring fanRelay as output
  lcd.init(); //initialise the lcd display
  lcd.backlight(); //turn on the backlight of the display
  stepper.setSpeed(1000); //set the speed of the stepper motor to 1000
  digitalWrite(fanRelay, HIGH); //turn off the relay (turn off the extraction fan)
}

void loop() {
  //if Serial is available - if messages are being received by the xBee
  if (Serial.available() > 0) {

    //read the value of the Serial and save it in the variable 'received'
    received = Serial.read();

    if (received == 'A') { //if received is equal to alphabet 'A'
      displayMessage("Flame detected!", "At home"); //display a message on the lcd
    } else if (received == 'B') { //if received is equal to alphabet 'B'
      displayMessage("Gas detected!", "Evacuating Gas!"); //display a message on the lcd
      evacuateGas(10000); //evacuate the gas for 10000 milliseconds - turn on stepper motor and fan
    } else if (received == 'C') { //if received is equal to alphabet 'C'
      lcd.clear(); //removes any messages on the lcd
    }
  }
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

//method to evacuate gas
void evacuateGas(int interval) {
  startFan(); //start the relay to start the extraction fan
  closeValve(); //gas the gas valve
  delay(interval); //delay to keep the fan on and valve closed
  openValve(); //open the gas valve
  stopFan(); //stop the extraction fan
}

//method to close the gas valve
void closeValve() {
  stepper.step(numberSteps); //turn the stepper motor
}

//method to start the extraction fan
void startFan() {
  digitalWrite(fanRelay, LOW); //power on the relay to turn on the extraction fan
}

//method to open the gas valve
void openValve() {
  stepper.step(-numberSteps); //turn the stepper motor
}

//method to stop the extraction fan
void stopFan() {
  digitalWrite(fanRelay, HIGH); //power off the relay to turn off the extraction fan
}
