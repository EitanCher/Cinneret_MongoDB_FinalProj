#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "FinalProj";
//const char* pswd = "55555333";

WiFiClient client;
int server_port = 80;

void wifi_Setup() {
  Serial.println("wifiSetup");
  WiFi.begin(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Attempting to connect to network...");
    delay(100);
  }
  Serial.println("Connected to network");
}

int GetDuration(short PhaseNum) {
  string MyInput = to_string(PhaseNum);
  HTTPClient http;
  http.begin(client, "http://77.137.39.113:6655/R/" + MyInput);
  int httpCode = http.GET();
  Serial.println(httpCode);
  if (httpCode == HTTP_CODE_OK) {
    Serial.print("HTTP response code ");
    Serial.println(httpCode);
    String Res = http.getString();
    Serial.println(Res);
    int ret = Res.toInt();
  }
  http.end();

  return ret;
}
