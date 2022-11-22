#include <Adafruit_NeoPixel.h>

#define PINc 3	 // input pin Neopixel is attached to pin 10
#define PINr 7
#define NUMPIXELS 8 // number of neopixels in Ring

Adafruit_NeoPixel pixelsR = Adafruit_NeoPixel(6, PINr, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixelsC = Adafruit_NeoPixel(8, PINc, NEO_GRB + NEO_KHZ800);

int delayval = 100;

int redColor = 0;
int blueColor = 0;
int greenColor = 0;

int data;
int winner = 0;
int i = 1;
int run = 0; //controls the number of times the start up sequence runs
int s = 0;
int flash = 0;

int mode = 8;             // Variable to store mode
int prevMode = 8;
bool startupComplete = false;

int aiMove[4]; //store the values for coordinate moves
int moveIndex=0;

void setup() {
  pixelsR.begin();
  pixelsC.begin();

  clearPixels();
  delay(100);

  Serial.begin(9600); //init the serial monitor
  Serial.println("setup complete");
}

//set the global rgb colors
void setColor(int r, int g, int b){
  redColor = r;
  greenColor = g;
  blueColor = b;
  Serial.println("set color complete");
}

//rainbow code
void rainbow(int wait) {
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<9; i++) { 
      int pixelHue = firstPixelHue + (i * 65536L / NUMPIXELS);
      pixelsR.setPixelColor(i, pixelsR.gamma32(pixelsR.ColorHSV(pixelHue)));
      pixelsC.setPixelColor(i, pixelsC.gamma32(pixelsC.ColorHSV(pixelHue)));
      showPixels();
      delay(wait);
    }
    //showPixels();
    delay(wait);
    clearPixels();
  }
}

//clears all of the pixels on both strips
void clearPixels(){
  pixelsC.clear();
  pixelsR.clear();
  showPixels();
  Serial.println("clear pixels complete");
}

//shows the pixels on both strips
void showPixels(){
  pixelsC.show();
  pixelsR.show();
  Serial.println("show pixels complete");
}

//light up a specific index of an LED on a given neopixel strip
// 1 = row
// 0 = col
void setNeopixelIndex(int pixel, int pos, int r, int g, int b){
  if (pixel == 1){//row
    pixelsR.setPixelColor(pos, pixelsR.Color(r, g, b));
    showPixels();
  }
  else if(pixel == 0){
    pixelsC.setPixelColor(pos, pixelsC.Color(r, g, b));
    showPixels();
  }
  Serial.println("set neopixel index complete");
}

//Get the position on the Neopixel you want to light up from the serial monitor
void getPosSerial(int pixel, int r, int g, int b){
  while(i==1){
   	Serial.println("Enter pos");
    while(Serial.available() == 0) {
    }
    if (Serial.available() > 0) {
      data = Serial.read()- '0';
    }
    i=0;
  }
  setNeopixelIndex(pixel,data,r,g,b);
}

//sets the color on both Neopixel strips
void setBothColor(int r, int g, int b){
  for (int i = 0; i<9; i++){
     pixelsC.setPixelColor(i, pixelsC.Color(r, g, b));
     //while(i<7){
      pixelsR.setPixelColor(i, pixelsR.Color(r,g, b));
     //}
     showPixels();
  }
  Serial.println("set both color complete");
}

//light up the LEDs a specific color for a certain pattern delay and number of times
void neoPixelPattern(int flashDelay, int r, int g, int b, int numRuns, int moveDirection){
  while(run < numRuns){
    if(moveDirection == 1){ //default 0-7 direction
      for (int i = 0; i<9; i++){
        pixelsC.setPixelColor(i, pixelsC.Color(r, g, b));
        pixelsR.setPixelColor(i, pixelsR.Color(r, g, b));
        showPixels();
        delay(flashDelay);
        clearPixels();
        delay(flashDelay);
      }
     }
     else{
       for (int i = 8; i>=0; i--){
          pixelsC.setPixelColor(i, pixelsC.Color(r, g, b));
          pixelsR.setPixelColor(i, pixelsR.Color(r, g, b));
          showPixels();
          delay(flashDelay);
          clearPixels();
          delay(flashDelay);
      }
     }
      run = run + 1;
  }
  Serial.println("neopixel pattern complete");
}

/* starts up the game by flashing both neopixels purple 2x then turning solid green
 * mode received = 9
 */
void startupSequence(){
  neoPixelPattern(50, 100,0, 255, 2,1); //flash purple 2x for 1/2 sec
  setBothColor(0, 255, 0); //show solid green
  startupComplete = true;
  Serial.println("startup sequence complete");
}

