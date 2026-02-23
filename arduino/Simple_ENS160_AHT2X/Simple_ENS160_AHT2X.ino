/*
GND━┯━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┯━━━━━━━┯━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┯━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
3V3─│─┬────────┬───────────────────────────│───────│─┬──────────────────────────────────────────┬─│────────────────────────────────────
    │ │ ┌──────│───────────────────────────│───────│─│────────────────────────────────────────┐ │ │
    │ │ │      │    ╔═══════════════════╗  │       │ │                                        │ │ │ ╔═════════════════════════════════╗
    │ │ │      │    ║ ● [U1 ATTinyX26]  ║  │       │ │                                        │ │ │ ║ [ENS160+AHT2X_1]                ║
    │ │ │      │    ║                   ║  │       │ │  ╔══════════════════════════╗          │ │ │ ║ AQI  0-5                        ║
    │ │ │      └────╢ VCC           GND ╟──┘       │ └──╢DC+ [8 Solid State Relay] ║          │ │ │ ║ TVOC 0-65000                    ║
    │ │ │           ╢ PA4       SCK PA3 ╟────────┐ └────╢DC-   Low Level Trigger   ║          │ │ │ ║ eCO2 400-65000                  ║
    │ │ │           ╢ PA5  RX1 MISO PA2 ╟──────┐ └──────╢CH1                       ║          │ │ │ ║                                 ║
    │ │ │ ┌─────────╢ PA6  TX1 MOSI PA1 ╟────┐ └────────╢CH2                       ║          │ │ │ ║ VIN 3V3 GND SCL SDA ADO  CS INT ║
    │ │ │ │ ┌───────╢ PA7      UPDI PA0 ╟▶   └──────────╢CH3     ┏━━━━━━━━┓  SW1⌈⊘⌉╟          │ │ │ ╚══╤═══╤═══╤═══╤═══╤═══╤═══╤═══╤══╝
    │ │ │ │ │ ┌─────╢ PB5           PC3 ╟───────────────╢CH4     ┗━━━━━━━━┛     ⌊⊘⌋╟          │ │ │        │   │   │   │
    │ │ │ │ │ │ ┌───╢ PB4           PC2 ╟───────────────╢CH5     ┏━━━━━━━━┓  SW2⌈⊘⌉╟          │ ├─│────────┘   │   │   └───────┐
    │ │ └─│─│─│─│───╢ PB3 RX0       PC1 ╟───────────────╢CH6     ┗━━━━━━━━┛     ⌊⊘⌋╟          │ │ ├────────────┘   └─────────┐ │
    │ │   │ │ │ │  ◀╢ PB2 TX0       PC0 ╟───────────────╢CH7     ┏━━━━━━━━┓  SW3⌈⊘⌉╟          │ │ └────────────────────────┐ │ │
    │ │ ┌─│─│─│─│───╢ PB1 SDA   SCL PB0 ╟──┐            ╢CH8     ┗━━━━━━━━┛     ⌊⊘⌋╟          │ │                          │ │ │
    │ │ │ │ │ │ │   ╚═══════════════════╝  │            ╢⌈⊘⌉DC+  ┏━━━━━━━━┓  SW4⌈⊘⌉╟          │ │  ╔════════════════════╗  │ │ │
    │ └─│─│─│─│─│──────────────────┐       │            ╢⌊⊘⌋DC-  ┗━━━━━━━━┛     ⌊⊘⌋╟          │ │  ║ ●  [U2 ATTinyX12]  ║  │ │ │
    ├───│─│─│─│─│──────────────┐   │       │            ╢⌈⊘⌉CH1  ┏━━━━━━━━┓  SW5⌈⊘⌉╟          │ │  ║                    ║  │ │ │
    │   │ │ │ │ └──────────┐   │   │       │            ╢⌊⊘⌋CH2  ┗━━━━━━━━┛     ⌊⊘⌋╟          │ └──╢ VCC            GND ╟──┘ │ │
    │   │ │ │ └────────┐   │   │   │       │            ╢⌈⊘⌉CH3  ┏━━━━━━━━┓  SW6⌈⊘⌉╟          │    ║                    ║    │ │
    │   │ │ └──────┐   │   │   │   │       │            ╢⌊⊘⌋CH4  ┗━━━━━━━━┛     ⌊⊘⌋╟          └────╢ PA6 TX     SCK PA3 ╟    │ │
    │  ┌┘ └────┐   │   │   │   │   │       │            ╢⌈⊘⌉CH5  ┏━━━━━━━━┓  SW7⌈⊘⌉╟               ║                    ║    │ │
    │  │   ┌───│───│───│───│───│───│───────┘            ╢⌊⊘⌋CH6  ┗━━━━━━━━┛     ⌊⊘⌋╟               ╢ PA7 RX    UPDI PA0 ╟    │ │
    │  │   │   │   │   │   │   │   │                    ╢⌈⊘⌉CH7  ┏━━━━━━━━┓  SW8⌈⊘⌉╟               ║                    ║    │ │
╔═══╧══╧═══╧═══╧═══╧═══╧═══╧═══╧═══╧═══════════╗        ╢⌊⊘⌋CH8  ┗━━━━━━━━┛     ⌊⊘⌋╟            ┌──╢ PA1 SDA    SCL PA2 ╟────┘ │
║ COM SDA SCL PSH TRA TRB BAK GND VCC          ║        ╚══════════════════════════╝            │  ║                    ║      │
║┌───────────────────────────────┐     ▢▢      ║                                                │  ╚════════════════════╝      │
║│ [SSD1306 + R + B]  64x32 OLED │             ║                                                │                              │
║│                               │   / ⎺⎺ \    ║                                                └──────────────────────────────┘
║│                               │ ⎛        ⎞  ║
║│                               │|          | ║
║│                               │ ⎝        ⎠  ║
║│                               │   \ __ /    ║
║│                               │             ║
║└───────────────────────────────┘     ▢▢      ║
╚══════════════════════════════════════════════╝


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
//#define SOFTSERIAL
//#define SSD1309

#ifdef SOFTSERIAL
  #include <SoftwareSerial.h>
  const uint8_t RX_PIN = 3; // PB3
  const uint8_t TX_PIN = 4; // PB4
  SoftwareSerial DebugSerial(RX_PIN, TX_PIN);
  #define DBG_PRINTLN(x) DebugSerial.println(x)
  #define DBG_PRINT(x)   DebugSerial.print(x)
#else
  #define DBG_PRINTLN(x) Serial.println(x)
  #define DBG_PRINT(x)   Serial.print(x)
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

    #ifdef SOFTSERIAL
      DebugSerial.begin(9600);   // ok at 1 MHz
    #else
      Serial.begin(115200);
    #endif

    #ifdef AHT21
    if (!aht21_begin())
    {
      DBG_PRINT(F("AHT21-ERROR "));
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
      DBG_PRINT(F("tempC*10:")); DBG_PRINT(aht.temperature10);
      DBG_PRINT(F(",humidP*10:")); DBG_PRINT(aht.humidity10);

      #ifdef SSD1309
        oled.setCursor(0, 0);
        oled.print( "T " ); oled.print( aht.temperature10 ); oled.print( " " );
        oled.setCursor(0, 1);
        oled.print( "H " ); oled.print( aht.humidity10 ); oled.print( " " );
      #endif
    }
    else
    {
      DBG_PRINTLN("- (AHT21)");
    }
    delay(500);
  #endif

  #ifdef ENS160
   ENS160_Measurement m;
   bool b = ens160_read(m);

  if(b) 
  {
    // AQI - TVOC - eCO2
    DBG_PRINT(F(",AQI*100:")); DBG_PRINT(m.aqi*100);
    DBG_PRINT(F(",TVOC:")); DBG_PRINT(m.tvoc);
    DBG_PRINT(F(",eCO2-400:")); DBG_PRINTLN(m.eco2-400);


    #ifdef SSD1309
      oled.setCursor(0, 2);
      oled.print("C "); oled.print(m.eco2-400);   oled.print(" Q"); oled.print(m.aqi); oled.print( " " );

      oled.setCursor(0, 3);
      oled.print("VOC "); oled.print(m.tvoc); oled.print( " " );
    #endif
  }
  else
  {
    DBG_PRINTLN("- (ENS160 - no read)");
  }
    #ifdef SSD1309
        oled.switchFrame();
    #endif

  delay(500);
  #endif
}
