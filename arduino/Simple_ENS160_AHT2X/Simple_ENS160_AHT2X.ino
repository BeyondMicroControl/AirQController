/*

https://asciiflow.com/#/ 
https://www.sciosense.com/wp-content/uploads/2023/12/ENS160-Datasheet.pdf

 ┌────────┐       ┌─────────────────────────────────────────────────┐           
 │ CH340N │       │ ENS160 + AHT2X                                  │
 │        │       │ AQI  0 - 5                                      │           
 │    RTS │       │ TVOC 0 – 65,000                                 │           
 │     5V │       │ eCO2 400 – 65,000                               │           
 │    GND ├─────┐ │                                                 │           
 │    RXD ├───┐ │ │  VIN   3V3   GND   SCL   SDA   ADO   CS    INT  │           
 │    TXD │   │ │ └───┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬───┘           
 │    3V3 ├─┐ │ │           │     │     │     └──┐                                             
 └────────┘ │ │ │           │     │     └────┐   │
            │ │ ├───────────│─────┴──────────│───│───────┐           ┌─────────────┐
            │ └─│─────┐     │                │   │       │           │             │
            └───│─────│─────┴────────────┬───│───│───────│─────┐     │     ┌─────┐ │
            ┌───┴─────┴─────┴─────┴───┐  │   │   │   ┌───┴─────┴─────┴─────┴───┐ │ │
            │        @4    @3         │  │   │   │   │  GND   VCC   SCL   SDA  │ │ │
            │╔═ATTiny85══════════════╗│  │   │   │   │╔═SSD1309═══════════════╗│ │ │
            │║ GND   PB4   PB3   PB5 ║│  │   │   │   │║                       ║│ │ │
            │║             TX     ⬤ ║│  │   │   │   │║                       ║│ │ │
            │║ SDA                   ║│  │   │   │   │║      64 x 32 OLED     ║│ │ │
            │║ MISO  MOSI  SCL       ║│  │   │   │   │║                       ║│ │ │
            │║ PB0   PB1   PB2   VCC ║│  │   │   │   │║                       ║│ │ │
            │╚═══════════════════════╝│  │   │   │   │╚═══════════════════════╝│ │ │
            │  @0    @1    @2         │  │   │   │   │                         │ │ │
            └───┬─────┬─────┬─────┬───┘  │   │   │   └─────────────────────────┘ │ │
                │           │     └──────┘   │   │                               │ │
                │           └────────────────┤   │                               │ │
                └────────────────────────────│───┴───────────────────────────────┘ │
                                             └─────────────────────────────────────┘


AQI
5 Unhealthy 10-25 mg/m3 2.2-2.5ppm
4 Poor      3-10. mg/m3 0.65-2.2   ppm
3 Moderate. 1-3.  mg/m3 0.22-0.65  ppm
2 Good      0.3-1 mg/m3 0.065-0.22 ppm
1 Excellent <0.3  mg/m3 0-0.065    ppm

eCO2 / CO2 
>1500 Bad Heavily contaminated indoor air / Ventilation required
1000 - 1500 Poor Contaminated indoor air / Ventilation recommended
800 - 1000 Fair Optional ventilation
600 - 800 Good Average
400 - 600 Excellent 
*/


#define AHT21
#define ENS160
//#define RS232
//#define SSD1309

#ifdef RS232
  #include <SoftwareSerial.h>
  const uint8_t RX_PIN = 3; // PB3
  const uint8_t TX_PIN = 4; // PB4
  SoftwareSerial DebugSerial(RX_PIN, TX_PIN);
  #define DBG_PRINTLN(x) DebugSerial.println(x)
  #define DBG_PRINT(x)   DebugSerial.print(x)
#else
  #define DBG_PRINTLN(x)
  #define DBG_PRINT(x)
#endif

#ifdef AHT21
  #include "AHT21_minimal.h"
#endif

#ifdef ENS160
  #include "ENS160_minimal.h"
  //const uint8_t LED_PIN = PB3;   // PB0 for example
#endif

#ifdef SSD1309
  #include <Tiny4kOLED.h>
  #include "zxpix_font.h"

  #define FONTZXPIX (&TinyOLEDFontzxpix)
  const DCfont *currentFont = FONTZXPIX;
#endif

void setup()
{
    delay(5000);

    #ifdef RS232
      DebugSerial.begin(9600);   // ok at 1 MHz
    #endif

    #ifdef AHT21
    if (!aht21_begin())
    {
      #ifdef RS232
      DBG_PRINT(F("AHT21-ERROR "));
      #endif
      while(true);
    }
    #endif

    #ifdef ENS160
      if (!ens160_begin()) {
        // 3 quick blinks = error
        for (uint8_t i = 0; i < 3; i++)
        {
          DBG_PRINT(F("ENS160-ERROR "));
        }
      }
    #endif

    #ifdef SSD1309
      oled.begin(64, 32, sizeof(tiny4koled_init_64x32r), tiny4koled_init_64x32r);
      //oled.begin(64, 32, sizeof(tiny4koled_init_64x32br), tiny4koled_init_64x32br);  // extra bright
      oled.enableChargePump(); // The default is off, but most boards need this.
      oled.setFont(currentFont);
      oled.clear();
      oled.switchRenderFrame();
      oled.clear();
      oled.on();
    #endif    
}

void loop()
{
  #ifdef AHT21
    AHT21_Measurement aht;
    if (aht21_read(aht))
    {
      #ifdef RS232
        DBG_PRINT(F("tempC*10:")); DBG_PRINT(aht.temperature10);
        DBG_PRINT(F(",humidP*10:")); DBG_PRINT(aht.humidity10);
      #endif

      #ifdef SSD1309
        oled.setCursor(0, 0);
        oled.print( "T " ); oled.print( aht.temperature10 ); oled.print( " " );
        oled.setCursor(0, 1);
        oled.print( "H " ); oled.print( aht.humidity10 ); oled.print( " " );
      #endif
    }
    else
    {
      #ifdef RS232
      DBG_PRINTLN("- (AHT21)");
      #endif
    }
    delay(500);
  #endif

  #ifdef ENS160
   ENS160_Measurement m;
   bool b = ens160_read(m);

  if(b) 
  {
    #ifdef RS232
      // AQI - TVOC - eCO2
      DBG_PRINT(F(",AQI*100:")); DBG_PRINT(m.aqi*100);
      DBG_PRINT(F(",TVOC:")); DBG_PRINT(m.tvoc);
      DBG_PRINT(F(",eCO2-400:")); DBG_PRINTLN(m.eco2-400);
    #endif

    #ifdef SSD1309
      oled.setCursor(0, 2);
      oled.print("C "); oled.print(m.eco2-400);   oled.print(" Q"); oled.print(m.aqi); oled.print( " " );

      oled.setCursor(0, 3);
      oled.print("VOC "); oled.print(m.tvoc); oled.print( " " );
    #endif
  }
  else
  {
    #ifdef RS232
      DBG_PRINTLN("- (ENS160)");
    #endif
  }
    #ifdef SSD1309
        oled.switchFrame();
    #endif

  delay(500);
  #endif
}
