#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Conexion de pines mini oled, GND>GND,VCC>3v3,D0>pin10,D1>pin9,RES>pin13,DC>pin11,CS>pin12
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

byte sensorInterrupt = 0;  // 0 = digital pin 2
byte sensorPin       = 2;

// The hall-effect flow sensor outputs approximately 4.5 pulses per second per
// litre/minute of flow.
float calibrationFactor = 5.4;

volatile byte pulseCount;  

float flowRate;
unsigned int flujomiliLitros;
unsigned long totalMiliLitros;
double totalconsumido;

unsigned long oldTime;

void setup()   {                

 display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  //configurando flujometro

  pulseCount        = 0;
  flowRate          = 0.0;
  flujomiliLitros   = 0;
  totalMiliLitros  = 0;
  totalconsumido    = 0.0;
  oldTime           = 0;

  // The Hall-effect sensor is connected to pin 2 which uses interrupt 0.
  // Configured to trigger on a FALLING state change (transition from HIGH
  // state to LOW state)
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);

 //fin config flujometro
 
}

void loop()
{
  
  //obtener datos sensor flujo
  if((millis() - oldTime) > 1000)    // Only process counters once per second
  { 
    detachInterrupt(sensorInterrupt);
        
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    
    oldTime = millis();
    
    // Divide the flow rate in litres/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to
    // convert to millilitres.
    flujomiliLitros = (flowRate / 60) * 1000;
    
    // Add the millilitres passed in this second to the cumulative total
    totalMiliLitros += flujomiliLitros;
    totalconsumido=totalMiliLitros/1000.0;  
    unsigned int frac;
    // Print the flow rate for this second in litres / minute
    /* Serial.print("Flujo actual: ");
    Serial.print(int(flowRate));  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\t");       // Print tab space

    // Print the cumulative total of litres flowed since starting
    Serial.print("Consumo Total de Liquido: ");        
    Serial.print(totalMiliLitros);
    Serial.println("mL"); 
    Serial.print("\t");       // Print tab space
    Serial.print(totalconsumido);
    Serial.print("L");
    */
    // Reset the pulse counter so we can start incrementing again
    pulseCount = 0;
    
    // Enable the interrupt again now that we've finished sending output
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  }
  //fin obtener datos sensor
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Flujo Actual");

  display.setTextSize(2);
  //display.setTextColor(WHITE);
  display.setTextColor(WHITE,BLACK);
  display.setCursor(0, 17);
  display.println(int(flowRate));
  display.print(" ");
  display.setCursor(40, 16);
  display.println(" L/min");
  
  display.setTextSize(1);
  display.setTextColor(WHITE,BLACK);
  display.setCursor(0, 39);
  display.println("Consumo Total");
  
  display.setTextSize(2);
  display.setTextColor(WHITE,BLACK);
  display.setCursor(0, 50);
  display.println(totalconsumido);
  
  display.setTextSize(2);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(80, 50);
  display.println("L");
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(91, 56);
  display.print("itros");
  
  display.display();
  delay(1);
  display.clearDisplay();
  }

void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
}
