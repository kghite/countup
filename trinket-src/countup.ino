// Countup Source Code - Change globals below to update fill times for each wheel

#include <Adafruit_NeoPixel.h>
#include <RTClib.h>

///// Project Settings /////
// Note: Defined constants in arduino don’t take up any program memory space on the chip.
// The compiler will replace references to these constants with the defined value at compile time.

// NeoPixels pin
#define PIN 1


// Define the starting led and pixel count of each NeoPixel wheel
#define NUMPIXELS 56
#define LG_W1 0
#define SM_W1 16
#define LG_W2 28
#define SM_W2 44

#define LG_W1_N 16
#define SM_W1_N 12
#define LG_W2_N 16
#define SM_W2_N 12

// Define the default RGB color
#define R 0
#define G 102
#define B 204

// Cron to sync RTC updates
#define DELAYVAL 30000

//////////

// Create hardware objects
RTC_DS3231 rtc;
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  //Set the date if rebooted
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  pixels.setBrightness(5);
  pixels.begin();
}

void loop() {
  // Set all pixel colors off to start
  pixels.clear();

  // Get the updated time values
  DateTime now = rtc.now();
  int minute = now.minute()+1;
  int day = now.dayOfTheWeek()+1;
  int year = now.year();

  // Update the first large wheel (year)
  TimeSpan span = DateTime(1, 1, year) - now;
  float percent_fill = span.days() / 365.0 * 100.0;
  update_wheel(percent_fill, LG_W1_N, LG_W1);

  // Update the first small wheel (hour)
  percent_fill = minute / 60.0 * 100.0;
  update_wheel(percent_fill, SM_W1_N, SM_W1);

  // Update the second large wheel (10 minutes)
  percent_fill = (minute % 10) / 10.0 * 100.0;
  update_wheel(percent_fill, LG_W2_N, LG_W2);

  // Update the second small wheel (week)
  percent_fill = day / 7.0 * 100.0;
  update_wheel(percent_fill, SM_W2_N, SM_W2);

  delay(DELAYVAL);
}


// Turn on the correct number of pixels in each wheel for the fill percent
void update_wheel(float percent_fill, int wheel_n, int wheel_start) {
  int fill_pixel = int(percent_fill * wheel_n);
  for(int i=wheel_start; i<fill_pixel; i++)  {
    pixels.setPixelColor(i, pixels.Color(R, G, B));
    pixels.show();
  }
}
