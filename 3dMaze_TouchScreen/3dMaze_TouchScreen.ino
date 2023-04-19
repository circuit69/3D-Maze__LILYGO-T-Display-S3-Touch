/***************************************************
  3D maze by Joel Krueger 3-28-2023
  Lilygo T-DISPLAY-S3

  Find your way out of the maze using the touch screen,
  get to the opening on one of the edges of the maze.

      center of touch screen -- move forward
   left side of touch screen -- turn left
  right side of touch screen -- turn right
               button 1 or 2 -- show map
****************************************************/
#define TOUCH_MODULES_CST_SELF
#include "Arduino.h"
#include "TFT_eSPI.h"
#include "TouchLib.h"
#include "Wire.h"

#define PIN_BUTTON_1                  0   //default pins on the Lilygo T-Display- S3
#define PIN_BUTTON_2                 14
#define PIN_BAT_VOLT                  4   //taken from pin_config.h so you don't need the extra file
#define PIN_LCD_BL                   38
#define PIN_POWER_ON                 15
#define PIN_IIC_SCL                  17
#define PIN_IIC_SDA                  18
#define PIN_TOUCH_INT                16
#define PIN_TOUCH_RES                21

TouchLib touch(Wire, PIN_IIC_SDA, PIN_IIC_SCL, CTS820_SLAVE_ADDRESS, PIN_TOUCH_RES);

#include <TFT_eSPI.h>  // Graphics and font library for ST7735 driver chip
TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h
TFT_eSprite scrn = TFT_eSprite(&tft);

int level = 0;              //level during game play
const int levelTotal = 3;   //number of levels in the maze[levelTotal][mazeTall] array below
const int mazeTall = 32;    //the height of each level in the maze
int mazeRow = 8;            //player's position, N&S
unsigned long mazeCol = 7;  //player's positions in the row, E&W
int facing = 0;             //0=north 1=east 2=south 3=west
unsigned int lineColor = tft.color565(0, 255, 255);
unsigned int bgColor = tft.color565(0, 48, 48);
int deb = 0;                //touch screen debounce

unsigned long maze[levelTotal][mazeTall] = {
  //you can use 2dMazeCreator.ino to edit these arrays (one level at a time)
  { //level 1
    //paste or copy array beginning here

    0b11111111111111111111111111111111,
    0b10001000000000000000000000000001,
    0b10101010101111111111111111111111,
    0b10101000000000000000000000000001,
    0b10101010101111111101111111111101,
    0b10101000000000000000100001000101,
    0b10101010101111111110101011010101,
    0b10101010100000000000101001010101,
    0b10100000001111111110101101010101,
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

    //paste or copy array ends here
    //you can edit any of the levels below as well
  },
  { //level 2
    0b11111111111111111111111111111111,
    0b10010000000000001000000000000001,
    0b10111101111110111111101111111011,
    0b10100000000010000001000000010001,
    0b10111111011111010101010101010101,
    0b10000000000000010000000100000101,
    0b10111111111111111111111111111001,
    0b10001000100000000000100010001011,
    0b10101010101111111110101010101011,
    0b10100010001000000010001000100001,
    0b10111111111011111011111111111101,
    0b10000000100010001000000000000001,
    0b10111110101010101111111111111111,
    0b10000010101010100000000100010001,
    0b11111010101010111111110101010101,
    0b10000010101010100000000001000101,
    0b10111110101000101111111111111101,
    0b10000010101111111000100010000101,
    0b11111010001000000010001000100001,
    0b10000011111111111111111111111101,
    0b10111110000100000000100010000001,
    0b10000000110101111110101010111111,
    0b10111111100101000000101010000001,
    0b10000010000101011111101011111101,
    0b11111010111101000000001000000001,
    0b10001010100011111111101111111111,
    0b10101010101000000000101000010000,
    0b10101010101111111111101011010101,
    0b10100010001000000000001001000101,
    0b10111110101011111111111111111101,
    0b10000000100000000000000000000001,
    0b11111111111111111111111111111111
  },
  { //level 3
    0b11111111111111111111111111111111,
    0b10010000000000001000000000000001,
    0b10111101111110111111101111111011,
    0b10100000000010000001000000010001,
    0b10111111011111010101010101011101,
    0b10000000000000010000000100000101,
    0b10111111111111111111111111111001,
    0b10001000100000000000100010001011,
    0b10101010101111111110101010101011,
    0b10100010001000000010001000100001,
    0b10111111111011111011111111111101,
    0b10000000100010001000000000000001,
    0b10111110101010101111111111111111,
    0b10000010101010100000000100010001,
    0b11111010101010111111110101010101,
    0b10000010101010100000000001000101,
    0b10111110101000101111111111111101,
    0b10000010101111111000100010000101,
    0b11111010001000000010001000100001,
    0b10000011111111111111111111111101,
    0b10111110000100000000100010000001,
    0b10000000110101111110101010111111,
    0b10111111100101000000101010000001,
    0b10000010000101011111101011111101,
    0b11111010111101000000001000000001,
    0b10001010100011111111101111111111,
    0b10101010101000000000101000010001,
    0b10101010101111111111101011010101,
    0b10100010101000000000001001000101,
    0b10111110101011111111111111111100,
    0b10000000100000000000000000000001,
    0b11111111111111111111111111111111
  }
};

