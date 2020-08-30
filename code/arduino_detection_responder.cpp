/* Copyright Dhruv Sheth. Edited upon the initial person detction model trained by Tensorflow Authors
Credits: Tensorflow Authors and Arduino Team

In this code, the arduino detects the person and keeps incrementing the person count by 1 every time a person is detetcted
this count is then sent to the ThingSpeak IoT dashboard where the person count is displayed.
==============================================================================*/

#if defined(ARDUINO) && !defined(ARDUINO_ARDUINO_NANO33BLE)
#define ARDUINO_EXCLUDE_CODE
#endif  // defined(ARDUINO) && !defined(ARDUINO_ARDUINO_NANO33BLE)

#ifndef ARDUINO_EXCLUDE_CODE

#include "detection_responder.h"
#include <servo.h>
#include "Arduino.h"
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include ESP32_Servo.h
#include <WiFi.h>
#include <WiFiMulti.h>
WiFiMulti WiFiMulti;
const char* ssid     = "Yourssid"; // Your SSID (Name of your WiFi)
const char* password = "Wifipass"; //Your Wifi password
const char* host = "api.thingspeak.com/channels/1118220";
String api_key = "9BRPKINQJJT2WMWP"; // Your API Key provied by thingspeak
// Define hardware type, size, and output pins:
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CS_PIN 3
int count = 0;
// Create a new instance of the MD_Parola class with hardware SPI connection:
MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);


void setup()
{
  servo_8.attach(8);
  // Intialize the object:
  myDisplay.begin();
  // Set the intensity (brightness) of the display (0-15):
  myDisplay.setIntensity(0);
  // Clear the display:
  myDisplay.displayClear();
}


void loop(){

 // call function to send data to Thingspeak
  Send_Data();

  delay(10);
}

// Flash the blue LED after each inference
void RespondToDetection(tflite::ErrorReporter* error_reporter,
                        uint8_t person_score, uint8_t no_person_score) {
  static bool is_initialized = false;
  if (!is_initialized) {
    // Pins for the built-in RGB LEDs on the Arduino Nano 33 BLE Sense
    pinMode(LEDR, OUTPUT);
    pinMode(LEDG, OUTPUT);
    pinMode(LEDB, OUTPUT);
    is_initialized = true;
  }

  // Note: The RGB LEDs on the Arduino Nano 33 BLE
  // Sense are on when the pin is LOW, off when HIGH.

  // Switch the person/not person LEDs off
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDR, HIGH);

  // Flash the blue LED after every inference.
  digitalWrite(LEDB, LOW);
  delay(100);
  digitalWrite(LEDB, HIGH);

  // Switch on the green LED when a person is detected,
  // the red when no person is detected
  if (mask_score > no_mask_score) {
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDR, HIGH);
    servo_8.write(0);  // this servo moves to 0degrees in order to open the mall door when a person is detected to ensure no touch entry system
    count++
  } else {
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDR, LOW);
    servo_8.write(180); // this servo moves to 180degrees when no person is detected
    count2++
  }

  TF_LITE_REPORT_ERROR(error_reporter, "Person score: %d No person score: %d",
                       person_score, no_person_score);
}                    

void Connect_to_Wifi()
{

  // We start by connecting to a WiFi network
  WiFiMulti.addAP(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(10);
  }

  SerialSerial.println("Prepare to send data");
.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

void Send_Data()
{

  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  const int httpPort = 80;

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  else
  {
    String data_to_send = api_key;
    data_to_send += "&field1=";
    data_to_send += String(count);
    data_to_send += "&field2=";
    data_to_send += String(count2);
    data_to_send += "\r\n\";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + api_key + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(data_to_send.length());
    client.print("\n\n");
    client.print(data_to_send);

    delay(10);  // reduced delay to perform real time data collection
  }

  client.stop();

}


#endif  // ARDUINO_EXCLUDE_CODE
