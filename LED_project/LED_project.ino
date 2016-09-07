#include <Adafruit_NeoPixel.h>

#define neoPinLeft 16
#define neoPinRight 17
#define togglePinIn 15

int brightness = 225;
//36
int leftMax = 36;
int rightMax = 36;
//bool toggle = false;
//toggle settings

int previous;
long time = 0;
long debounce = 200;
int currentColor = 1;
int numberOfColors = 4;
int maxEffects = 2;

int groupOne[3] = {0,leftMax, 'L'};
int groupTwo[3] = {0,rightMax, 'R'};


Adafruit_NeoPixel stripLeft = Adafruit_NeoPixel(leftMax, neoPinLeft, NEO_RGB);
Adafruit_NeoPixel stripRight = Adafruit_NeoPixel(rightMax, neoPinRight, NEO_RGB);
uint32_t red = stripLeft.Color(225, 0, 0);
uint32_t blue = stripLeft.Color(0, 225, 0);
uint32_t green = stripLeft.Color(0, 0, 225);
uint32_t off = stripLeft.Color(0, 0, 0);
uint32_t purple = stripLeft.Color(0, 120, 225);
uint32_t color;
//const int numOfGroups = 4;
//const int globalGroup[numOfGroups];

int testArray[2] = {0, leftMax};
int groupArray[2][3];

int masterArray[2][4];

int previousEffect;

int colorSave;

int currentWipeIndex;
int colorWipeDelay = 200;
int colorWipeDelaySingle = 70;

void setup() {

  Serial.begin(9600);

  pinMode(togglePinIn, INPUT_PULLUP);

  stripLeft.begin();
  stripLeft.show();

  stripRight.begin();
  stripRight.show();

  //force a state for init
  currentEffect = 2;
  effectsPick(false);
}

void loop() {

  buttonInput();

  if (currentEffect == 1 || currentEffect == 2); {
    effectsPick(true);
  }
  stripLeft.show();
  stripRight.show();
}

//group flag will return 76 and 82, not sure why even tho it's pulling the correct start and end values for the array index based on group, why?
int setGroup(int groupArray[2][4]) {
  for(int group = 0; group <= 1; group++){
    int currentPixleGroupStart = groupArray[group][0];
    int currentPixleGroupEnd = groupArray[group][1];
    int currentPixleSide = groupArray[group][2];
    //Serial.print("current Group " + String(group) + " | current group flag " + String(groupArray[group][3]) + " | currentPixleGroupStart " + String(currentPixleGroupStart) + " | currentPixleGroupEnd " + String(currentPixleGroupEnd) + " | currentPixleSide " + String(currentPixleSide) +"\n");
    //Serial.print("currentColor | " + String(currentColor) + "\n");
    for(int x = currentPixleGroupStart; x <= currentPixleGroupEnd ; x++) {
      if (groupArray[group][3] == 1) {
        setPixle(x, currentPixleSide); 
        //Serial.print(String(x) + "\n");   
      }
    }
    //groupArray[group][2] = 0;
  }
}

void setPixle(int pixelIndex, char side) {
  if (side == 'L')
    stripLeft.setPixelColor(pixelIndex, colorPick(currentColor));
  if (side == 'R')
    stripRight.setPixelColor(pixelIndex, colorPick(currentColor));
  if (side == 'B')
    stripLeft.setPixelColor(pixelIndex, colorPick(currentColor));
    stripRight.setPixelColor(pixelIndex, colorPick(currentColor));
}

void buttonInput(){
  bool reading = digitalRead(togglePinIn);
  int returnedButtonState = pressCheck(reading, previous);
  if (returnedButtonState != 4)
    //Serial.print(String(returnedButtonState) + "\n");
  //runs once returnedButtonState returns a change
  if (returnedButtonState != 4 && returnedButtonState != 2) {
   currentColor = setColor(returnedButtonState);
   effectsPick(true);
  } else if (returnedButtonState == 2) {
   effectsPick(false);
  }
  if (returnedButtonState == 0) { 
    currentEffect = 0;
    currentColor = 0;
  }
  
  previous = reading;
}

