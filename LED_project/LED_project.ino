#include <Adafruit_NeoPixel.h>

#define neoPinLeft 16
#define neoPinRight 17
#define togglePinIn 15

int brightness = 225;
int leftMax = 36;
int rightMax = 36;
//bool toggle = false;
//toggle settings

int previous;
long time = 0;
long debounce = 200;
int currentColor = 1;
int numberOfColors = 4;


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


void setup() {

  Serial.begin(9600);

  pinMode(togglePinIn, INPUT_PULLUP);
  //setting Group ranges
  //globalGroup[1] = [startOfGroupOne, endOfGroupOne]

  stripLeft.begin();
  stripLeft.show();

  stripRight.begin();
  stripRight.show();

  groupArray[0][0] = testArray[0];
  groupArray[0][1] = testArray[1];
  groupArray[0][2] = 0;
  groupArray[1][0] = testArray[0];
  groupArray[1][1] = testArray[1];
  groupArray[1][2] = 0;
}

void loop() {

  buttonInput();
  setPixle(groupArray);


  stripLeft.show();
  stripRight.show();
  /*
  if (currentColor == 5) {
    colorWipe(colorPick(currentColor),10);
  }
  else {
  
    for (int i = 0; i < leftMax; i++) {
      stripLeft.setPixelColor(i, colorPick(currentColor));
    }
    for (int i = 0; i < rightMax; i++) {
      stripRight.setPixelColor(i, colorPick(currentColor));
    }
    stripLeft.setBrightness(brightness);
    stripRight.setBrightness(brightness);
    stripLeft.show();
    stripRight.show();
  }
  */
}

int setPixle(int groupArray[3][3]) {
  for(int group = 0; group <= 1; group++){
    int currentPixleGroupStart = groupArray[group][0];
    int currentPixleGroupEnd = groupArray[group][1];
    //Serial.print("current Group " + String(group) + " | current group flag " + String(groupArray[group][2]) + " | currentPixleGroupStart " + String(currentPixleGroupStart) + " | currentPixleGroupEnd " + String(currentPixleGroupEnd) + "\n");
    Serial.print("currentColor | " + String(currentColor) + "\n");
    for(int x = currentPixleGroupStart; x <= currentPixleGroupEnd ; x++) {
      if (groupArray[group][2] == 1) {
        setPixle(x); 
        Serial.print(String(x) + "\n");   
      }
    }
    groupArray[group][2] = 0;
  }
}

void setPixle(int pixelIndex) {
  stripLeft.setPixelColor(pixelIndex, colorPick(currentColor));
}

void buttonInput(){
  bool reading = digitalRead(togglePinIn);
  int returnedButtonState = pressCheck(reading, previous);

  //runs once returnedButtonState returns a change
  if (returnedButtonState != 4 && returnedButtonState != 2) {
   currentColor = setColor(returnedButtonState);
   effectsPick(true);
  } else if (returnedButtonState == 2) {
   effectsPick(false);
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
  int currentEffect;
  Serial.print("currentEffect | " + String(currentEffect) + "\n");
  if (!refresh) {
  currentEffect++;
  }
  switch (currentEffect) {
    case 0:
      groupArray[0][2] = 1;
      groupArray[1][2] = 1;
    break;
  } 
  if (currentEffect > 1){
    currentEffect = 0;
  }
  currentEffect = currentEffect;
}

//pressCheck will return what state the button is in based on how long it's been held down continusly, any break will reset the trimmer and return the current value it's on.
//-would like to find away to refactor the time logic-
int pressCheck(int reading, int previous){
  int buttonState;
  uint32_t test = buttonHold(time,2000);

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

void colorWipe(uint32_t c, uint8_t wait) {
  for(int i=0; i<leftMax; i++) {
    stripRight.setPixelColor(i, c);
    stripLeft.setPixelColor(i,c);
    stripRight.show();
    stripLeft.show();
    delay(wait);
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
/*
  void setPixle(group) {
  for (int outterGroupCount = 0; outterGroupCount < = numberOfGroups; outterGroupCount++)
  {
    int currentGroup[] = group[outterGroupCount];
    for(int innerGroupCount = currentGroup[1]; innerGroupCount <= currentGroup[2]; innerGroupCount++)
    {
        strip.setPixelColor(innerGroupCount, color);
          if(innerGroupCount < maxLeftStrip)
          {
            strip2.setPixelColor(innerGroupCount, color);
          }
    }
  }
  }
*/

