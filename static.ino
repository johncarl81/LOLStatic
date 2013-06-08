#include <Charliplexing.h>   

#define DELAY 0
#define SCREENX 14
#define SCREENY 9
#define SCALEX 4
#define SCALEY 4

#define SIZEX 56 // SCREENX * SCALEX
#define SIZEY 36 // SCREENY * SCALEY

//using bitwise operations so SCALEX (4) is incorpoated into byte
byte worldArray[SCREENX * SIZEY];
long density = 50;
double shadesScale = (SCALEX * SCALEY) * 1.0 / SHADES;

 
void setup() {
  LedSign::Init(GRAYSCALE);
  randomSeed(analogRead(5));
  //Builds the world with an initial seed.
  Serial.begin(9600);
  seedWorld();
}

void loop() {
  
  int x = random(SIZEX);
  int y = random(SIZEY);
  
  setWorld(x, y, random(2));
  
  update(x,y);
  delay(DELAY);
}

// Update the given pixel location.
void update(int x, int y){
  
  
  int scaledx = x / SCALEX;
  int scaledy = y / SCALEY;
  
  // Sum the local pixels around the scaled location.
  int total = 0;
  for(int i = 0; i < SCALEX; i++){
    for(int j = 0; j < SCALEY; j++){
      total += world((scaledx * SCALEX) + i, (scaledy * SCALEY) + j);
    }
  }
 
 // Set the pixel location to the shade value.
 LedSign::Set((scaledx + 7) % SCREENX, (scaledy + 4) % SCREENY, total / shadesScale); 
}

//Re-seeds based off of RESEEDRATE
void seedWorld(){
  for (int i = 0; i < SIZEX; i++) {
    for (int j = 0; j < SIZEY; j++) {
      if (random(100) < density) {
        setWorld(i, j, 1);
      }
      else {
        setWorld(i, j, 0);
      }
    }
  }
  
  // Update the entire scren.
  for (int i = 0; i < SCREENX; i++) {
    for (int j = 0; j < SCREENY; j++) {
      update(i * SCALEX,j * SCALEY);
    }
  }
}

// Set a bit at the given location.
byte setWorld(int x, int y, byte input){
  
  int xRef = x / 4;
  int xBit = x - (4 * (x / 4));
  
  bitWrite(worldArray[(xRef * SIZEY) + y], xBit, input);
}

// Get a bit from the given location.
byte world(int x, int y){
  
  int xRef = x / 4;
  int xBit = x - (4 * (x / 4));
  
  return bitRead(worldArray[(xRef * SIZEY) + y], xBit);
}
