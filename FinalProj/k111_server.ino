#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "yyyy";
const char* pswd = "6666";

WiFiClient client;
int server_port = 80;

void wifi_Setup() {
  Serial.println("wifiSetup");
  WiFi.begin(ssid, pswd);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Attempting to connect to network...");
    delay(100);
  }
  Serial.println("Connected to network");
}

int GetDuration(int PhaseNum) {
  int ret = -1;
  HTTPClient http;
  
  String MyInput = String(PhaseNum);
  http.begin(client, "http://192.168.1.17:6655/R/" + MyInput);
  int httpCode = http.GET();
  Serial.println(httpCode);

  if (httpCode == HTTP_CODE_OK) {
    Serial.print("HTTP response code ");
    Serial.println(httpCode);
    String Res = http.getString();
    Serial.println(Res);
    ret = Res.toInt();
  }
  http.end();

  return ret;
}
