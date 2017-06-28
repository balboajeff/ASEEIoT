#include <Servo.h>
#include <ESP8266WiFi.h>

Servo myServo;

// WiFi config
const char ssid[] = "sparkfun-workshop";
const char password[] = "sparkfun";

// ThingSpeak config
const String channel_id = "277951";

// Server, file, and port
const char hostname[] = "api.thingspeak.com";
const String uri = "/channels/" + channel_id + "/fields/1/last.txt";
const int port = 80;

// Global variables
WiFiClient client;
int temp;

void setup() {

  // Pour a bowl of serial
  Serial.begin(9600);
  
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // Wait until we are connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Show our IP address
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  myServo.attach(5);
  myServo.write(20);
}

void loop() {

  // Open TCP connection to host
  Serial.print("Connecting to ");
  Serial.println(hostname);
  if ( !client.connect(hostname, port) ) {
    Serial.println("TCP connection failed");
    return;
  }

  // Build GET request
  String req = "GET " + uri + " HTTP/1.1\r\n" + 
                "Host: " + hostname + "\r\n" +
                "Connection: close\r\n" +
                "\r\n";
  
  // Send request to server
  Serial.print(req);
  client.print(req);

  // Wait for response from server
  while ( client.available() == 0 ) {;
    delay(1);
  }

  // Look for end of response header
  if ( !client.find("\r\n\r\n") ) {
    Serial.println("HTTP response not found");
  }

  // Ignore rogue integer
  if ( client.find("\r\n") ) {
    temp = client.parseInt();
    Serial.print("Temperature: ");
    Serial.println(temp);
    Serial.println();
  }

  // Close TCP connection
  client.stop();
  int val=map(temp,65,88,5,170);
myServo.write(temp);
  delay(10000);
}

