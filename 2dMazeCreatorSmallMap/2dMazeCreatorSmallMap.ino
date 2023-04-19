/***************************************************
  Maze editor with smaller map by Joel Krueger 3-28-2023
  Lilygo T-Display-S3

  The maze is based on an array containing 32 bytes, each row in the maze is one byte,
  so the maze is 32 rows tall.
  Each byte is an unsigned long 32 bit word, each bit corresponds to a block on the map,
  1 is a block, 0 no block.

  Copy and paste the array to edit into the sketch below, be sure to follow the layout closely.
  Run the sketch and edit the maze with the touch screen
  touch screen -- move around the maze
               button 1 -- prints the maze array to the serial monitor
               button 2 -- set current block to 1 or 0 (on/off)
  When your done editing, open the serial monitor,
  press button 1 and the array will ouptut to the serial monitor.
  Copy the code from the serial monitor straight into other sketches,
    like 2dMaze.ino and 3dMaze.ino (in this folder).
  TIP:  The maze will not be saved if you lose power,
        copy the code into this sketch and reload your board to edit it later.
****************************************************/
#define TOUCH_MODULES_CST_SELF
#include "Arduino.h"
#include "TFT_eSPI.h"
#include "TouchLib.h"
#include "Wire.h"

#define PIN_BUTTON_1                  0   //default pins on the Lilygo T-Display- S3
#define PIN_BUTTON_2                 14
#define PIN_BAT_VOLT                  4   //taken from pin_config.h so you don't nedd the extra file
#define PIN_LCD_BL                   38
#define PIN_POWER_ON                 15
#define PIN_IIC_SCL                  17
#define PIN_IIC_SDA                  18
#define PIN_TOUCH_INT                16
#define PIN_TOUCH_RES                21

TouchLib touch(Wire, PIN_IIC_SDA, PIN_IIC_SCL, CTS820_SLAVE_ADDRESS, PIN_TOUCH_RES);

#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h
TFT_eSprite scrnBuff = TFT_eSprite(&tft);


int level = 0;                        //level during game play
const int levelTotal = 1;             //number of levels in the array
const int mazeTall = 32;              //the height of the maze
int mazeRow = 0;                      //references the array to the top of the screen
unsigned long mazeCol = 0;            //references the array to the left of the screen
//int deb = 0;                //touch screen debounce

unsigned long maze[levelTotal][mazeTall] = {
  {
    //paste array beginning here


    0b11111111111111111111111111111111,
    0b10000000000000000000000000000001,
    0b10000000000000000000000000000001,
    0b10000000000000000000000000000001,
    0b10000000000000000000000000000001,
    0b10000000000000000000000000000001,
    0b10000000000000000000000000000001,
    0b10000000000000000000000000000001,
    0b10000000000000000000000000000001,
    0b10000000000000000000000000000001,
    0b10000000000000000000000000000001,
    0b10000000000000000000000000000001,
    0b10000000000000000000000000000001,
    0b10000000000000000000000000000001,
    0b10000000000000000000000000000001,
    0b10000000000000000000000000000001,
    0b10000000000000000000000000000001,
    0b10000000000000000000000000000001,
    0b10000000000000000000000000000001,
    0b10000000000000000000000000000001,
    0b10000000000000000000000000000001,
    0b10000000000000000000000000000001,
    0b10000000000000000000000000000001,
    0b10000000000000000000000000000001,
    0b10000000000000000000000000000001,
    0b10000000000000000000000000000001,
    0b10000000000000000000000000000001,
    0b10000000000000000000000000000001,
    0b10000000000000000000000000000001,
    0b10000000000000000000000000000001,
    0b10000000000000000000000000000001,
    0b11111111111111111111111111111111

/*    0b11111111111111111111111111111111,
    0b10001000000000000000000000000001,
    0b10101011111111111111111111111111,
    0b10101000000000000000000000000001,
    0b10101011111111111101111111111101,
    0b10101010000000000000100001000101,
    0b10101010111111111110101011010101,
    0b10101010100000000000101001010101,
    0b10100010001111111110101101010101,
    0b10111111111000000000101001010001,
    0b10000000000011111111101011011111,
    0b11111110111110000000101000010001,
    0b10000000000010111111101111110101,
    0b10111110111010100000001000000101,
    0b10100010100010101111111011111101,
    0b10101010101110100000000010000001,
    0b10101010101000111111111110111111,
    0b10101010101010000000000000100001,
    0b10101010101010111111111111101101,
    0b10101010101010000000000000001001,
    0b10101010101011111111111111111011,
    0b10001000101000000000001000001001,
    0b11111111101011111111101010101101,
    0b10000000001010001000101010100101,
    0b10111111111010101010101010110001,
    0b10000100011010101010101010011111,
    0b10110001010000101010100001000000,
    0b10011111010111101010111111011111,
    0b10100010010100001010000001000001,
    0b10101011110101111011111101111101,
    0b10001000000100000010000000000001,
    0b11111111111111111111111111111111
    */

    //paste array ends here
  }
};