uint32_t colorPick(int x) {
  switch (x) {
    case 0: return stripLeft.Color(0, 0, 0); //off, needs to be first, hold button seems to defualt to 0
      break;
    case 1: return stripLeft.Color(0, 225, 0); //red
      break;
    case 2: return stripLeft.Color(0, 0, 225); //blue
      break;
    case 3: return stripLeft.Color(225, 0, 0); //green
      break;
    case 4: return stripLeft.Color(0, 120, 225); //purple
      break;
    case 5: return colorPick(2);
  }
}

void effectsPick(bool refresh) {
  //Serial.print("refresh | " + String(refresh) + "\n");                                              
  //Serial.print("currentEffectStart | " + String(currentEffect) + "\n");
  //Serial.print("previouseEffect | " + String(previousEffect) + "\n");
  if (!refresh) {
  clearStrip();
  currentEffect++;
  }

  if (currentEffect > maxEffects){
    currentEffect = 0;
  }

  switch (currentEffect) {
    case 0:
      if (!refresh) {

        masterArray[0][0] = groupOne[0];
        masterArray[0][1] = groupOne[1];
        masterArray[0][2] = groupOne[2];
        masterArray[0][3] = 1;

        masterArray[1][0] = groupTwo[0];
        masterArray[1][1] = groupTwo[1];
        masterArray[1][2] = groupTwo[2];
        masterArray[1][3] = 1;
      }
      setGroup(masterArray);
    break;
    case 1:  
      if (!refresh) {
        currentWipeIndex = 0;
        currentColor = 1;
      } else {
        setPixle(currentWipeIndex, 'B');
        currentWipeIndex++;
        if (currentWipeIndex > leftMax){
          currentWipeIndex = 0;
          setColor(1);
        }
        delay(colorWipeDelay);
      }
    break;
    case 2:
      Serial.print("currentWipeIndex |" + String(currentWipeIndex) + "\n"); 
      if (!refresh) {
        currentWipeIndex = 0;
        currentColor = 1;
      } else {
        setPixle(currentWipeIndex, 'B');
        if (currentWipeIndex != 0) {
          colorSave = currentColor;
          currentColor = 0;
          setPixle(currentWipeIndex-1, 'B');
          currentColor = colorSave;
        } else {
          colorSave = currentColor;
          currentColor = 0;
          setPixle(leftMax, 'B');
          currentColor = colorSave;
        }
        currentWipeIndex++;
        if (currentWipeIndex > leftMax){
          currentWipeIndex = 0;
          //setColor(1);
        }
        delay(colorWipeDelaySingle);
      }
    break;
  } 

  //Serial.print("currentEffectEnd | " + String(currentEffect) + "\n");
  previousEffect = currentEffect;
}

void clearStrip() {
  for(int x = 0; x < leftMax; x++){
    stripLeft.setPixelColor(x, colorPick(0));
  }
  for(int x = 0; x < rightMax; x++){
    stripRight.setPixelColor(x, colorPick(0));
  }
  stripLeft.show();
  stripRight.show();
}

//pressCheck will return what state the button is in based on how long it's been held down continusly, any break will reset the trimmer and return the current value it's on.
//-would like to find away to refactor the time logic-
int pressCheck(int reading, int previous){
  int buttonState;

  if (reading == previous && previous != 0 && time == 0) {
    time = millis();
  }
  
  if (time != 0 && (time + 1000) > millis()) {
    buttonState = 1;
  } 
  else if (time != 0 && (time + 1000) < millis() && (time != 0 && (time + 2000) > millis())){
    buttonState = 2;
  } else {
    buttonState = 0;
  }
  if (reading != previous) {
    time = 0;
  }

  //check to ensure that buttonstate changed before returning a value
  if (previous == 1 && reading == 0) {
    return buttonState;
  } else {
    return 4;
  }
}

//no longer in use, minor refactor attempt for pressCheck
bool buttonHold(int time, int pause){
  if (time != 0 && (time + pause) > millis()){
    return true;
  } else {
    return false;    
  }
}

int setColor(int currentButtonState) {
  switch(currentButtonState) {
      case 0: 
        currentColor = 0;
      break;
      case 1:
         currentColor++;
      break;
    }

  if (currentColor > numberOfColors){
    currentColor = 1;
  }
  
  return currentColor;
}


