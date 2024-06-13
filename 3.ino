#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

const char *serverURL = "http://192.168.0.115:5000/sensor/data";
const char *ssid = "SamsungInstitute";
const char *password = "112233445566";

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }

    Serial.println("Connected to the WiFi network");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    Serial.println("Deteksi Suhu dan kelembaban");
    dht.begin();
}

void loop()
{
    delay(2000);
    HTTPClient http;

    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (isnan(h) || isnan(t))
    {
        Serial.println("Sensor tidak terbaca!");
        return;
    }

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" *C ");

    String payload = "{";
    payload += "\"temperature\":";
    payload += String(t);
    payload += ",";
    payload += "\"kelembapan\":";
    payload += String(h);
    payload += ",";
    payload += "\"timestamp\":";
    payload += String(millis());
    payload += "}";

    http.begin(serverURL);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0)
    {
        String response = http.getString();
        Serial.println(httpResponseCode);
        Serial.println(response);
    }
    else
    {
        Serial.print("Error on sending POST: ");
        Serial.println(httpResponseCode);
    }

    http.end();
}
