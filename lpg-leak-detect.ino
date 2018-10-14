#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

void handleRoot();
void handleLpglvl();
void handleNotFound();

void setup()
{
  Serial.begin(9600);
  Serial.println("Initializing wifi ops...");

  pinMode(D1, OUTPUT);//PWR indicator
  pinMode(D2, OUTPUT);// Comms indicator
  pinMode(D9, OUTPUT);//LPG critical level indicator
  digitalWrite(D1, HIGH);
  digitalWrite(D9, LOW);


  WiFi.mode(WIFI_STA);
  WiFi.disconnect(false);
  WiFi.persistent(false);

  delay(500);

  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.begin("your_ssid_name", "your_ssid_pwd");

    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
    Serial.println();
  }

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("lpgleakdet"))
  {
    Serial.println("mDNS responder started...");
  }
  else
  {
    Serial.println(" !!! Error setting up mDNS responder!");
  }

  server.on("/", handleRoot);
  server.on("/lpglvl", handleLpglvl);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP Server running....");
}

void loop()
{
  server.handleClient();
}

void handleRoot()
{
  Serial.println("New client...");
  server.send(200, "text/plain", "Defunct url. query on /lpglvl");
  Serial.println("response sent, notified correct url...");
}

void handleLpglvl()
{
  int iLvl = analogRead( A0 );
  if (iLvl >= 200)
  {
    digitalWrite(D9, HIGH);
  }
  else
  {
    digitalWrite(D9, LOW);
  }
  String strLvl = String(iLvl);

  digitalWrite(D2, HIGH);
  Serial.println("New client...");
  server.send( 200, "text/plain", strLvl);
  Serial.println("response sent...");
  digitalWrite(D2, LOW);
}

void handleNotFound()
{
  server.send(404, "text/plain", "404: Not found");
}