void drawStats() {
  scrn.setCursor(120, 18);
  scrn.setTextSize(2);
  switch (facing) {
    case 0:
      scrn.print("N");
      break;
    case 1:
      scrn.print("E");
      break;
    case 2:
      scrn.print("S");
      break;
    case 3:
      scrn.print("W");
      break;
  }
  scrn.print("  " + String(mazeRow) + "  " + String(mazeCol) + "  ");
}

void drawMap() {
  int color = lineColor;
  scrn.fillRect(0, 0, 320, 170, TFT_BLACK);
  for (int i = 0; i < 17; i++) {
    for (int j = 0; j < 32; j++) {
      if (mazeRow - 8 + i > -1 && mazeRow - 8 + i < 32) {
        if ((maze[level][mazeRow - 8 + i] & 1ul << 31 - j)) {
          scrn.drawRect(j * 10, i * 10, 9, 9, color);
        }
      }
    }
  }
  switch (facing) {       //draw triangle position on the map
    case 0:
      scrn.fillTriangle(mazeCol * 10 + 4, 81, mazeCol * 10 + 2, 88, mazeCol * 10 + 6, 88, TFT_RED);
      break;
    case 1:
      scrn.fillTriangle(mazeCol * 10 + 8, 84, mazeCol * 10 + 1, 82, mazeCol * 10 + 1, 86, TFT_RED);
      break;
    case 2:
      scrn.fillTriangle(mazeCol * 10 + 4, 88, mazeCol * 10 + 2, 81, mazeCol * 10 + 6, 81, TFT_RED);
      break;
    case 3:
      scrn.fillTriangle(mazeCol * 10 + 1, 84, mazeCol * 10 + 8, 82, mazeCol * 10 + 8, 86, TFT_RED);
      break;
  }
  scrn.pushSprite(0, 0);
}

