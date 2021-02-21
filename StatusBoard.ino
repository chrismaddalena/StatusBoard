#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_NeoPixel.h>
#include "index.h"

// Set Web Server Port - Default is 80
ESP8266WebServer server(80);

// Variable to store the HTTP request
String header;

// WiFi Settings - Update with SSID and PSK
const char* ssid     = "YOUR_SSID";
const char* password = "YOUR_PW";

/*
  * Neopixel Strip Settings
  * 
  * Update ``STRIP_PIN`` with the PIN number for your LED strip
  * Update ``STRIP_LEDS`` with the number of pixels in your LED strip
  * The ``WAIT`` var is time (milliseconds) between each LED in strip turning on/off
  *   Set to ``0`` to turn on/off immediately
  *   A value of ``50`` looks nice, but will also slightly delay the status toggling on the webpage
  *   Status updates once the animation finishes
  *   Total Seconds = (WAIT * STRIP_LEDS) / 1000
*/
#define STRIP_PIN 2
#define STRIP_LEDS 22
Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIP_LEDS, STRIP_PIN, NEO_GRB + NEO_KHZ800);
int WAIT = 25;

/* 
 * Neopixel Single LED Settings
 * 
 * Update the ``x_PIN`` vars with the PIN numbers for your LEDs 
 * Keep track of your LED and pin combinations for proper placement in your display
 * Each LED is assumed to be a single LED, so LED count is set to ``1``
 */
