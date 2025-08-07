/*
 *  This sketch sends a message to a TCP server
 *
 */

#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti WiFiMulti;

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network
  WiFiMulti.addAP("SSID", "passpasspass");

  Serial.println();
  Serial.println();
  Serial.print("Waiting for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);
}

void loop() {
  //    const uint16_t port = 80;
  //    const char * host = "192.168.1.1"; // ip or dns
  const uint16_t port = 1337;
  const char *host = "192.168.1.10";  // ip or dns

  Serial.print("Connecting to ");
  Serial.println(host);

  // Use NetworkClient class to create TCP connections
  NetworkClient client;

  if (!client.connect(host, port)) {
    Serial.println("Connection failed.");
    Serial.println("Waiting 5 seconds before retrying...");
    delay(5000);
    return;
  }

  // This will send a request to the server
  //uncomment this line to send an arbitrary string to the server
  //client.print("Send this data to the server");
  //uncomment this line to send a basic document request to the server
  client.print(String("GET /ready.json HTTP/1.1\r\n") +
             "Host: 192.168.1.10\r\n" +
             "Connection: close\r\n\r\n");
  int maxloops = 0;

  //wait for the server's reply to become available
  while (!client.available() && maxloops < 1000) {
    maxloops++;
    delay(1);  //delay 1 msec
  }
  if (client.available() > 0) {
    //read back one line from the server
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r" || line.length() == 0) {
        break; // Headers ended
  }
}
  } else {
    Serial.println("client.available() timed out ");
  }
  String json = client.readString();
  Serial.println("Response body:");
  Serial.println(json);
  Serial.println("Closing connection.");
  client.stop();

   if (json.indexOf("\"ready\": true") != -1) {
    Serial.println("Firmware is ready — starting download...");

    WiFiClient fwClient;
    if (!fwClient.connect(host, port)) {
      Serial.println("Firmware connection failed.");
      return;
    }

    fwClient.print(String("GET /firmware.bin HTTP/1.1\r\n") +
                   "Host: " + host + "\r\n" +
                   "Connection: close\r\n\r\n");

    // Skip headers
    while (fwClient.connected()) {
      String line = fwClient.readStringUntil('\n');
      if (line == "\r" || line.length() == 0) break;
    }

    // Now read binary firmware
    uint8_t buffer[512];
    size_t totalBytes = 0;

    while (fwClient.available()) {
      int bytesRead = fwClient.read(buffer, sizeof(buffer));
      totalBytes += bytesRead;

      // For now, just report size
      Serial.printf("Read %d bytes\n", bytesRead);

      // Later: send to STM32 via Serial2.write(buffer, bytesRead);
    }

    Serial.printf("Firmware download complete. Total: %d bytes\n", totalBytes);
    fwClient.stop();
  } else {
    Serial.println("Firmware not ready.");
  }

  delay(5000);  // Wait before next check
}
