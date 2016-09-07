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
int currentColor = 0;
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



void setup() {

  Serial.begin(9600);

  pinMode(togglePinIn, INPUT_PULLUP);
  //setting Group ranges
  //globalGroup[1] = [startOfGroupOne, endOfGroupOne]

  stripLeft.begin();
  stripLeft.show();

  stripRight.begin();
  stripRight.show();
}

void loop() {

  checkToggle();
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
}

void checkToggle(){
  bool reading = digitalRead(togglePinIn);
  //Serial.print("pressCheck | " + String(pressCheck(reading, previous)) + "\n");
  int currentState = pressCheck(reading, previous);
  //Serial.print("currentState | " + String(currentState) + "\n");
  Serial.print("currentColor | " + String(currentColor) + "\n");
  if (currentState != 4) {
    switch(currentState) {
      case 0: 
        currentColor = 0;
      break;
      case 1:
         currentColor++;
      break;
    }
  }
  if (currentColor > numberOfColors){
    currentColor = 1;
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

uint32_t effectsPick(int currentEffect) {
  int previousEffect;
  if (currentEffect != previousEffect) {
    switch (currentEffect) {
      case 1:
    
      break;
    } 
  }
  previousEffect == currentEffect;
}


int pressCheck(int reading, int previous){
  //long time;
  int buttonState;
  uint32_t test = buttonHold(time,2000);
  //Serial.print("time | " + String(time) + " : mills | " + String(millis()) + "\n");
  //Serial.print("Current Reading | " + String(reading) + " : Previous Reading | " + String(previous) + "\n");
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
  if (previous == 1 && reading == 0) {
    //Serial.print("im returning \n");
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

bool buttonHold(int time, int pause){
  if (time != 0 && (time + pause) > millis()){
    return true;
  } else {
    return false;    
  }
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

