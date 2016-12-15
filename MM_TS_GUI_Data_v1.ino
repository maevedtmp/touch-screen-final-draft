/////////////////////////////////////////////////
////////arduino & touchscreen sound toy//////////
/////////////////////////////////////////////////

//libraries
#include <stdint.h>
#include <TouchScreen.h>
#include <TFTv2.h>
#include <SPI.h>

//option display
int OptionWidth = 30; //width is along y-axis
int OptionHeight = 60; //height is along x-axis
unsigned int colours[4] = {0xFCE3, 0x2FFE, 0xC1DF, 0xF98E};

//touchscreen
//for better pressure precision, resistance between X+ and X- must be known
//(read with a multimeter). this touch sheild is 300 ohms
TouchScreen ts = TouchScreen(XP, YP, XM, YM); //initialise touchscreen
int px, py; //position x, position y
int ppx, ppy; //previous position x, previous position y

void setup() {
  TFT_BL_ON; //turn on backlight
  Tft.TFTinit(); //initialise TFT library
  Serial.begin(115200); //start serial
  //draw option selector
  for (int i = 0; i < 4; i++)
  {
    Tft.fillRectangle(i * 60, 0, OptionHeight, OptionWidth, colours[i]);
  }
}

void loop() {

  Point p = ts.getPoint(); //point object holds x, y, & z co-ordinates

  //map ADC calue read into pixel co-ordinates
  px = map(p.x, TS_MINX, TS_MAXX, 0, 240);
  py = map(p.y, TS_MINY, TS_MAXY, 0, 320);

  //pressure of 0 means no pressing on screen!
  if (p.z > __PRESURE) { //if touchscreen is pressed
    if (py > OptionWidth + 25) { //excluding the options strip
      //draw circles to show location (px/py)
      if (ppx != px || ppy != py) { //if ppx/ppy do not equal px/py
        Tft.fillCircle(ppx, ppy, 25, BLACK); //erase previous image
      }
      for (int r = 0; r < 25; r = r + 2) //circles increase in size
      {
        Tft.drawCircle(px, py, r, random(0xFFFF)); //draw circles centre (px, py), radius r, random colour
        //set ppx/ppy values to px/py values
        ppx = px;
        ppy = py;
        //circles continue to draw whenever pressure is detected outside options strip
        Serial.write((byte) (px >> 7));
        Serial.write((byte) (px % 128));
        Serial.write((byte) (py >> 7));
        Serial.write((byte) (py % 128));
        Serial.write((byte) 255); //end of packet signifier
      }
    }
    if (py < OptionWidth) {
      Serial.write((byte) (px >> 7));
      Serial.write((byte) (px % 128));
      Serial.write((byte) (py >> 7));
      Serial.write((byte) (py % 128));
      Serial.write((byte) 255); //end of packet signifier
    }
  }
  delay(20);
}
//end of file! :-)
