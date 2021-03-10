#include <API.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

API::API(String url) {
    this->url = url;
}

void API::sendMeasurements(float temperature,float humidity, float air_pressure, float lumionsity, bool movement) {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, url);
    http.addHeader("Content-Type", "application/json");
    
    Serial.println(url);

    int httpCode = http.POST(
        "{\"temperature\":\"" + (String)temperature + "\"" +
        ",\"humidity\":\"" + (String)humidity + "\"" +
        ",\"luminosity\":\"" + (String)lumionsity + "\"" +
        ",\"movement\":\"" + (String)movement +"\"" +
        ",\"air_pressure\":\"" + (String)air_pressure + "\"}");

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