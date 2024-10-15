#include <U8x8lib.h> // Refer to this site: https://github.com/olikraus/u8g2/wiki

// Pin Map
#define SDA 21 // SDA Pin is GPIO 21 for SH1106 OLED
#define SCL 22 // SCL Pin is GPIO 22 for SH1106 OLED

//#ifdef U8X8_HAVE_HW_SPI
//#include <SPI.h>
//#endif

// Please UNCOMMENT one of the contructor lines below
// U8x8 Contructor List 
// The complete list is available here: https://github.com/olikraus/u8g2/wiki/u8x8setupcpp
// Please update the pin numbers according to your setup. Use U8X8_PIN_NONE if the reset pin is not connected
//U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE); 	      
//U8X8_SSD1306_128X64_ALT0_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE); 	      // same as the NONAME variant, but may solve the "every 2nd line skipped" problem
//U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 2, /* data=*/ 0, /* reset=*/ U8X8_PIN_NONE); 	      // Digispark ATTiny85
//U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // OLEDs without Reset of the Display
U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);
//unsigned int RefreshDisplay;
uint32_t Prv_Display, RefreshPeriod_Display = 500;
uint8_t Screen;

// Timer
uint32_t hh, hh1, hh2, mm, mm1, mm2, ss, ss1, ss2, Old_ss2; // Hour, Minute, Second

void setup(void)
{  
  // Initialize Serial communication
  Serial.begin(115200);

  // Initialize OLED
  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  //u8x8.setFont(u8x8_font_8x13_1x2_r); // 4 Lines
  //u8x8.setFont(u8x8_font_victoriabold8_r); // 8 Lines Bold

  // Intermission before working
  delay(1000);
  
}

void loop(void)
{

  Screen++;
  UpdateTime();
  Display_u8x8();
  delay(500);
  
}

void UpdateTime() { // Timer
  ss = millis() / 1000;
  
  mm = ss / 60;
  
  hh = ss / 3600;
  hh2 = hh % 10;
  hh1 = hh / 10;

  ss = ss - mm * 60;
  ss2 = ss % 10;
  ss1 = ss / 10;

  mm = mm - hh * 60;
  mm2 = mm % 10;
  mm1 = mm / 10;
}

void Display_u8x8() {
  // Clear display
  if((millis() - Prv_Display) > RefreshPeriod_Display) {
    u8x8.clearDisplay();
    Prv_Display = millis();
  }
  
  // Timer
  u8x8.setCursor(3, 0); 
  u8x8.setInverseFont(1);
  u8x8.println(String(" Screen ") + Screen);  
  u8x8.setInverseFont(0);
  u8x8.println(String(""));
  u8x8.println(String("") + hh1 + hh2 + String(":") + mm1 + mm2 + String(":") + ss1 + ss2);
  u8x8.println(String(""));
  
  //u8x8.refreshDisplay();    // only required for SSD1606/7
}