void drawMaze(int color) {
  for (int a = 0; a < 24; a++) {
    for (unsigned long b = 0; b < 32; b++) {
      if ((a + mazeRow >= 0) && (a + mazeRow < mazeTall) &&
          (b + mazeCol >= 0ul) && (b + mazeCol < 32ul) &&
          (maze[level][a + mazeRow] & (1ul << 31ul - mazeCol - b))) {
        scrnBuff.drawRect(b * 10, a * 10 - 5, 9, 9, color);
      } else {
        scrnBuff.drawRect(b * 10, a * 10 - 5, 9, 9, TFT_BLACK);
      }
    }
  }
  scrnBuff.pushSprite(0, 0);
}

void flashScreen() {
  for (int i = 0; i < 6; i++) {
    drawMaze(TFT_GREEN);
    delay(100);
    drawMaze(TFT_CYAN);
    delay(100);
  }
}

void drawX() {
  int x = 151, y = 86;                           //the position of the X
  unsigned int color = TFT_RED;
  scrnBuff.fillRect(x, y, 8, 8, color);
}

void touchScreen(int x, int y) {
  if (y > 200) {
    mazeCol--;              //move left
    drawMaze(TFT_CYAN);
  }
  if (y < 120) {
    mazeCol++;              //move right
    drawMaze(TFT_CYAN);
  }
  if (x < 85 && y > 120 && y < 200) {
    mazeRow--;              //move up
    drawMaze(TFT_CYAN);
  }
  if (x > 85 && y > 120 && y < 200) {
    mazeRow++;              //move down
    drawMaze(TFT_CYAN);
  }
}

void setup() {
  pinMode(PIN_BUTTON_1, INPUT); //buttons on dev board
  pinMode(PIN_BUTTON_2, INPUT);
  pinMode(PIN_POWER_ON, OUTPUT);
  digitalWrite(PIN_POWER_ON, HIGH);
  pinMode(PIN_TOUCH_RES, OUTPUT);
  digitalWrite(PIN_TOUCH_RES, LOW);
  delay(500);
  digitalWrite(PIN_TOUCH_RES, HIGH);
  tft.init();
  tft.setRotation(3);           //USB plug on the left
  tft.fillScreen(TFT_BLACK);
  scrnBuff.createSprite(320, 170);
  scrnBuff.fillSprite(TFT_BLACK);
  Wire.begin(PIN_IIC_SDA, PIN_IIC_SCL);
  drawX();
  drawMaze(TFT_CYAN);
  Serial.begin(115200);
}

void loop() {
  if (touch.read()) {
    TP_Point t = touch.getPoint(0);
    if (t.x < 170) {
      touchScreen(t.x, t.y);
    }
  }
  if (!digitalRead(PIN_BUTTON_1)) {      //button pressed to send code to the serial monitor
    //print a format that can be manually copied into the array above
    Serial.println();
    Serial.println("Paste the code in the 2dMaze.ino or 3dMaze.ino sketches.");
    Serial.println();
    for (int i = 0; i < mazeTall; i++) {
      Serial.print("    0b" + String(maze[level][i], BIN));
      if (i < mazeTall - 1) {
        Serial.println(",");
      } else {
        Serial.println("");
      }
    }
    Serial.println("");
    flashScreen();
    delay(250);
    while (!digitalRead(PIN_BUTTON_1)) { //wait here until button is released
      delay(10);
    }
  }
  if (!digitalRead(PIN_BUTTON_2)) {      //button pressed, toggle block on/off
    if (!bitRead(maze[level][mazeRow + 9], 16ul - mazeCol)) {
      //set bit
      bitSet(maze[level][mazeRow + 9], 16ul - mazeCol);
    } else {
      //clear bit
      bitClear(maze[level][mazeRow + 9], 16ul - mazeCol);
    }
    drawMaze(TFT_CYAN);
    while (!digitalRead(PIN_BUTTON_2)) { //wait here until button is released
      delay(10);
    }
  }
  delay(100);
}