#define DND_PIN 12
#define COFFEE_PIN 13
#define HEADPHONES_PIN 15
Adafruit_NeoPixel dnd_pixel = Adafruit_NeoPixel(1, DND_PIN, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel coffee_pixel = Adafruit_NeoPixel(1, COFFEE_PIN, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel headphones_pixel = Adafruit_NeoPixel(1, HEADPHONES_PIN, NEO_RGB + NEO_KHZ800);

/*
 * Neopixel Colors
 * 
 * Set the color RGB values to your liking
 * Note that ``255`` is maximum brightness for that color
 * The following colors are roughly half brightness
 */
uint32_t strip_color = strip.Color(127, 0, 0);  // Red
uint32_t dnd_color = dnd_pixel.Color(127, 0, 127);  // Magenta
uint32_t coffee_color = coffee_pixel.Color(102, 127, 25); // Light Green
uint32_t headphones_color = headphones_pixel.Color(0, 76, 127); // Light Blue

/*
 * Starting State of Neopixels
 * 
 * These should all be false unless you want them to default to ON at boot
 */
bool STRIP = false;
bool DND_PIXEL = false;
bool HEADPHONES_PIXEL = false;
bool COFFEE_PIXEL = false;

/*
 * Begin Custom Functions
 * 
 * These functions are wired to the API endpoints
 */

void colorWipe(uint32_t c, uint8_t wait) {
  // Fill the strip LEDs one after the other with a color
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void stripToggle(){
  if (STRIP) {
    colorWipe(strip.Color(0, 0, 0), WAIT);
    STRIP = false;
  } else {
    colorWipe(strip_color, WAIT);
    STRIP = true;
  }
  if (STRIP) {
    String json = "{\"result\": true, \"message\": \"Meeting LED On\"}";
    server.send(200, "application/json", json);
  } else {
    String json = "{\"result\": false, \"message\": \"Meeting LED Off\"}";
    server.send(200, "application/json", json);
  }
}

void headphonesToggle(){
  if (HEADPHONES_PIXEL) {
    headphones_pixel.setPixelColor(0, 0, 0, 0);
    headphones_pixel.show();
    HEADPHONES_PIXEL = false;
  } else {
    headphones_pixel.setPixelColor(0, headphones_color);
    headphones_pixel.setBrightness(75);
    headphones_pixel.show();
    HEADPHONES_PIXEL = true;
  }
  if (HEADPHONES_PIXEL) {
    String json = "{\"result\": true, \"message\": \"Headphones LED On\"}";
    server.send(200, "application/json", json);
  } else {
    String json = "{\"result\": false, \"message\": \"Headphones LED Off\"}";
    server.send(200, "application/json", json);
  }
}

void dndToggle(){
  if (DND_PIXEL) {
    dnd_pixel.setPixelColor(0, 0, 0, 0);
    dnd_pixel.show();
    DND_PIXEL = false;
  } else {
    dnd_pixel.setPixelColor(0, dnd_color);
    dnd_pixel.setBrightness(75);
    dnd_pixel.show();
    DND_PIXEL = true;
  }
  if (DND_PIXEL) {
    String json = "{\"result\": true, \"message\": \"DND LED On\"}";
    server.send(200, "application/json", json);
  } else {
    String json = "{\"result\": false, \"message\": \"DND LED Off\"}";
    server.send(200, "application/json", json);
  }
}

void coffeeToggle(){
  if (COFFEE_PIXEL) {
    coffee_pixel.setPixelColor(0, 0, 0, 0);
    coffee_pixel.show();
    COFFEE_PIXEL = false;
  } else {
    coffee_pixel.setPixelColor(0, coffee_color);
    coffee_pixel.setBrightness(75);
    coffee_pixel.show();
    COFFEE_PIXEL = true;
  }

  if (COFFEE_PIXEL) {
    String json = "{\"result\": true, \"message\": \"Coffee LED On\"}";
    server.send(200, "application/json", json);
  } else {
    String json = "{\"result\": false, \"message\": \"Coffee LED Off\"}";
    server.send(200, "application/json", json);
  }
}

void getStatus(){
  String strip_status = "false";
  String headphones_status = "false";
  String dnd_status = "false";
  String coffee_status = "false";
  if (STRIP) {
    strip_status = "true";
  }
  if (HEADPHONES_PIXEL) {
    headphones_status = "true";
  }
  if (DND_PIXEL) {
    dnd_status = "true";
  }
  if (COFFEE_PIXEL) {
    coffee_status = "true";
  }
  String json = "{\"meeting\":" + strip_status + ", \"headphones\":" + headphones_status + ", \"dnd\":" + dnd_status + ", \"coffee\":" + coffee_status + "}";
  server.send(200, "application/json", json);
}

void resetAll(){
  colorWipe(strip.Color(0, 0, 0), WAIT);
  STRIP = false;

  dnd_pixel.setPixelColor(0, 0, 0, 0);
  dnd_pixel.show();
  DND_PIXEL = false;

  headphones_pixel.setPixelColor(0, 0, 0, 0);
  headphones_pixel.show();
  HEADPHONES_PIXEL = false;

  coffee_pixel.setPixelColor(0, 0, 0, 0);
  coffee_pixel.show();
  COFFEE_PIXEL = false;

  String json = "{\"result\": true, \"message\": \"Reset all LEDS to off\"}";
  server.send(200, "application/json", json);
}

void root() {
  server.send(200, "text/html", INDEX_HTML);
}

/*
 * Configure Web Server
 */

void setup() {
  // Initialize Neopixel strip with all pixels off
  strip.begin();
  dnd_pixel.begin();
  headphones_pixel.begin();
  coffee_pixel.begin();

  colorWipe(strip.Color(0, 0, 0), 50);
  dnd_pixel.setPixelColor(0, 0, 0, 0);
  dnd_pixel.show();
  headphones_pixel.setPixelColor(0, 0, 0, 0);
  headphones_pixel.show();
  coffee_pixel.setPixelColor(0, 0, 0, 0);
  coffee_pixel.show();

  // Begin serial connection for debugging
  Serial.begin(115200);
  delay(100);

  // Start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected"); 
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Associate URL paths with custom handler functions
  server.on("/", root);
  server.on("/meeting", stripToggle);
  server.on("/meeting/", stripToggle);
  server.on("/dnd", dndToggle);
  server.on("/dnd/", dndToggle);
  server.on("/headphones", headphonesToggle);
  server.on("/headphones/", headphonesToggle);
  server.on("/coffee", coffeeToggle);
  server.on("/coffee/", coffeeToggle);
  server.on("/status", getStatus);
  server.on("/status/", getStatus);
  server.on("/reset", resetAll);
  server.on("/reset/", resetAll);

  // Start the web server
  server.begin();
  Serial.println("Ready!");
}

// Repeating loop for handling web requests
void loop() {
  server.handleClient();
}
