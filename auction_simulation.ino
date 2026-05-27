/*
Auction simulation: Random

***
Hardware Requirement: 
UNO, Analog Joystick and ST7735S
***

1.  Wiring: ST7735S TFT + Arduino Uno
TFT Pin	Arduino Uno	Function
VCC	5V	Power
GND	GND	Ground
CS	D10	Chip Select
RESET	D8	Reset
DC or A0	D9	Data/Command select
SDA	D11	SPI Data (MOSI)
SCL	D13	SPI Clock (SCK)
LED or BLK	3.3V or 100Ω to 5V	Backlight
2. Analog Joystick to Arduino Uno (unchanged)
Joystick Pin	Arduino Uno
GND	GND
VCC	5V
VRx	A0
VRy	A1
SW	D2
*/


#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

// TFT Pins
#define TFT_CS     10
#define TFT_RST    9
#define TFT_DC     8

// Define Red and Blue colors (RGB565)
#define COLOR_RED   0xF800  // Red (255, 0, 0)
#define COLOR_BLUE  0x001F  // Blue (0, 0, 255)

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

#define SCREEN_W 128
#define SCREEN_H 160

// Graph position and size
#define GRAPH_X 15
#define GRAPH_Y 80     // Positioned below labels
#define GRAPH_W 98
#define GRAPH_H 60

#define MAX_POINTS GRAPH_W

float bidder1Bids[MAX_POINTS];
float bidder2Bids[MAX_POINTS];

void setup() {
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);

  drawLabels();
  clearGraphArea();

  for (int i = 0; i < MAX_POINTS; i++) {
    bidder1Bids[i] = 0;
    bidder2Bids[i] = 0;
  }
}

void loop() {
  float newBid1 = random(10, GRAPH_H);
  float newBid2 = random(10, GRAPH_H);

  shiftLeftAndAdd(newBid1, newBid2);

  clearGraphArea();
  drawGraph();
  drawWinner(newBid1, newBid2);

  delay(150);
}

void shiftLeftAndAdd(float b1, float b2) {
  for (int i = 0; i < MAX_POINTS - 1; i++) {
    bidder1Bids[i] = bidder1Bids[i + 1];
    bidder2Bids[i] = bidder2Bids[i + 1];
  }
  bidder1Bids[MAX_POINTS - 1] = b1;
  bidder2Bids[MAX_POINTS - 1] = b2;
}

void drawGraph() {
  // Draw bidder 1 in blue
  for (int i = 0; i < MAX_POINTS - 1; i++) {
    int x1 = GRAPH_X + i;
    int y1 = GRAPH_Y + GRAPH_H - (int)bidder1Bids[i];
    int x2 = GRAPH_X + i + 1;
    int y2 = GRAPH_Y + GRAPH_H - (int)bidder1Bids[i + 1];
    tft.drawLine(x1, y1, x2, y2, COLOR_BLUE);
  }

  // Draw bidder 2 in red
  for (int i = 0; i < MAX_POINTS - 1; i++) {
    int x1 = GRAPH_X + i;
    int y1 = GRAPH_Y + GRAPH_H - (int)bidder2Bids[i];
    int x2 = GRAPH_X + i + 1;
    int y2 = GRAPH_Y + GRAPH_H - (int)bidder2Bids[i + 1];
    tft.drawLine(x1, y1, x2, y2, COLOR_RED);
  }
}

void clearGraphArea() {
  tft.fillRect(GRAPH_X, GRAPH_Y, GRAPH_W, GRAPH_H, ST77XX_BLACK);
}

void drawLabels() {
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);

  // Auction math formulas (top of screen)
  tft.setCursor(2, 0);
  tft.println("1. Winner = argmax_i b_i");

  tft.setCursor(2, 10);
  tft.println("2. Price = max_i b_i");

  tft.setCursor(2, 20);
  tft.println("3. u_i = v_i - p_i");

  tft.setCursor(2, 30);
  tft.println("4. W = sum_i u_i");

  tft.setCursor(2, 40);
  tft.println("5. Eff = W / W_opt");

  // Bidder labels below formulas
  tft.setTextColor(COLOR_BLUE);
  tft.setCursor(10, 55);
  tft.println("Bidder 1");

  tft.setTextColor(COLOR_RED);
  tft.setCursor(70, 55);
  tft.println("Bidder 2");
}

void drawWinner(float b1, float b2) {
  // Clear previous winner text area
  tft.fillRect(0, 66, SCREEN_W, 10, ST77XX_BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, 66);

  if (b1 > b2) {
    tft.setTextColor(COLOR_BLUE);
    tft.print("Winner: Bidder 1");
  } else if (b2 > b1) {
    tft.setTextColor(COLOR_RED);
    tft.print("Winner: Bidder 2");
  } else {
    tft.setTextColor(ST77XX_WHITE);
    tft.print("Winner: Tie");
  }
}

