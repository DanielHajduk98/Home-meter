#include <API.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>

API::API(String url) {
    this->url = url;
}

void API::setup() {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, url + "/monitor");
  http.addHeader("Content-Type", "application/json");
  
  int httpCode = http.POST("{\"macAddress\":\"" + WiFi.macAddress() + "\"}");

  Serial.println(httpCode);

  http.end();  
}

void API::sendMeasurements(
    float temperature,
    float humidity,
    float air_pressure,
    float lumionsity,
    byte movement,
    float heatIndex) 
  {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, url + "/measurement");
    http.addHeader("Content-Type", "application/json");
    
    int httpCode = http.POST(
        "{\"monitor_mac\":\"" + (String)WiFi.macAddress() + "\"" +
        ",\"temperature\":\"" + (String)temperature + "\"" +
        ",\"humidity\":\"" + (String)humidity + "\"" +
        ",\"luminosity\":\"" + (String)lumionsity + "\"" +
        ",\"movement\":\"" + (String)movement +"\"" +
        ",\"heat_index\":\"" + (String)heatIndex +"\"" +
        ",\"air_pressure\":\"" + (String)air_pressure + "\"}");

    Serial.println(url + "/measurement");
    
    if (httpCode > 0) {
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
        Serial.println("received payload:\n<<");
        Serial.println(payload);
        Serial.println(">>");
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
}