void aiTurnSequence(){ //set all neopixels to red
   setBothColor(255, 0, 0);
   delay(1000);
   Serial.println("ai turn sequence complete");
}

void playerTurnSequence(){
  setBothColor(0, 255, 0); //turn all neopixels green
  delay(1000);
  Serial.println("player turn sequence complete");
}

void playerWinSequence(){
  rainbow(100);
  clearPixels();
  Serial.println("player win sequence complete");
}

void aiWinSequence(){
  //neoPixelPattern(250, 255, 255, 255, 10, 1); //flash white 10x for 1/4 sec
  neoPixelPattern(250, 255, 255, 255, 10, 0);
  Serial.println("ai win sequence complete");
}


void invalidMoveSequence(){
  Serial.println("Invalid Move Sequence");
  while (s < 3){  
      setBothColor(255, 100, 0);
      delay(100);
  
      setBothColor(0, 0, 0);
      delay(100); 
      s = s+1;
  }
  clearPixels();
  setBothColor(0,255,0);
}

// test out serial comm between python file
void gameMode(){

  while(Serial.available())//check if data is available
  {
    mode = Serial.parseInt(); //while data is available read the data
    
    if(moveIndex == 4){
          Serial.println("reset moveIndex to 0");
          moveIndex = 0;
    } 
  }
    if(mode != prevMode){

      Serial.print("COORD VALUE RECEIVED = ");
      Serial.println(mode);    
    
      if(mode >=0 && mode < 8){ //we are receiving the index for coordinate moves

        Serial.println("DATA FOR COORD MOVES");

        aiMove[moveIndex]=mode;
  
//          if(moveIndex == 0){ //start row - yellow
//            Serial.println("set aiMove[0] as start row - yellow");
//            aiMove[0] = mode;
//            //setNeopixelIndex(1, aiMoves[moveIndex], 255 , 255, 0);
//          }
//          else{ //end row - blue
//            aiMove[2] = mode;
//          }
//    
//          if(moveIndex == 1){ //start col - yellow
//            aiMove[1] = mode;
//            //Serial.println("start col is yellow");
//            //setNeopixelIndex(0, aiMoves[moveIndex], 255 , 255, 0);
//          }
//          else{//end col - blue
//            aiMove[3] = mode;
//            //Serial.println("end col is blue");
//            //setNeopixelIndex(0, aiMoves[moveIndex], 0 , 0, 255);
//          }
//        }

        if(moveIndex == 3){
          Serial.println("ALL 4 COORD RECEIVED!");
          Serial.println("Set the coordinate colors");
          //clearPixels();
          setBothColor(255,0,0);
          
          setNeopixelIndex(1, aiMove[0], 255 , 255, 0); //start row yellow
          setNeopixelIndex(0, aiMove[1], 255 ,255, 0); //start col yellow
          
          setNeopixelIndex(1, aiMove[2], 0 , 0, 255); //end row blue
          setNeopixelIndex(0, aiMove[3], 0 , 0, 255); //end col blue
  
          Serial.print("Start Coordinate: ");
          Serial.print("(");
          Serial.print(aiMove[0]);
          Serial.print(",");
          Serial.print(aiMove[1]);
          Serial.println(")");

          Serial.print("End Coordinate: ");
          Serial.print("(");
          Serial.print(aiMove[2]);
          Serial.print(",");
          Serial.print(aiMove[3]);
          Serial.println(")");
          
          delay(250);
          
        }
        
        moveIndex = moveIndex + 1;

      }
      else{ //we are receiving a game mode not a coordinate
        clearPixels();
        Serial.println("SWITCH CASE MENU");
        switch (mode) { //which mode of the game are we in?
          case 9:
            // mode 9 is startup
            Serial.println("startup mode");
            startupSequence();
            break;
          case 10: //player's turn
            Serial.println("player's turn sequence");
            playerTurnSequence();
            mode = 8;
            break;
          case 11: //ai's turn
            Serial.println("ai's turn sequence");
            aiTurnSequence();
            break;
          case 12: //player wins
            Serial.println("Player wins mode");
            playerWinSequence();
            break;
          case 13: //ai wins
            Serial.println("AI wins mode");
            aiWinSequence();
            break;
          case 14: //invalid move
            Serial.println("Invalid Move!");
            s = 0;
            invalidMoveSequence();
            break;
          default:
            clearPixels();
            delay(100);
            break;
        }
        delay(100);
      }
    }
  prevMode = mode;
  //mode = 8;
}

void loop(){
  gameMode();
}
