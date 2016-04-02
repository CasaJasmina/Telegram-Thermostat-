 
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
#include <WiFiSSLClient.h>
#include <WiFi101.h>
#include <TelegramBot.h>
#include <WiFiUdp.h>  
#include <RTCZero.h>
#include "config.h"
#include <time.h>
#include <FlashStorage.h>

RTCZero rtc;
// Used for NTP

WiFiUDP Udp; // A UDP instance to let us send and receive packets over UDP

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

Sensor DHTSensor;

unsigned long timer;
unsigned long TeleTimer;
int counter = 0;
int HomeTimer = 0;

bool TeleFlag = false;
bool AutoMode = false;
bool ManualMode = false;
bool online = false;
bool RelayStatus = false;
bool flag = false;
int day_counter = 1;
int STATE;
int default_temp;


typedef struct _day {
  bool hours[24] = {false};
} Programma;

typedef struct _autom {
    Programma days [7];
} WeekAutomation;

WeekAutomation days;

message m;

FlashStorage(my_flash_store, typeof(default_temp));
FlashStorage(my_flash_store_two, typeof(days));


// Initialize Wifi connection to the router
char ssid[] = "xxxx";             // your network SSID (name)
char pass[] = "yyyy";           // your network key

// Initialize Telegram BOT
const String BotToken = "****";
const String BotName = "****";
const String BotUsername = "****";

WiFiSSLClient client;
TelegramBot bot (BotToken, BotName, BotUsername, client);

volatile bool buttonUpPressed = false;
volatile bool buttonEnterPressed = false;
volatile bool buttonDownPressed = false;

void setup() {

  Serial.begin(115200);

  DHTSensor.begin();
  ScreenBegin();

  pinMode(1, INPUT);
  pinMode(0, INPUT);
  pinMode(4, INPUT);

  attachInterrupt(1, buttonUpPressedCallback, RISING);
  attachInterrupt(4, buttonEnterPressedCallback, RISING);
  attachInterrupt(0, buttonDownPressedCallback, RISING);

  my_flash_store.read(&default_temp);
  my_flash_store_two.read(&days);


  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  // attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");

  rtc.begin();
  GetCurrentTime();
  // set_zone(GMT*ONE_HOUR);
  bot.begin();
  Display_Home();


}

void loop() {

  while (!DHTSensor.ReadSensors()) {
    delay(1000);
  }

  if (online == false && millis() - TeleTimer > 10 * 1000) { // check telgram every 10 second
    m = bot.getUpdates(); // Read new messages
    if ( m.text.equals("Start")) {
      STATE = 5;
      TeleFlag = true;
      online = true;
      TelegramMode();
    }
    TeleTimer = millis();
  }

  switch  (STATE) {

    case 0:
      Home();
      break;

    case 1:
      NavMenu();
      break;

    case 2:
      Auto();
      break;

    case 3:
      Manual();
      break;

    case 4:
      Prog();
      break;

    case 5:
      m = bot.getUpdates();
      TelegramMode();
      break;

  }


}

void buttonUpPressedCallback() {
  buttonUpPressed = true;
}

void buttonEnterPressedCallback() {
  buttonEnterPressed = true;
}
void buttonDownPressedCallback() {
  buttonDownPressed = true;
}

bool isButtonPressed(int i) {
  return (digitalRead(i) == LOW);
}
