#include <API.h>

struct apiCallResult {
     int httpCode;
     String response;
};

/*!
    @brief  Constructor for API object.
    @param  baseUrl
            Server base url
    @note   Requires EEPROM init before starting.
*/
  API::API(String baseUrl) {
    this->baseUrl = baseUrl;
  }

/*!
    @brief  Write String to EEPROM
    @param  addrOffset
            Starting EEPROM addres from where we will start reading
    @param  strToWrite 
            A const reference to a String object.
    @return void.
*/
void API::writeStringToEEPROM(int addrOffset, const String &strToWrite) {
  byte len = strToWrite.length();
  EEPROM.write(addrOffset, len);
  for (int i = 0; i < len; i++) {
    EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
  }
  EEPROM.commit();
}

/*!
    @brief  Read String from EEPROM
    @param  addrOffset
            Starting EEPROM addres from where we will start reading
    @return Http response code.
    @note   Requires EEPROM init before starting.
*/
String API::readStringFromEEPROM(int addrOffset) {
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1];
  for (int i = 0; i < newStrLen; i++)
  {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  data[newStrLen] = '\0';
  return String(data);
}

/*!
    @brief  Ask server for token and check if server works.
            Saves token in EEPROM
    @return Http response code.
    @note   Requires EEPROM init before starting.
*/
int API::setup() {  
  String payload = "{\"mac_address\":\"" + WiFi.macAddress() + "\"}";

  apiCallResult result = API::POST("/api/monitor", payload, 10000);

  if (result.httpCode > 0) {
    if(result.response != "Monitor already in DB") {
      API::writeStringToEEPROM(0, "cJ8ccOSte3eysL6WKLv77pOMLXjMo303");
    }
  } 

  return result.httpCode;
}


/*!
    @brief  Sends data to server.
    @param  temperature
            Temperature value
    @param  humidity
            Relative humidity value
    @param  air_pressure
            Air pressure value in hPa
    @param  luminosity
            Luminosity value
    @param  movement
            Movement value
    @param  heatIndex
            Heat index value
    @return Http response code.
*/
int API::sendMeasurements(
    float temperature,
    float humidity,
    float air_pressure,
    float lumionsity,
    byte movement,
    float heatIndex) 
  {
    String token = API::readStringFromEEPROM(0);

    String payload = 
        "{\"monitor_mac\":\"" + (String)WiFi.macAddress() + "\"" +
        ",\"token\":\"" + token + "\"" +
        ",\"temperature\":\"" + (String)temperature + "\"" +
        ",\"humidity\":\"" + (String)humidity + "\"" +
        ",\"luminosity\":\"" + (String)lumionsity + "\"" +
        ",\"movement\":\"" + (String)movement +"\"" +
        ",\"heat_index\":\"" + (String)heatIndex +"\"" +
        ",\"air_pressure\":\"" + (String)air_pressure + "\"}";

    apiCallResult result = API::POST("/api/measurement", payload);

    return result.httpCode;
}

/*!
    @brief  POST API call.
    @param  endpoint
            Called endpoint. Start with /
    @param  payload
            Data being send with POST request in JSON format.
    @param  timeout
            Optional timeout value
    @return Returns  struct with http code and response payload.
    @note   On http response will be blank. To fix that configure your server to send Content-Length in header.
*/
API::apiCallResult API::POST(String endpoint, String payload, int timeout) {
  // For SSL use WiFiClientSecure and set insecure
  WiFiClientSecure client;
  client.setInsecure();

  // For http use WiFiClient
  // WiFiClient client;   
  
  HTTPClient http;

  http.begin(client, baseUrl + endpoint);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(timeout);

  int httpCode = http.POST(payload);

  Serial.println(baseUrl + endpoint);

  if (httpCode > 0) {
    Serial.printf("[HTTP] POST... code: %d\n", httpCode);
  } else {
    Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  const String& response = http.getString();
  Serial.println("response: " + response);

  apiCallResult result = { httpCode, response };

  http.end();

  return result;
}