void drawLines() {
  int color = TFT_DARKGREY;
  scrn.fillRect(0, 0, 320, 170, TFT_BLACK);
  if (false) {
    scrn.drawLine(0, 0, 160, 95, color);
    scrn.drawLine(320, 0, 160, 95, color);
    scrn.drawLine(0, 170, 160, 116, color);
    scrn.drawLine(320, 170, 160, 116, color);

    scrn.drawRect(150, 90, 20, 30, color);   //center box
    scrn.drawRect(120, 73, 80, 58, color);   //+2
    scrn.drawRect(80, 48, 162, 97, color);   //+1
    scrn.drawRect(20, 12, 280, 152, color);  //current block
  }
  color = lineColor;

  switch (facing) {
    case 0:  //facing north
      //depth 0 #########################
      scrn.drawLine(20, 12, 20, 164, color);             //left corner
      if (maze[level][mazeRow] & 1ul << 32 - mazeCol) {  //wall to the left
        scrn.drawLine(0, 0, 20, 12, color);              //top diagonal
        scrn.drawLine(0, 170, 20, 164, color);           //bottom diagonal
      } else if (maze[level][mazeRow - 1] & 1ul << 32 - mazeCol) {
        //no wall on the left, check ahead 1 on the left
        scrn.drawLine(0, 12, 20, 12, color);    //top straight
        scrn.drawLine(0, 164, 20, 164, color);  //bottom straight
      }
      scrn.drawLine(300, 12, 300, 164, color);           //right corner
      if (maze[level][mazeRow] & 1ul << 30 - mazeCol) {  //wall to the right
        scrn.drawLine(320, 0, 300, 12, color);           //top diagonal
        scrn.drawLine(320, 170, 300, 164, color);        //botton diagonal
      } else if (maze[level][mazeRow - 1] & 1ul << 30 - mazeCol) {
        //no wall on the right, check ahead 1 on the right
        scrn.drawLine(300, 12, 320, 12, color);    //top straight
        scrn.drawLine(300, 164, 320, 164, color);  //bottom straight
      }
      if (maze[level][mazeRow - 1] & 1ul << 31 - mazeCol) {  //wall straight ahead
        scrn.drawLine(20, 12, 300, 12, color);               //top line
        scrn.drawLine(20, 164, 300, 164, color);             //bottom line
      } else {
        //depth 1 ######################### facing north
        scrn.drawLine(80, 48, 80, 145, color);                 //left corner
        if (maze[level][mazeRow - 1] & 1ul << 32 - mazeCol) {  //wall to the left
          scrn.drawLine(20, 12, 80, 48, color);                //top diagonal
          scrn.drawLine(20, 164, 80, 145, color);              //bottom diagonal
        } else if (maze[level][mazeRow - 2] & 1ul << 32 - mazeCol) {
          //no wall on the left, check ahead 1 on the left
          scrn.drawLine(20, 48, 80, 48, color);    //top straight
          scrn.drawLine(20, 145, 80, 145, color);  //bottom straight
        }
        scrn.drawLine(242, 48, 242, 145, color);               //right corner
        if (maze[level][mazeRow - 1] & 1ul << 30 - mazeCol) {  //wall to the right
          scrn.drawLine(300, 12, 242, 48, color);              //top diagonal
          scrn.drawLine(300, 164, 242, 145, color);            //botton diagonal
        } else if (maze[level][mazeRow - 2] & 1ul << 30 - mazeCol) {
          //no wall on the right, check ahead 1 on the right
          scrn.drawLine(300, 48, 242, 48, color);    //top straight
          scrn.drawLine(300, 145, 242, 145, color);  //bottom straight
        }
        if (maze[level][mazeRow - 2] & 1ul << 31 - mazeCol) {  //wall straight ahead
          scrn.drawLine(80, 48, 242, 48, color);               //top line
          scrn.drawLine(80, 145, 242, 145, color);             //bottom line
        } else {
          //depth 2 ######################### facing north
          scrn.drawLine(120, 73, 120, 131, color);               //left corner
          if (maze[level][mazeRow - 2] & 1ul << 32 - mazeCol) {  //wall to the left
            scrn.drawLine(80, 48, 120, 73, color);               //top diagonal
            scrn.drawLine(80, 145, 120, 131, color);             //bottom diagonal
          } else if (maze[level][mazeRow - 3] & 1ul << 32 - mazeCol) {
            //no wall on the left, check ahead 1 on the left
            scrn.drawLine(80, 73, 120, 73, color);    //top straight
            scrn.drawLine(80, 131, 120, 131, color);  //bottom straight
          }
          scrn.drawLine(200, 73, 200, 131, color);               //right corner
          if (maze[level][mazeRow - 2] & 1ul << 30 - mazeCol) {  //wall to the right
            scrn.drawLine(242, 48, 200, 73, color);              //top diagonal
            scrn.drawLine(242, 145, 200, 131, color);            //botton diagonal
          } else if (maze[level][mazeRow - 3] & 1ul << 30 - mazeCol) {
            //no wall on the right, check ahead 1 on the right
            scrn.drawLine(242, 73, 200, 73, color);    //top straight
            scrn.drawLine(242, 131, 200, 131, color);  //bottom straight
          }
          if (maze[level][mazeRow - 3] & 1ul << 31 - mazeCol) {  //wall straight ahead
            scrn.drawLine(120, 73, 200, 73, color);              //top line
            scrn.drawLine(120, 131, 200, 131, color);            //bottom line
          } else {
            //depth 3 ######################### facing north
            scrn.drawLine(150, 90, 150, 120, color);               //left corner
            if (maze[level][mazeRow - 3] & 1ul << 32 - mazeCol) {  //wall to the left
              scrn.drawLine(120, 73, 150, 90, color);              //top diagonal
              scrn.drawLine(120, 131, 150, 120, color);            //bottom diagonal
            } else if (maze[level][mazeRow - 4] & 1ul << 32 - mazeCol) {
              //no wall on the left, check ahead 1 on the left
              scrn.drawLine(120, 90, 150, 90, color);    //top straight
              scrn.drawLine(120, 120, 150, 120, color);  //bottom straight
            }
            scrn.drawLine(170, 90, 170, 120, color);               //right corner
            if (maze[level][mazeRow - 3] & 1ul << 30 - mazeCol) {  //wall to the right
              scrn.drawLine(200, 73, 170, 90, color);              //top diagonal
              scrn.drawLine(200, 131, 170, 120, color);            //botton diagonal
            } else if (maze[level][mazeRow - 4] & 1ul << 30 - mazeCol) {
              //no wall on the right, check ahead 1 on the right
              scrn.drawLine(200, 90, 170, 90, color);    //top straight
              scrn.drawLine(200, 120, 170, 120, color);  //bottom straight
            }
            if (maze[level][mazeRow - 4] & 1ul << 31 - mazeCol) {  //wall straight ahead
              scrn.drawLine(150, 90, 170, 90, color);              //top line
              scrn.drawLine(150, 120, 170, 120, color);            //bottom line
            } else {
              //depth 4 ######################### facing north
              scrn.drawLine(158, 95, 158, 117, color);               //center distant line
              if (maze[level][mazeRow - 4] & 1ul << 32 - mazeCol) {  //wall to the left
                scrn.drawLine(150, 90, 158, 95, color);              //top diagonal
                scrn.drawLine(150, 120, 158, 117, color);            //bottom diagonal
              } else if (maze[level][mazeRow - 5] & 1ul << 32 - mazeCol) {
                //no wall on the left, check ahead 1 on the left
                scrn.drawLine(150, 95, 158, 95, color);    //top straight
                scrn.drawLine(150, 117, 158, 117, color);  //bottom straight
              }
              scrn.drawLine(162, 95, 162, 117, color);               //right corner
              if (maze[level][mazeRow - 4] & 1ul << 30 - mazeCol) {  //wall to the right
                scrn.drawLine(170, 90, 162, 95, color);              //top diagonal
                scrn.drawLine(170, 120, 162, 117, color);            //botton diagonal
              } else if (maze[level][mazeRow - 5] & 1ul << 30 - mazeCol) {
                //no wall on the right, check ahead 1 on the right
                scrn.drawLine(170, 95, 162, 95, color);    //top straight
                scrn.drawLine(170, 117, 162, 117, color);  //bottom straight
              }
            }
          }
        }
      }
      break;

    case 1:  //facing east
      //depth 0-------------------------
      scrn.drawLine(20, 12, 20, 164, color);                 //left corner
      if (maze[level][mazeRow - 1] & 1ul << 31 - mazeCol) {  //wall to the left
        scrn.drawLine(0, 0, 20, 12, color);                  //top diagonal
        scrn.drawLine(0, 170, 20, 164, color);               //bottom diagonal
      } else if (maze[level][mazeRow - 1] & 1ul << 30 - mazeCol) {
        //no wall on the left, check ahead 1 on the left
        scrn.drawLine(0, 12, 20, 12, color);    //top straight
        scrn.drawLine(0, 164, 20, 164, color);  //bottom straight
      }
      scrn.drawLine(300, 12, 300, 164, color);               //right corner
      if (maze[level][mazeRow + 1] & 1ul << 31 - mazeCol) {  //wall to the right
        scrn.drawLine(320, 0, 300, 12, color);               //top diagonal
        scrn.drawLine(320, 170, 300, 164, color);            //botton diagonal
      } else if (maze[level][mazeRow + 1] & 1ul << 30 - mazeCol) {
        //no wall on the right, check ahead 1 on the right
        scrn.drawLine(300, 12, 320, 12, color);    //top straight
        scrn.drawLine(300, 164, 320, 164, color);  //bottom straight
      }
      if (maze[level][mazeRow] & 1ul << 30 - mazeCol) {  //wall straight ahead
        scrn.drawLine(20, 12, 300, 12, color);           //top line
        scrn.drawLine(20, 164, 300, 164, color);         //bottom line
      } else {
        //depth 1 ######################### facing east
        scrn.drawLine(80, 48, 80, 145, color);                 //left corner
        if (maze[level][mazeRow - 1] & 1ul << 30 - mazeCol) {  //wall to the left
          scrn.drawLine(20, 12, 80, 48, color);                //top diagonal
          scrn.drawLine(20, 164, 80, 145, color);              //bottom diagonal
        } else if (maze[level][mazeRow - 1] & 1ul << 29 - mazeCol) {
          //no wall on the left, check ahead 1 on the left
          scrn.drawLine(20, 48, 80, 48, color);    //top straight
          scrn.drawLine(20, 145, 80, 145, color);  //bottom straight
        }
        scrn.drawLine(242, 48, 242, 145, color);               //right corner
        if (maze[level][mazeRow + 1] & 1ul << 30 - mazeCol) {  //wall to the right
          scrn.drawLine(300, 12, 242, 48, color);              //top diagonal
          scrn.drawLine(300, 164, 242, 145, color);            //botton diagonal
        } else if (maze[level][mazeRow + 1] & 1ul << 29 - mazeCol) {
          //no wall on the right, check ahead 1 on the right
          scrn.drawLine(300, 48, 242, 48, color);    //top straight
          scrn.drawLine(300, 145, 242, 145, color);  //bottom straight
        }
        if (maze[level][mazeRow] & 1ul << 29 - mazeCol) {  //wall straight ahead
          scrn.drawLine(80, 48, 242, 48, color);           //top line
          scrn.drawLine(80, 145, 242, 145, color);         //bottom line
        } else {
          //depth 2 ######################### facing east
          scrn.drawLine(120, 73, 120, 131, color);               //left corner
          if (maze[level][mazeRow - 1] & 1ul << 29 - mazeCol) {  //wall to the left
            scrn.drawLine(80, 48, 120, 73, color);               //top diagonal
            scrn.drawLine(80, 145, 120, 131, color);             //bottom diagonal
          } else if (maze[level][mazeRow - 1] & 1ul << 28 - mazeCol) {
            //no wall on the left, check ahead 1 on the left
            scrn.drawLine(80, 73, 120, 73, color);    //top straight
            scrn.drawLine(80, 131, 120, 131, color);  //bottom straight
          }
          scrn.drawLine(200, 73, 200, 131, color);               //right corner
          if (maze[level][mazeRow + 1] & 1ul << 29 - mazeCol) {  //wall to the right
            scrn.drawLine(242, 48, 200, 73, color);              //top diagonal
            scrn.drawLine(242, 145, 200, 131, color);            //botton diagonal
          } else if (maze[level][mazeRow + 1] & 1ul << 28 - mazeCol) {
            //no wall on the right, check ahead 1 on the right
            scrn.drawLine(242, 73, 200, 73, color);    //top straight
            scrn.drawLine(242, 131, 200, 131, color);  //bottom straight
          }
          if (maze[level][mazeRow] & 1ul << 28 - mazeCol) {  //wall straight ahead
            scrn.drawLine(120, 73, 200, 73, color);          //top line
            scrn.drawLine(120, 131, 200, 131, color);        //bottom line
          } else {
            //depth 3 ######################### facing east
            scrn.drawLine(150, 90, 150, 120, color);               //left corner
            if (maze[level][mazeRow - 1] & 1ul << 28 - mazeCol) {  //wall to the left
              scrn.drawLine(120, 73, 150, 90, color);              //top diagonal
              scrn.drawLine(120, 131, 150, 120, color);            //bottom diagonal
            } else if (maze[level][mazeRow - 1] & 1ul << 27 - mazeCol) {
              //no wall on the left, check ahead 1 on the left
              scrn.drawLine(120, 90, 150, 90, color);    //top straight
              scrn.drawLine(120, 120, 150, 120, color);  //bottom straight
            }
            scrn.drawLine(170, 90, 170, 120, color);               //right corner
            if (maze[level][mazeRow + 1] & 1ul << 28 - mazeCol) {  //wall to the right
              scrn.drawLine(200, 73, 170, 90, color);              //top diagonal
              scrn.drawLine(200, 131, 170, 120, color);            //botton diagonal
            } else if (maze[level][mazeRow + 1] & 1ul << 27 - mazeCol) {
              //no wall on the right, check ahead 1 on the right
              scrn.drawLine(200, 90, 170, 90, color);    //top straight
              scrn.drawLine(200, 120, 170, 120, color);  //bottom straight
            }
            if (maze[level][mazeRow] & 1ul << 27 - mazeCol) {  //wall straight ahead
              scrn.drawLine(150, 90, 170, 90, color);          //top line
              scrn.drawLine(150, 120, 170, 120, color);        //bottom line
            } else {
              //depth 4 ######################### facing east
              scrn.drawLine(158, 95, 158, 117, color);               //center distant line
              if (maze[level][mazeRow - 1] & 1ul << 27 - mazeCol) {  //wall to the left
                scrn.drawLine(150, 90, 158, 95, color);              //top diagonal
                scrn.drawLine(150, 120, 158, 117, color);            //bottom diagonal
              } else if (maze[level][mazeRow - 1] & 1ul << 26 - mazeCol) {
                //no wall on the left, check ahead 1 on the left
                scrn.drawLine(150, 95, 158, 95, color);    //top straight
                scrn.drawLine(150, 117, 158, 117, color);  //bottom straight
              }
              scrn.drawLine(162, 95, 162, 117, color);               //right corner
              if (maze[level][mazeRow + 1] & 1ul << 27 - mazeCol) {  //wall to the right
                scrn.drawLine(170, 90, 162, 95, color);              //top diagonal
                scrn.drawLine(170, 120, 162, 117, color);            //botton diagonal
              } else if (maze[level][mazeRow + 1] & 1ul << 26 - mazeCol) {
                //no wall on the right, check ahead 1 on the right
                scrn.drawLine(170, 95, 162, 95, color);    //top straight
                scrn.drawLine(170, 117, 162, 117, color);  //bottom straight
              }
            }
          }
        }
      }
      break;

    case 2:  //facing south
      //depth 0-------------------------
      scrn.drawLine(20, 12, 20, 164, color);             //left corner
      if (maze[level][mazeRow] & 1ul << 30 - mazeCol) {  //wall to the left
        scrn.drawLine(0, 0, 20, 12, color);              //top diagonal
        scrn.drawLine(0, 170, 20, 164, color);           //bottom diagonal
      } else if (maze[level][mazeRow + 1] & 1ul << 30 - mazeCol) {
        //no wall on the left, check ahead 1 on the left
        scrn.drawLine(0, 12, 20, 12, color);    //top straight
        scrn.drawLine(0, 164, 20, 164, color);  //bottom straight
      }
      scrn.drawLine(300, 12, 300, 164, color);           //right corner
      if (maze[level][mazeRow] & 1ul << 32 - mazeCol) {  //wall to the right
        scrn.drawLine(320, 0, 300, 12, color);           //top diagonal
        scrn.drawLine(320, 170, 300, 164, color);        //botton diagonal
      } else if (maze[level][mazeRow + 1] & 1ul << 32 - mazeCol) {
        //no wall on the right, check ahead 1 on the right
        scrn.drawLine(300, 12, 320, 12, color);    //top straight
        scrn.drawLine(300, 164, 320, 164, color);  //bottom straight
      }
      if (maze[level][mazeRow + 1] & 1ul << 31 - mazeCol) {  //wall straight ahead
        scrn.drawLine(20, 12, 300, 12, color);               //top line
        scrn.drawLine(20, 164, 300, 164, color);             //bottom line
      } else {
        //depth 1 ######################### facing south
        scrn.drawLine(80, 48, 80, 145, color);                 //left corner
        if (maze[level][mazeRow + 1] & 1ul << 30 - mazeCol) {  //wall to the left
          scrn.drawLine(20, 12, 80, 48, color);                //top diagonal
          scrn.drawLine(20, 164, 80, 145, color);              //bottom diagonal
        } else if (maze[level][mazeRow + 2] & 1ul << 30 - mazeCol) {
          //no wall on the left, check ahead 1 on the left
          scrn.drawLine(20, 48, 80, 48, color);    //top straight
          scrn.drawLine(20, 145, 80, 145, color);  //bottom straight
        }
        scrn.drawLine(242, 48, 242, 145, color);               //right corner
        if (maze[level][mazeRow + 1] & 1ul << 32 - mazeCol) {  //wall to the right
          scrn.drawLine(300, 12, 242, 48, color);              //top diagonal
          scrn.drawLine(300, 164, 242, 145, color);            //botton diagonal
        } else if (maze[level][mazeRow + 2] & 1ul << 32 - mazeCol) {
          //no wall on the right, check ahead 1 on the right
          scrn.drawLine(300, 48, 242, 48, color);    //top straight
          scrn.drawLine(300, 145, 242, 145, color);  //bottom straight
        }
        if (maze[level][mazeRow + 2] & 1ul << 31 - mazeCol) {  //wall straight ahead
          scrn.drawLine(80, 48, 242, 48, color);               //top line
          scrn.drawLine(80, 145, 242, 145, color);             //bottom line
        } else {
          //depth 2 ######################### facing south
          scrn.drawLine(120, 73, 120, 131, color);               //left corner
          if (maze[level][mazeRow + 2] & 1ul << 30 - mazeCol) {  //wall to the left
            scrn.drawLine(80, 48, 120, 73, color);               //top diagonal
            scrn.drawLine(80, 145, 120, 131, color);             //bottom diagonal
          } else if (maze[level][mazeRow + 3] & 1ul << 30 - mazeCol) {
            //no wall on the left, check ahead 1 on the left
            scrn.drawLine(80, 73, 120, 73, color);    //top straight
            scrn.drawLine(80, 131, 120, 131, color);  //bottom straight
          }
          scrn.drawLine(200, 73, 200, 131, color);               //right corner
          if (maze[level][mazeRow + 2] & 1ul << 32 - mazeCol) {  //wall to the right
            scrn.drawLine(242, 48, 200, 73, color);              //top diagonal
            scrn.drawLine(242, 145, 200, 131, color);            //botton diagonal
          } else if (maze[level][mazeRow + 3] & 1ul << 32 - mazeCol) {
            //no wall on the right, check ahead 1 on the right
            scrn.drawLine(242, 73, 200, 73, color);    //top straight
            scrn.drawLine(242, 131, 200, 131, color);  //bottom straight
          }
          if (maze[level][mazeRow + 3] & 1ul << 31 - mazeCol) {  //wall straight ahead
            scrn.drawLine(120, 73, 200, 73, color);              //top line
            scrn.drawLine(120, 131, 200, 131, color);            //bottom line
          } else {
            //depth 3 ######################### facing south
            scrn.drawLine(150, 90, 150, 120, color);               //left corner
            if (maze[level][mazeRow + 3] & 1ul << 30 - mazeCol) {  //wall to the left
              scrn.drawLine(120, 73, 150, 90, color);              //top diagonal
              scrn.drawLine(120, 131, 150, 120, color);            //bottom diagonal
            } else if (maze[level][mazeRow + 4] & 1ul << 30 - mazeCol) {
              //no wall on the left, check ahead 1 on the left
              scrn.drawLine(120, 90, 150, 90, color);    //top straight
              scrn.drawLine(120, 120, 150, 120, color);  //bottom straight
            }
            scrn.drawLine(170, 90, 170, 120, color);               //right corner
            if (maze[level][mazeRow + 3] & 1ul << 32 - mazeCol) {  //wall to the right
              scrn.drawLine(200, 73, 170, 90, color);              //top diagonal
              scrn.drawLine(200, 131, 170, 120, color);            //botton diagonal
            } else if (maze[level][mazeRow + 4] & 1ul << 32 - mazeCol) {
              //no wall on the right, check ahead 1 on the right
              scrn.drawLine(200, 90, 170, 90, color);    //top straight
              scrn.drawLine(200, 120, 170, 120, color);  //bottom straight
            }
            if (maze[level][mazeRow + 4] & 1ul << 31 - mazeCol) {  //wall straight ahead
              scrn.drawLine(150, 90, 170, 90, color);              //top line
              scrn.drawLine(150, 120, 170, 120, color);            //bottom line
            } else {
              //depth 4 ######################### facing south
              scrn.drawLine(158, 95, 158, 117, color);               //center distant line
              if (maze[level][mazeRow + 4] & 1ul << 30 - mazeCol) {  //wall to the left
                scrn.drawLine(150, 90, 158, 95, color);              //top diagonal
                scrn.drawLine(150, 120, 158, 117, color);            //bottom diagonal
              } else if (maze[level][mazeRow + 5] & 1ul << 30 - mazeCol) {
                //no wall on the left, check ahead 1 on the left
                scrn.drawLine(150, 95, 158, 95, color);    //top straight
                scrn.drawLine(150, 117, 158, 117, color);  //bottom straight
              }
              scrn.drawLine(162, 95, 162, 117, color);               //right corner
              if (maze[level][mazeRow + 4] & 1ul << 32 - mazeCol) {  //wall to the right
                scrn.drawLine(170, 90, 162, 95, color);              //top diagonal
                scrn.drawLine(170, 120, 162, 117, color);            //botton diagonal
              } else if (maze[level][mazeRow + 5] & 1ul << 32 - mazeCol) {
                //no wall on the right, check ahead 1 on the right
                scrn.drawLine(170, 95, 162, 95, color);    //top straight
                scrn.drawLine(170, 117, 162, 117, color);  //bottom straight
              }
            }
          }
        }
      }
      break;

    case 3:  //facing west
      //depth 0-------------------------
      scrn.drawLine(20, 12, 20, 164, color);                 //left corner
      if (maze[level][mazeRow + 1] & 1ul << 31 - mazeCol) {  //wall to the left
        scrn.drawLine(0, 0, 20, 12, color);                  //top diagonal
        scrn.drawLine(0, 170, 20, 164, color);               //bottom diagonal
      } else if (maze[level][mazeRow + 1] & 1ul << 32 - mazeCol) {
        //no wall on the left, check ahead 1 on the left
        scrn.drawLine(0, 12, 20, 12, color);    //top straight
        scrn.drawLine(0, 164, 20, 164, color);  //bottom straight
      }
      scrn.drawLine(300, 12, 300, 164, color);               //right corner
      if (maze[level][mazeRow - 1] & 1ul << 31 - mazeCol) {  //wall to the right
        scrn.drawLine(320, 0, 300, 12, color);               //top diagonal
        scrn.drawLine(320, 170, 300, 164, color);            //botton diagonal
      } else if (maze[level][mazeRow - 1] & 1ul << 32 - mazeCol) {
        //no wall on the right, check ahead 1 on the right
        scrn.drawLine(300, 12, 320, 12, color);    //top straight
        scrn.drawLine(300, 164, 320, 164, color);  //bottom straight
      }
      if (maze[level][mazeRow] & 1ul << 32 - mazeCol) {  //wall straight ahead
        scrn.drawLine(20, 12, 300, 12, color);           //top line
        scrn.drawLine(20, 164, 300, 164, color);         //bottom line
      } else {
        //depth 1 ######################### facing west
        scrn.drawLine(80, 48, 80, 145, color);                 //left corner
        if (maze[level][mazeRow + 1] & 1ul << 32 - mazeCol) {  //wall to the left
          scrn.drawLine(20, 12, 80, 48, color);                //top diagonal
          scrn.drawLine(20, 164, 80, 145, color);              //bottom diagonal
        } else if (maze[level][mazeRow + 1] & 1ul << 33 - mazeCol) {
          //no wall on the left, check ahead 1 on the left
          scrn.drawLine(20, 48, 80, 48, color);    //top straight
          scrn.drawLine(20, 145, 80, 145, color);  //bottom straight
        }
        scrn.drawLine(242, 48, 242, 145, color);               //right corner
        if (maze[level][mazeRow - 1] & 1ul << 32 - mazeCol) {  //wall to the right
          scrn.drawLine(300, 12, 242, 48, color);              //top diagonal
          scrn.drawLine(300, 164, 242, 145, color);            //botton diagonal
        } else if (maze[level][mazeRow - 1] & 1ul << 33 - mazeCol) {
          //no wall on the right, check ahead 1 on the right
          scrn.drawLine(300, 48, 242, 48, color);    //top straight
          scrn.drawLine(300, 145, 242, 145, color);  //bottom straight
        }
        if (maze[level][mazeRow] & 1ul << 33 - mazeCol) {  //wall straight ahead
          scrn.drawLine(80, 48, 242, 48, color);           //top line
          scrn.drawLine(80, 145, 242, 145, color);         //bottom line
        } else {
          //depth 2 ######################### facing west
          scrn.drawLine(120, 73, 120, 131, color);               //left corner
          if (maze[level][mazeRow + 1] & 1ul << 33 - mazeCol) {  //wall to the left
            scrn.drawLine(80, 48, 120, 73, color);               //top diagonal
            scrn.drawLine(80, 145, 120, 131, color);             //bottom diagonal
          } else if (maze[level][mazeRow + 1] & 1ul << 34 - mazeCol) {
            //no wall on the left, check ahead 1 on the left
            scrn.drawLine(80, 73, 120, 73, color);    //top straight
            scrn.drawLine(80, 131, 120, 131, color);  //bottom straight
          }
          scrn.drawLine(200, 73, 200, 131, color);               //right corner
          if (maze[level][mazeRow - 1] & 1ul << 33 - mazeCol) {  //wall to the right
            scrn.drawLine(242, 48, 200, 73, color);              //top diagonal
            scrn.drawLine(242, 145, 200, 131, color);            //botton diagonal
          } else if (maze[level][mazeRow - 1] & 1ul << 34 - mazeCol) {
            //no wall on the right, check ahead 1 on the right
            scrn.drawLine(242, 73, 200, 73, color);    //top straight
            scrn.drawLine(242, 131, 200, 131, color);  //bottom straight
          }
          if (maze[level][mazeRow] & 1ul << 34 - mazeCol) {  //wall straight ahead
            scrn.drawLine(120, 73, 200, 73, color);          //top line
            scrn.drawLine(120, 131, 200, 131, color);        //bottom line
          } else {
            //depth 3 ######################### facing west
            scrn.drawLine(150, 90, 150, 120, color);               //left corner
            if (maze[level][mazeRow + 1] & 1ul << 34 - mazeCol) {  //wall to the left
              scrn.drawLine(120, 73, 150, 90, color);              //top diagonal
              scrn.drawLine(120, 131, 150, 120, color);            //bottom diagonal
            } else if (maze[level][mazeRow + 1] & 1ul << 35 - mazeCol) {
              //no wall on the left, check ahead 1 on the left
              scrn.drawLine(120, 90, 150, 90, color);    //top straight
              scrn.drawLine(120, 120, 150, 120, color);  //bottom straight
            }
            scrn.drawLine(170, 90, 170, 120, color);               //right corner
            if (maze[level][mazeRow - 1] & 1ul << 34 - mazeCol) {  //wall to the right
              scrn.drawLine(200, 73, 170, 90, color);              //top diagonal
              scrn.drawLine(200, 131, 170, 120, color);            //botton diagonal
            } else if (maze[level][mazeRow - 1] & 1ul << 35 - mazeCol) {
              //no wall on the right, check ahead 1 on the right
              scrn.drawLine(200, 90, 170, 90, color);    //top straight
              scrn.drawLine(200, 120, 170, 120, color);  //bottom straight
            }
            if (maze[level][mazeRow] & 1ul << 35 - mazeCol) {  //wall straight ahead
              scrn.drawLine(150, 90, 170, 90, color);          //top line
              scrn.drawLine(150, 120, 170, 120, color);        //bottom line
            } else {
              //depth 4 ######################### facing west
              scrn.drawLine(158, 95, 158, 117, color);               //center distant line
              if (maze[level][mazeRow + 1] & 1ul << 35 - mazeCol) {  //wall to the left
                scrn.drawLine(150, 90, 158, 95, color);              //top diagonal
                scrn.drawLine(150, 120, 158, 117, color);            //bottom diagonal
              } else if (maze[level][mazeRow + 1] & 1ul << 36 - mazeCol) {
                //no wall on the left, check ahead 1 on the left
                scrn.drawLine(150, 95, 158, 95, color);    //top straight
                scrn.drawLine(150, 117, 158, 117, color);  //bottom straight
              }
              scrn.drawLine(162, 95, 162, 117, color);               //right corner
              if (maze[level][mazeRow - 1] & 1ul << 35 - mazeCol) {  //wall to the right
                scrn.drawLine(170, 90, 162, 95, color);              //top diagonal
                scrn.drawLine(170, 120, 162, 117, color);            //botton diagonal
              } else if (maze[level][mazeRow - 1] & 1ul << 36 - mazeCol) {
                //no wall on the right, check ahead 1 on the right
                scrn.drawLine(170, 95, 162, 95, color);    //top straight
                scrn.drawLine(170, 117, 162, 117, color);  //bottom straight
              }
            }
          }
        }
      }
      break;
  }

  scrn.drawRect(0, 0, 320, 170, TFT_LIGHTGREY);  //drawBorder();
  drawStats();

  scrn.pushSprite(0, 0);
}

