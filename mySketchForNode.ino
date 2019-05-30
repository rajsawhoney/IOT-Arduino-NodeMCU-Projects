#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
 
const char* ssid  = "Pabitra_dhrn";
const char* password = "1789211155";
String host = "prashantrajtest.000webhostapp.com"; 
String url="/api/Led/read_all.php?id=1";
String globalStatus;
void setup()
{
  pinMode(D1,OUTPUT);
  Serial.begin(115200);
  Serial.println("Making the world go round!!!!Please wait.....");
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Netmask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
  
}

void LedControl()
{
  WiFiClient client;
  Serial.println("Led control signal");
  Serial.print("Requesting URL: ");
  Serial.println(url);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +"Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
  delay(500);
  String section="header";
  while(client.available()){
    String line = client.readStringUntil('\r');
    if (section=="header"){ // headers..
      if (line=="\n") // skips the empty space at the beginning 
        section="json";
    }
    else if (section=="json"){  // print the good stuff
      section="ignore";
      String result = line.substring(1);
      // Parse JSON
      int size = result.length() + 1;
      char json[size];
      result.toCharArray(json, size);
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& json_parsed = jsonBuffer.parseObject(json);
      if (!json_parsed.success())
      {
        Serial.println("parseObject() failed");
        return;
      }
      String ledStatus = json_parsed["Led"][0]["status"];
      Serial.println(ledStatus);
      globalStatus=ledStatus;
    }
    }
  }

void loop()
{
  Serial.print("connecting to ");
  Serial.println(host);
  LedControl();
  delay(500);
  Serial.println(globalStatus);
  if(globalStatus=="on")
  {
    digitalWrite(D1,HIGH);
    Serial.println("Led is On!!!!");
  }
  else if(globalStatus=="off")
  {
    digitalWrite(D1,LOW);
    Serial.println("Led is Off!!!!");
  }
  else
  {
    Serial.println("Invalid command sent!!!!");
  }
  }
