#ifndef CONFIG_H
#define CONFIG_H

#include <FlashStorage.h>

RTCZero rtc;
// Used for NTP

WiFiUDP Udp; // A UDP instance to let us send and receive packets over UDP

// Reserve a portion of flash memory to store an "int" variable
// and call it "my_flash_store".


unsigned int localPort = 2390;      // local port to listen for UDP packets
IPAddress timeServer(129, 6, 15, 28); // time.nist.gov NTP server
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

const int GMT = 1*60*60; //change this to adapt it to your time zone   hours*minutes*seconds


#define DHTPIN      0     // what digital pin we're connected to  
#define DHTTYPE     DHT22   // DHT 22  (AM2302), AM2321
#define RELAY_PIN   1

#define TFT_CS      12
#define TFT_RST     13  // you can also connect this to the Arduino reset in which case, set this #define pin to 0!
#define TFT_DC      11
#define TFT_SCLK    9
#define TFT_MOSI    8

//#define USE_fahrenheit true  // Uncomment this to use Fahrenheit insted of Celsius

class Sensor {
  public:
      Sensor(void);
      void begin();
      bool ReadSensors();
      float GetTemp();
      float GetHumidity();
  private:
      float t;
      float f;
      float h;
};
extern Sensor DHTSensor;

#endif
