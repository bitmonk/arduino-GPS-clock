#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

// Tens senconds on a single didisplay using GMT from Adafruit GPS

SoftwareSerial mySerial(3, 2);
Adafruit_GPS GPS(&mySerial);
#define GPSECHO  true
boolean usingInterrupt = false;
void useInterrupt(boolean);

void setup() {
  Serial.begin(115200);
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);
  useInterrupt(true);
  
  pinMode(5, OUTPUT);  
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(12, 0);  // start with the "dot" off
  
  delay(1000);
}

SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
#ifdef UDR0
  if (GPSECHO)
    if (c) UDR0 = c;  
    // writing direct to UDR0 is much much faster than Serial.print 
    // but only one character can be written at a time. 
#endif
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}

uint32_t timer = millis();

void loop() {
 if (! usingInterrupt) {
  char c = GPS.read();
  if (GPSECHO)
      if (c) Serial.print(c);
 }
 if (GPS.newNMEAreceived()) {
   if (!GPS.parse(GPS.lastNMEA()))
     return;
 }
 if (timer > millis())  timer = millis();
 if (millis() - timer > 1000) {
   timer = millis();
   Serial.print("\nTime: ");
   Serial.print(GPS.hour, DEC); Serial.print(':');
   Serial.print(GPS.minute, DEC); Serial.print(':');
   Serial.print(GPS.seconds, DEC); Serial.print('.');
   Serial.println(GPS.milliseconds);
 }
 int a = (GPS.seconds/10);
 switch (a) {
 case 9:
 // write '9'
 digitalWrite(5, 1);
 digitalWrite(6, 1);
 digitalWrite(7, 1);
 digitalWrite(8, 0);
 digitalWrite(9, 0);
 digitalWrite(10, 1);
 digitalWrite(11, 1);
 break;
 case 8:
 // write '8'
 digitalWrite(5, 1);
 digitalWrite(6, 1);
 digitalWrite(7, 1);
 digitalWrite(8, 1);
 digitalWrite(9, 1);
 digitalWrite(10, 1);
 digitalWrite(11, 1);
 break;
 case 7:
 // write '7'
 digitalWrite(5, 1);
 digitalWrite(6, 1);
 digitalWrite(7, 1);
 digitalWrite(8, 0);
 digitalWrite(9, 0);
 digitalWrite(10, 0);
 digitalWrite(11, 0);
 break;
 case 6:
 // write '6'
 digitalWrite(5, 1);
 digitalWrite(6, 0);
 digitalWrite(7, 1);
 digitalWrite(8, 1);
 digitalWrite(9, 1);
 digitalWrite(10, 1);
 digitalWrite(11, 1);
 break;
 case 5:
 // write '5'
 digitalWrite(5, 1);
 digitalWrite(6, 0);
 digitalWrite(7, 1);
 digitalWrite(8, 1);
 digitalWrite(9, 0);
 digitalWrite(10, 1);
 digitalWrite(11, 1);
 break;
 case 4:
 // write '4'
 digitalWrite(5, 0);
 digitalWrite(6, 1);
 digitalWrite(7, 1);
 digitalWrite(8, 0);
 digitalWrite(9, 0);
 digitalWrite(10, 1);
 digitalWrite(11, 1);
 break;
 case 3:
 // write '3'
 digitalWrite(5, 1);
 digitalWrite(6, 1);
 digitalWrite(7, 1);
 digitalWrite(8, 1);
 digitalWrite(9, 0);
 digitalWrite(10, 0);
 digitalWrite(11, 1);
 break;
 case 2:
 // write '2'
 digitalWrite(5, 1);
 digitalWrite(6, 1);
 digitalWrite(7, 0);
 digitalWrite(8, 1);
 digitalWrite(9, 1);
 digitalWrite(10, 0);
 digitalWrite(11, 1);
 break;
 case 1:
 // write '1'
 digitalWrite(5, 0);
 digitalWrite(6, 1);
 digitalWrite(7, 1);
 digitalWrite(8, 0);
 digitalWrite(9, 0);
 digitalWrite(10, 0);
 digitalWrite(11, 0);
 break;
 case 0:
 // write '0'
 digitalWrite(5, 1);
 digitalWrite(6, 1);
 digitalWrite(7, 1);
 digitalWrite(8, 1);
 digitalWrite(9, 1);
 digitalWrite(10, 1);
 digitalWrite(11, 0);
 break;
 }
}