void touchScreen(int x, int y) {
  if (y > 120 && y < 200) {  //the center of the screen was pressed
    switch (facing) {
      case 0:  //north
        if (!(maze[level][mazeRow - 1] & 1ul << 31ul - mazeCol)) {
          mazeRow -= 1;
        }
        break;
      case 1:  //east
        if (!(maze[level][mazeRow] & 1ul << 30ul - mazeCol)) {
          mazeCol += 1;
        }
        break;
      case 2:  //south
        if (!(maze[level][mazeRow + 1] & 1ul << 31ul - mazeCol)) {
          mazeRow += 1;
        }
        break;
      case 3:  //west
        if (!(maze[level][mazeRow] & 1ul << 32ul - mazeCol)) {
          mazeCol -= 1;
        }
        break;
    }
    drawLines();
  }

  if (y > 200) { //the left side of screen was pressed
    facing -= 1;                      //turn left
    if (facing < 0) {
      facing = 3;
    }
    drawLines();
  }
  if (y < 120) { //the right side of screen was pressed
    facing += 1;                       //turn right
    if (facing > 3) {
      facing = 0;
    }
    drawLines();
  }
}

void setup() {
  pinMode(PIN_BUTTON_1, INPUT);  //buttons on dev board
  pinMode(PIN_BUTTON_2, INPUT);
  pinMode(PIN_POWER_ON, OUTPUT);
  digitalWrite(PIN_POWER_ON, HIGH);
  pinMode(PIN_TOUCH_RES, OUTPUT);
  digitalWrite(PIN_TOUCH_RES, LOW);
  delay(500);
  digitalWrite(PIN_TOUCH_RES, HIGH);
  tft.init();
  tft.setRotation(3);            //USB plug on the left
  tft.fillScreen(TFT_BLACK);
  scrn.createSprite(320, 170);
  Wire.begin(PIN_IIC_SDA, PIN_IIC_SCL);
  drawLines();
}

