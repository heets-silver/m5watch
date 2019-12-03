#include <M5Stack.h>
#include "M5_Watch.h"
#define TFT_GREY 0x5AEB
#define byte uint8_t

extern Ymdhms ymdhms;
uint32_t targetTime = 0;              // for next 1 second timeout
static uint8_t conv2d(const char* p); // Forward declaration needed for IDE 1.6.x
//uint8_t hh = conv2d(__TIME__), mm = conv2d(__TIME__ + 3), ss = conv2d(__TIME__ + 6); // Get H, M, S from compile time
uint8_t hh, mm, ss;
byte omm = 99, oss = 99;
byte xcolon = 0, xsecs = 0;
unsigned int colour = 0;

void init_time()
{
  hh = ymdhms.hh;
  mm = ymdhms.mm;
  ss = ymdhms.ss;
}
void display_watch()
{
  if (targetTime < millis()) {
    // Set next update for 1 second later
    targetTime = millis() + 1000;

    // Adjust the time values by adding 1 second
    ss++;              // Advance second
    if (ss == 60) {    // Check for roll-over
      ss = 0;          // Reset seconds to zero
      omm = mm;        // Save last minute time for display update
      mm++;            // Advance minute
      if (mm > 59) {   // Check for roll-over
        mm = 0;
        hh++;          // Advance hour
        if (hh > 23) { // Check for 24hr roll-over (could roll-over on 13)
          hh = 0;      // 0 for 24 hour clock, set to 1 for 12 hour clock
        }
      }
    }
  }    
  // Update digital time
  int xpos = 0;
  int ypos = 65; // Top left corner ot clock text, about half way down
  int ysecs = ypos + 24;

  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
  if (omm != mm) { // Redraw hours and minutes time every minute
    omm = mm;
    // Draw hours and minutes
    if (hh < 10) xpos += M5.Lcd.drawChar('0', xpos, ypos, 8); // Add hours leading zero for 24 hr clock
    xpos += M5.Lcd.drawNumber(hh, xpos, ypos, 8);             // Draw hours
    xcolon = xpos; // Save colon coord for later to flash on/off later
    xpos += M5.Lcd.drawChar(':', xpos, ypos - 8, 8);
    if (mm < 10) xpos += M5.Lcd.drawChar('0', xpos, ypos, 8); // Add minutes leading zero
    xpos += M5.Lcd.drawNumber(mm, xpos, ypos, 8);             // Draw minutes
    xsecs = xpos; // Sae seconds 'x' position for later display updates
  }

  if (oss != ss) { // Redraw seconds time every second
    oss = ss;
    xpos = xsecs;
  
    if (ss % 2) { // Flash the colons on/off
      M5.Lcd.setTextColor(0x39C4, TFT_BLACK);        // Set colour to grey to dim colon
      M5.Lcd.drawChar(':', xcolon, ypos - 8, 8);     // Hour:minute colon
      xpos += M5.Lcd.drawChar(':', xsecs, ysecs, 6); // Seconds colon
      M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);    // Set colour back to yellow
    }
    else {
      M5.Lcd.drawChar(':', xcolon, ypos - 8, 8);     // Hour:minute colon
      xpos += M5.Lcd.drawChar(':', xsecs, ysecs, 6); // Seconds colon
    }
  
    //Draw seconds
    if (ss < 10) xpos += M5.Lcd.drawChar('0', xpos, ysecs, 6); // Add leading zero
    M5.Lcd.drawNumber(ss, xpos, ysecs, 6);                     // Draw seconds
  }

}

// Function to extract numbers from compile time string
static uint8_t conv2d(const char* p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}
