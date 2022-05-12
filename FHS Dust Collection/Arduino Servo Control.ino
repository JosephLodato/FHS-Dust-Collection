//Keypad Parameters
#include <Keypad.h>
const byte rows = 4; //four rows
const byte cols = 3; //three columns, we ignore the letters on the side as we dont need them
char keys[rows][cols] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'#', '0', '*'}
};
byte rowPins[rows] = {8, 7, 6, 5};
byte colPins[cols] = {4, 3, 2};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

//Servo Parameters
#include <Servo.h>

Servo cncServo;
Servo sanderServo;
Servo drillServo;
Servo floorServo;
Servo chopServo;

#define cncPin 13
#define sanderPin 12
#define drillPin 11
#define floorPin 9
#define chopPin 10

int openAngle = 180;
int closedAngle = 0;

bool cncOpen = false;
bool sanderOpen = false;
bool drillOpen = false;
bool floorOpen = false;
bool chopOpen = false;



void setup() {
  Serial.begin(9600); //sets bit rate (9600) and starts communitcation with the serial monitor
  cncServo.attach(cncPin);
  sanderServo.attach(sanderPin); // attaches the defined servos to their defined pins
  drillServo.attach(drillPin);
  floorServo.attach(floorPin);
  chopServo.attach(chopPin);

}

void loop() {
  char keyPressed = keypad.getKey(); // Checks for a key press

  if (keyPressed) {
    Serial.println(keyPressed); // log key press to attached computer (for debuging)


    if (keyPressed == '1') {
      if (floorOpen == false) { // checks to see if gate is already open
        closeAllGates();  //close all other gates
        floorServo.write(openAngle); // opens this gate
        floorOpen = true; // to keep track of what gates are open
      } else {
        floorServo.write(closedAngle); // Close this gate if it was alreaedy open at the time of keypress
        floorOpen = false;
      }
    }

    if (keyPressed == '2') {  //Same as all others, see the first if statemnent for explanation
      if (chopOpen == false) {
        closeAllGates();
        chopServo.write(openAngle);
        chopOpen = true;
      } else {
        chopServo.write(closedAngle);
        chopOpen = false;
      }
    }

    if (keyPressed == '3') {  //Same as all others, see the first if statemnent for explanation
      if (drillOpen == false) {
        closeAllGates();
        drillServo.write(openAngle);
        drillOpen = true;
      } else {
        drillServo.write(closedAngle);
        drillOpen = false;
      }
    }

    if (keyPressed == '4') {  //Same as all others, see the first if statemnent for explanation
      if (sanderOpen == false) {
        closeAllGates();
        sanderServo.write(openAngle);
        sanderOpen = true;
      } else {
        sanderServo.write(closedAngle);
        sanderOpen = false;
      }
    }

    if (keyPressed == '5') {  //Same as all others, see the first if statemnent for explanation
      if (cncOpen == false) {
        closeAllGates();
        cncServo.write(openAngle);
        cncOpen = true;
      } else {
        cncServo.write(closedAngle);
        cncOpen = false;

      }
    }

    //Special cases:
    
    //actually runs when the '*' symbol is pressed.
    if (keyPressed == '#') { // These keypads give the the '#' symobol when you press '*', so I adjusted it with code
      cncServo.write(openAngle);
      delay(1500);
      sanderServo.write(openAngle);
      delay(1500);
      drillServo.write(openAngle);
      delay(1500);
      floorServo.write(openAngle);
      delay(1500);
      chopServo.write(openAngle);

      cncOpen = true;
      sanderOpen = true;
      drillOpen = true;
      floorOpen = true;
      chopOpen = true;
    }

     //actually runs when the '#' symbol is pressed.
    if (keyPressed == '*') { // These keypads give the the '#' symobol when you press '*', so I adjusted it with code
      closeAllGates();
    }
  }
}



void closeAllGates() { //function to close all the gates in a way that does not overload PSU
  bool arrayOfGateStatus[5] = {cncOpen, sanderOpen, drillOpen, floorOpen, chopOpen}; //So we can loop through all the gate status
  int numGatesOpen = 0;
  
  for (byte i = 0; i < 5; i++){ // Loops through all gates, for each that is open add one to numGatesOpen
    if(arrayOfGateStatus[i] == true){
      numGatesOpen++; 
    }
  }
  
  if(numGatesOpen <= 1){ // if only one gate is open we can just close that one by sending a close command to everything, the already closed ones will not move
    cncServo.write(closedAngle);
    sanderServo.write(closedAngle);
    drillServo.write(closedAngle);
    floorServo.write(closedAngle);
    chopServo.write(closedAngle);

    cncOpen = false;  //update to make sure all gate status is correct
    sanderOpen = false;
    drillOpen = false;
    floorOpen = false;
    chopOpen = false;
  }
  else{  // if more then 1 gate is open we must add a delay between gates so we dont draw more then 2 amps from the PSU 
    cncServo.write(closedAngle);
    delay(1500); 
    sanderServo.write(closedAngle);
    delay(1500); 
    drillServo.write(closedAngle);
    delay(1500);
    floorServo.write(closedAngle);
    delay(1500);
    chopServo.write(closedAngle);

    cncOpen = false;
    sanderOpen = false;
    drillOpen = false;
    floorOpen = false;
    chopOpen = false;
  }


}
