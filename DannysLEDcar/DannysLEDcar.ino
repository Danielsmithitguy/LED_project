#include <Adafruit_NeoPixel.h>

#define neoPinLeft 4
#define neoPinRight 17
#define togglePinIn 5

int brightness = 225;
int leftMax = 36;
int rightMax = 8;
//bool toggle = false;
//toggle settings

int previous;
long time = 0;
long debounce = 200;
int currentColor = 0;
int numberOfColors = 3;
int colorArray[5];

Adafruit_NeoPixel stripLeft = Adafruit_NeoPixel(leftMax, neoPinLeft, NEO_RGB);
Adafruit_NeoPixel stripRight = Adafruit_NeoPixel(rightMax, neoPinRight, NEO_RGB);
uint32_t red = stripLeft.Color(225, 0, 0);
uint32_t blue = stripLeft.Color(0, 225, 0);
uint32_t green = stripLeft.Color(0, 0, 225);
uint32_t off = stripLeft.Color(0, 0, 0);
uint32_t purple = stripLeft.Color(0, 120, 225);
uint32_t color;
//const int numOfGroups = 5;
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

void checkToggle() {
  bool reading = digitalRead(togglePinIn);
  Serial.print(String(pressCheck(reading, previous)) + "\n");
  if (pressCheck(reading, previous)){
    //delay(20);
    currentColor = -1;
    Serial.print("test");
  }
  // Check if state changed from high to low (button press).
  if(reading == LOW && previous == HIGH) {
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.
    reading = digitalRead(togglePinIn);
    if (reading == LOW) {
      currentColor++;
      if (currentColor > numberOfColors){
      currentColor = 1;
      }
    }
  }
  

  // Set the last button state to the old state.
  previous = reading;


  /*
    reading = digitalRead(togglePinIn);

    if (previous != reading)
    {
     time = millis();
    }
    previous = reading;
    //cutout && previous == LOW was (reading == HIGH && previous == LOW && (millis() - time) > debounce)
    if (reading == LOW && (millis() - time) > debounce) {

     //set neopixle toggle state
     if (state > numberOfStates) {
       toggle = 0;
     }
     else
     {
       toggle++;
     }
    }
  */
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
  }
}

bool pressCheck(int x, int y) {
  if (x == y and x != 0 and time == 0) {
    time = millis();
    //Serial.print(time);
  }
  //Serial.print(time);
  Serial.print("time : " + String(time) + " | " + "millis :" + String(millis()) + "\n");
  if (x != y) {    
    if (time != 0 and (time + 200) < millis()) {
      //Serial.print("\n before: current color | " + String(currentColor));
      //currentColor = numberOfColors;
      //Serial.print("\n After: current color | " + String(currentColor));
      //delay(20);
      time = 0;
      return true;
    }
    time = 0;
  }
  return false;
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

