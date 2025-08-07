#include <WiFi.h>

const char* ssid = "Vodafone-DAE4";
const char* password = "sAKsg2RayehdfmAs";

WiFiServer server(80);

String header;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);  // RX, TX for Serial2

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.accept();
  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            // Respond to client
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Parse commands from header
            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("Sending '26 ON' to STM32");
              Serial2.println("26 ON");
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("Sending '26 OFF' to STM32");
              Serial2.println("26 OFF");
            } else if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("Sending '27 ON' to STM32");
              Serial2.println("27 ON");
            } else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("Sending '27 OFF' to STM32");
              Serial2.println("27 OFF");
            }

            // Send HTML response with buttons (simplified)
            client.println("<html><body>");
            client.println("<h1>ESP32 Web Server</h1>");
            client.println("<p><a href=\"/26/on\">GPIO 26 ON</a></p>");
            client.println("<p><a href=\"/26/off\">GPIO 26 OFF</a></p>");
            client.println("<p><a href=\"/27/on\">GPIO 27 ON</a></p>");
            client.println("<p><a href=\"/27/off\">GPIO 27 OFF</a></p>");
            client.println("</body></html>");

            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected");
  }
}
