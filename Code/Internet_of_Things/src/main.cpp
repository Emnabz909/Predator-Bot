#include <Arduino.h>
#include "PrintStream.h"
#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_MLX90640.h>
#include <Wire.h>
#include <SPI.h>

// Define the MLX90640
Adafruit_MLX90640 mlx; ///< Instance of the MLX90640 thermal camera.
#define MLX90640_I2C_ADDR 0x33 ///< I2C address of the MLX90640 sensor.
#define WIDTH 32              ///< Width of the thermal image.
#define HEIGHT 24             ///< Height of the thermal image.
float mlx90640Frame[WIDTH * HEIGHT]; ///< Array to store thermal image data.

// WiFi credentials
const char* ssid = "Predator_Bot"; ///< WiFi SSID for the access point.
const char* password = "Gabi405!"; ///< WiFi password for the access point.
IPAddress local_ip(192, 168, 5, 1); ///< Local IP address of the ESP32.
IPAddress gateway(192, 168, 5, 1);  ///< Gateway IP address.
IPAddress subnet(255, 255, 255, 0); ///< Subnet mask for the WiFi network.

// Web Server
WebServer server(80); ///< HTTP web server instance.

/**
 * @brief Initializes the WiFi as an access point.
 */
void setup_wifi(void) {
    Serial.println("Setting up WiFi access point...");
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    WiFi.softAP(ssid, password);
    Serial.println("done.");
}

/**
 * @brief Generates an HTML header for the webpage.
 * 
 * @param a_string Reference to the HTML string being constructed.
 * @param page_title Title of the webpage.
 */
void HTML_header(String& a_string, const char* page_title) {
    a_string += "<!DOCTYPE html> <html>\n";
    a_string += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n<title>";
    a_string += page_title;
    a_string += "</title>\n";
    a_string += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
    a_string += "body{margin-top: 50px;} h1 {color: #4444AA;margin: 50px auto 30px;}\n";
    a_string += "p {font-size: 24px;color: #222222;margin-bottom: 10px;}\n";
    a_string += "</style>\n</head>\n";
}

/**
 * @brief Handles HTTP requests to the root page.
 */
void handle_DocumentRoot() {
    Serial.println("HTTP request from client");

    String a_str;
    HTML_header(a_str, "ESP32 Thermal Camera");
    a_str += "<body>\n<div id=\"webpage\">\n";
    a_str += "<h1>Thermal Camera Data</h1>\n";
    a_str += "<p><a href=\"/toggle\">Toggle LED</a></p>\n";
    a_str += "<p><a href=\"/thermal\">Show Thermal Data</a></p>\n";
    a_str += "</div>\n</body>\n</html>\n";
    server.send(200, "text/html", a_str);
}

/**
 * @brief Handles HTTP requests for thermal camera data.
 */
void handle_ThermalData(void) {
    String thermal_data = "<!DOCTYPE html> <html><body>\n";
    thermal_data += "<h1>Thermal Camera Data</h1>\n";

    int status = mlx.getFrame(mlx90640Frame);
    if (status != 0) {
        thermal_data += "<p>Error reading from MLX90640 sensor.</p>";
    } else {
        thermal_data += "<table border=\"1\"><tr>";
        // Create a table for thermal data
        for (int y = 0; y < HEIGHT; y++) {
            thermal_data += "<tr>";
            for (int x = 0; x < WIDTH; x++) {
                int index = (y * WIDTH) + x;
                float temp = mlx90640Frame[index];

                // Display temperature values in the table
                thermal_data += "<td>";
                thermal_data += String(temp, 2);
                thermal_data += "&deg;C</td>";
            }
            thermal_data += "</tr>";
        }
        thermal_data += "</table>";
    }
    thermal_data += "<br><a href=\"/\">Back to main page</a>";
    thermal_data += "</body></html>\n";

    server.send(200, "text/html", thermal_data);
}

/**
 * @brief Handles HTTP requests for undefined pages (404).
 */
void handle_NotFound(void) {
    server.send(404, "text/plain", "Not found");
}

/**
 * @brief Configures the MLX90640 thermal camera.
 */
void setup_MLX90640() {
    Wire.begin(21, 22, 400000); // SDA: 21, SCL: 22, Clock Speed: 400kHz
    if (!mlx.begin(MLX90640_I2C_ADDR, &Wire)) {
        Serial.println("Failed to find MLX90640. Check connections and address!");
        while (1);
    }
    mlx.setMode(MLX90640_CHESS);           // Chess mode for reduced noise
    mlx.setResolution(MLX90640_ADC_18BIT); // Highest resolution
    mlx.setRefreshRate(MLX90640_8_HZ);     // Refresh rate: 8 Hz
    Serial.println("MLX90640 configuration complete.");
}

/**
 * @brief Main setup function for the ESP32.
 */
void setup() {
    Serial.begin(115200);
    while (!Serial) { }
    Serial.println("Starting ESP32 Web Server...");

    setup_wifi();

    // Setup MLX90640
    setup_MLX90640();

    // Configure WebServer routes
    server.on("/", handle_DocumentRoot);
    server.onNotFound(handle_NotFound);

    server.begin();
    Serial.println("HTTP server started");
}

/**
 * @brief Main loop function for the ESP32.
 */
void loop() {
    server.handleClient();
}
