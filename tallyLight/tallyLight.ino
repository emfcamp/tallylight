#include "FastLED.h"
#include <ETH.h>

static bool eth_connected = false;

#define NUM_LEDS 11
#define DATA_PIN 32
CRGB leds[NUM_LEDS];

#define SW1 34 // Mode switch
#define SW2 35 // Brightness switch

#define PHY_EN 16 // PHY clock enable, write HIGH to enable

void setup() {
  pinMode(PHY_EN, OUTPUT);
  digitalWrite(PHY_EN, HIGH);
  delay(500);

  Serial.begin(115200);
  Serial.println("Good morning :3");

  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);

  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);

  WiFi.onEvent(WiFiEvent);
  ETH.begin(0);
}

void loop() {
  for (int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
    FastLED.setBrightness(digitalRead(SW2) ? 255 : 30);
    leds[whiteLed] = digitalRead(SW1) ? CRGB::Red : CRGB::Green;
    FastLED.show();
    delay(20);
    leds[whiteLed] = CRGB::Black;
  }
}

void WiFiEvent(WiFiEvent_t event)
{
  switch (event) {
    case ARDUINO_EVENT_ETH_START:
      Serial.println("ETH Started");
      //set eth hostname here
      ETH.setHostname("esp32-ethernet");
      break;
    case ARDUINO_EVENT_ETH_CONNECTED:
      Serial.println("ETH Connected");
      break;
    case ARDUINO_EVENT_ETH_GOT_IP:
      Serial.print("ETH MAC: ");
      Serial.print(ETH.macAddress());
      Serial.print(", IPv4: ");
      Serial.print(ETH.localIP());
      if (ETH.fullDuplex()) {
        Serial.print(", FULL_DUPLEX");
      }
      Serial.print(", ");
      Serial.print(ETH.linkSpeed());
      Serial.println("Mbps");
      eth_connected = true;
      break;
    case ARDUINO_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      eth_connected = false;
      break;
    case ARDUINO_EVENT_ETH_STOP:
      Serial.println("ETH Stopped");
      eth_connected = false;
      break;
    default:
      break;
  }
}
