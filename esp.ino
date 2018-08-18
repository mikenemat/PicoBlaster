#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)

#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <fauxmoESP.h>


WiFiManager wifiManager;
fauxmoESP fauxmo;
int action = 0; // 0 = Nothing, 1 = ON, 2 = OFF, 3 = DIM
int dimVal = 0; // Dim value 0-255

int OFF_SECONDS = 3; // Number of seconds it takes the target device to turn off from full brightness after OFF pressed, for dimmer calibration
int DIM_SECONDS = 5; // Number of seconds it takes the target device to turn on to full brightness from OFF using DIM UP button, for dimmer calibration

int PIN_ON = 12;
int PIN_OFF = 14;
int PIN_DIM_DOWN = 13;
int PIN_DIM_UP = 15;

//GPIO12 ESP = ON = SI4010 PIN 12/GPIO2
//GPIO14 ESP = OFF = SI4010 PIN 10/GPIO4
//GPIO13 ESP = DIM DOWN = SI4010 PIN 14/GPIO8
//GPIO15 ESP = DIM UP = SI4010 PIN 8/GPIO6

void setup()
{
  pinMode(PIN_ON, OUTPUT);
  pinMode(PIN_OFF, OUTPUT);
  pinMode(PIN_DIM_DOWN, OUTPUT);
  pinMode(PIN_DIM_UP, OUTPUT);
  digitalWrite(PIN_ON, HIGH);
  digitalWrite(PIN_OFF, HIGH);
  digitalWrite(PIN_DIM_DOWN, HIGH);
  digitalWrite(PIN_DIM_UP, HIGH);
  
  Serial.begin(115200);

  wifiManager.setConfigPortalTimeout(3000);
  if (!wifiManager.autoConnect()) {
    Serial.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.reset();
    delay(5000);
  }

  Serial.println("connected...yeey :)");
  Serial.println("local ip");
  Serial.println(WiFi.localIP());

  fauxmo.addDevice("Shed Light");
  fauxmo.enable(true);
  fauxmo.enable(false);
  fauxmo.enable(true);

  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
    Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
    if (state == true && value == 255) {
      action = 1;
    } else if (state == true && value > 0) {
      action = 3;
      dimVal = value;
    } else {
      action = 2;
    }
  });
}

void loop()
{
  fauxmo.handle();

  if (action == 1) {
    digitalWrite(PIN_ON, LOW);
    delay(100);
    digitalWrite(PIN_ON, HIGH);
    action = 0;
  } else if (action == 3) {
    digitalWrite(PIN_OFF, LOW);
    delay(100);
    digitalWrite(PIN_OFF, HIGH);
    delay(1000 * OFF_SECONDS);
    int waitVal = (DIM_SECONDS * 1000) * ((float)((float)dimVal / 255));
    Serial.println("DIM PRESS DIMVAL: " + String (dimVal) + " WAITVAL: "+ String(waitVal));
    digitalWrite(PIN_DIM_UP, LOW);
    delay(waitVal);
    digitalWrite(PIN_DIM_UP, HIGH);
    Serial.println("DIM RELEASE DIMVAL: " + String (dimVal) + " WAITVAL: " + String(waitVal));
    action = 0;
  } else if (action == 2) {
    digitalWrite(PIN_OFF, LOW);
    delay(100);
    digitalWrite(PIN_OFF, HIGH);
    action = 0;
  }
}