void loop() {
  if (touch.read()) {
    TP_Point t = touch.getPoint(0);
    if (t.x < 170) {
      touchScreen(t.x, t.y);
      delay(250);
    }
  }

  if (!digitalRead(PIN_BUTTON_1) || !digitalRead(PIN_BUTTON_2)) {      //button pressed to view the map
    drawMap();
    while (!digitalRead(PIN_BUTTON_1) || !digitalRead(PIN_BUTTON_2)) { //wait here until button is released
      delay(10);
    }
    drawLines();
  }

  if (mazeCol == 0 || mazeCol == 31 || mazeRow == 0 || mazeRow == mazeTall - 1) {
    //player has made it to one of the sides of the maze
    for (int i = 0; i < 8; i++) {
      tft.setTextColor(TFT_YELLOW, TFT_BLACK);
      tft.drawCentreString(("LEVEL  " + String(level + 1)), 160, 60, 4);
      tft.drawCentreString("COMPLETE", 160, 90, 4);
      delay(250);
      tft.setTextColor(TFT_ORANGE, TFT_BLACK);
      tft.drawCentreString(("LEVEL  " + String(level + 1)), 160, 60, 4);
      tft.drawCentreString("COMPLETE", 160, 90, 4);
      delay(250);
    }
    level++;                    //let's go to the next level
    if (level == levelTotal) {
      level = 0;
    }
    mazeRow = 8;                //start position in the maze
    mazeCol = 7;
    facing = 0;
    delay(3000);
    drawLines();
  }
}
